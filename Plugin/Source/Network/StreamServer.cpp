/*
  ==============================================================================
   GSF FIGHTER - Stream Server Implementation
  ==============================================================================
*/

#include "StreamServer.h"

StreamServer::StreamServer()
    : Thread("GSF StreamServer")
{
    fifoL.resize(131072, 0.0f);
    fifoR.resize(131072, 0.0f);
}

StreamServer::~StreamServer()
{
    stop();
}

void StreamServer::configure(uint32_t sr, uint16_t bd, uint16_t nc, int spb)
{
    sampleRate     = sr;
    bitDepth       = bd;
    numChannels    = nc;
    samplesPerBlock = spb;
}

void StreamServer::start(const std::array<uint8_t, 4>& connectionCode)
{
    if (running.load())
        return;

    connCode = connectionCode;
    running.store(true);
    audioFifo.reset();
    sequenceNumber = 0;

    // Start TCP server for client connections
    tcpServer = std::make_unique<juce::StreamingSocket>();
    if (!tcpServer->createListener(gsf::kStreamPort, "0.0.0.0"))
    {
        DBG("GSF FIGHTER: Failed to create TCP listener on port " << gsf::kStreamPort);
        running.store(false);
        return;
    }

    // Start discovery broadcaster
    discovery = std::make_unique<DeviceDiscovery>();
    discovery->startBroadcasting(connCode, sampleRate, bitDepth, numChannels, 0, gsf::kMaxClients);

    // Start server thread
    startThread(juce::Thread::Priority::high);
}

void StreamServer::stop()
{
    running.store(false);

    if (discovery)
    {
        discovery->stopBroadcasting();
        discovery.reset();
    }

    // Disconnect all clients
    disconnectAll();

    if (tcpServer)
    {
        tcpServer->close();
        tcpServer.reset();
    }

    stopThread(3000);
}

void StreamServer::pushAudioBlock(const float* left, const float* right, int numSamples)
{
    const auto scope = audioFifo.write(numSamples);

    if (scope.blockSize1 > 0)
    {
        std::memcpy(fifoL.data() + scope.startIndex1, left, sizeof(float) * scope.blockSize1);
        std::memcpy(fifoR.data() + scope.startIndex1, right, sizeof(float) * scope.blockSize1);
    }
    if (scope.blockSize2 > 0)
    {
        std::memcpy(fifoL.data() + scope.startIndex2, left + scope.blockSize1, sizeof(float) * scope.blockSize2);
        std::memcpy(fifoR.data() + scope.startIndex2, right + scope.blockSize1, sizeof(float) * scope.blockSize2);
    }
}

int StreamServer::getClientCount() const
{
    std::lock_guard<std::mutex> lock(clientMutex);
    int count = 0;
    for (const auto& c : clients)
        if (c && c->isConnected())
            ++count;
    return count;
}

std::vector<ClientInfo> StreamServer::getClientList() const
{
    std::lock_guard<std::mutex> lock(clientMutex);
    std::vector<ClientInfo> list;
    for (const auto& c : clients)
        if (c && c->isConnected())
            list.push_back(c->getInfo());
    return list;
}

void StreamServer::disconnectClient(int clientIndex)
{
    std::lock_guard<std::mutex> lock(clientMutex);
    if (clientIndex >= 0 && clientIndex < static_cast<int>(clients.size()))
    {
        if (clients[clientIndex])
            clients[clientIndex]->disconnect();
    }
}

void StreamServer::disconnectAll()
{
    std::lock_guard<std::mutex> lock(clientMutex);
    for (auto& c : clients)
        if (c)
            c->disconnect();
    clients.clear();
}

void StreamServer::setClientPreset(int clientIndex, gsf::PresetID preset)
{
    std::lock_guard<std::mutex> lock(clientMutex);
    if (clientIndex >= 0 && clientIndex < static_cast<int>(clients.size()))
    {
        if (clients[clientIndex] && clients[clientIndex]->isConnected())
        {
            clients[clientIndex]->setPreset(preset);

            // Send preset change notification
            gsf::PacketHeader header {};
            header.magic       = gsf::kProtocolMagic;
            header.version     = gsf::kProtocolVersion;
            header.type        = gsf::PacketType::PresetSync;
            header.payloadSize = sizeof(gsf::PresetChangePayload);
            header.sequenceNum = sequenceNumber++;

            gsf::PresetChangePayload payload {};
            payload.preset = preset;

            clients[clientIndex]->sendControlPacket(header, &payload, sizeof(payload));
        }
    }
}

void StreamServer::run()
{
    lastHeartbeatTime = juce::Time::currentTimeMillis();

    while (!threadShouldExit() && running.load())
    {
        // 1. Accept new connections (non-blocking check)
        handleIncomingConnections();

        // 2. Send buffered audio to all clients
        sendAudioToClients();

        // 3. Heartbeats & cleanup
        auto now = juce::Time::currentTimeMillis();
        if (now - lastHeartbeatTime > kHeartbeatIntervalMs)
        {
            sendHeartbeats();
            cleanupStaleClients();
            lastHeartbeatTime = now;
        }

        // Sleep briefly to avoid busy-waiting (sub-millisecond granularity)
        juce::Thread::sleep(1);
    }
}

void StreamServer::handleIncomingConnections()
{
    if (!tcpServer || !tcpServer->isConnected())
        return;

    // Non-blocking accept
    auto* newSocket = tcpServer->waitForNextConnection();
    if (newSocket == nullptr)
        return;

    // Read connection request
    gsf::PacketHeader header {};
    int bytesRead = newSocket->read(&header, sizeof(header), false);

    if (bytesRead == sizeof(header) && header.magic == gsf::kProtocolMagic
        && header.type == gsf::PacketType::ConnectRequest)
    {
        gsf::ConnectRequestPayload request {};
        bytesRead = newSocket->read(&request, sizeof(request), false);

        if (bytesRead == sizeof(request))
        {
            // Validate connection code
            bool codeMatch = std::memcmp(request.connectionCode, connCode.data(), 4) == 0;

            if (codeMatch && getClientCount() < gsf::kMaxClients)
            {
                // Accept connection
                gsf::PacketHeader acceptHeader {};
                acceptHeader.magic       = gsf::kProtocolMagic;
                acceptHeader.version     = gsf::kProtocolVersion;
                acceptHeader.type        = gsf::PacketType::ConnectAccept;
                acceptHeader.payloadSize = sizeof(gsf::AudioConfigPayload);
                acceptHeader.sequenceNum = sequenceNumber++;

                gsf::AudioConfigPayload config {};
                config.sampleRate     = sampleRate;
                config.bitDepth       = bitDepth;
                config.numChannels    = numChannels;
                config.samplesPerBlock = static_cast<uint16_t>(samplesPerBlock);

                auto wrappedSocket = std::unique_ptr<juce::StreamingSocket>(newSocket);
                wrappedSocket->write(&acceptHeader, sizeof(acceptHeader));
                wrappedSocket->write(&config, sizeof(config));

                auto session = std::make_unique<ClientSession>(
                    juce::String(request.deviceName),
                    wrappedSocket->getHostName(),
                    wrappedSocket->getPort(),
                    std::move(wrappedSocket));

                session->setPreset(request.requestedPreset);

                std::lock_guard<std::mutex> lock(clientMutex);
                clients.push_back(std::move(session));

                if (discovery)
                    discovery->updateClientCount(static_cast<uint8_t>(clients.size()));

                DBG("GSF FIGHTER: Client connected - " << request.deviceName);
            }
            else
            {
                // Reject
                gsf::PacketHeader rejectHeader {};
                rejectHeader.magic       = gsf::kProtocolMagic;
                rejectHeader.version     = gsf::kProtocolVersion;
                rejectHeader.type        = gsf::PacketType::ConnectReject;
                rejectHeader.payloadSize = 0;
                rejectHeader.sequenceNum = sequenceNumber++;

                newSocket->write(&rejectHeader, sizeof(rejectHeader));
                newSocket->close();
                delete newSocket;
            }
        }
        else
        {
            newSocket->close();
            delete newSocket;
        }
    }
    else
    {
        newSocket->close();
        delete newSocket;
    }
}

void StreamServer::sendAudioToClients()
{
    const int available = audioFifo.getNumReady();
    if (available < samplesPerBlock)
        return;

    const int toRead = juce::jmin(available, samplesPerBlock);

    // Read from FIFO
    std::vector<float> tempL(toRead), tempR(toRead);

    const auto scope = audioFifo.read(toRead);

    if (scope.blockSize1 > 0)
    {
        std::memcpy(tempL.data(), fifoL.data() + scope.startIndex1, sizeof(float) * scope.blockSize1);
        std::memcpy(tempR.data(), fifoR.data() + scope.startIndex1, sizeof(float) * scope.blockSize1);
    }
    if (scope.blockSize2 > 0)
    {
        std::memcpy(tempL.data() + scope.blockSize1, fifoL.data() + scope.startIndex2, sizeof(float) * scope.blockSize2);
        std::memcpy(tempR.data() + scope.blockSize1, fifoR.data() + scope.startIndex2, sizeof(float) * scope.blockSize2);
    }

    // Build audio packet
    const int pcmSize = toRead * numChannels * 3; // 24-bit = 3 bytes per sample
    const int headerSize = sizeof(gsf::PacketHeader);
    const int payloadHeaderSize = sizeof(gsf::AudioDataPayload);
    const int totalSize = headerSize + payloadHeaderSize + pcmSize;

    std::vector<uint8_t> packet(totalSize);

    // Header
    gsf::PacketHeader header {};
    header.magic       = gsf::kProtocolMagic;
    header.version     = gsf::kProtocolVersion;
    header.type        = gsf::PacketType::AudioData;
    header.payloadSize = static_cast<uint16_t>(payloadHeaderSize + pcmSize);
    header.sequenceNum = sequenceNumber++;

    std::memcpy(packet.data(), &header, headerSize);

    // Audio data header
    gsf::AudioDataPayload audioHeader {};
    audioHeader.timestampUs = static_cast<uint64_t>(
        juce::Time::getHighResolutionTicks() * 1000000
        / juce::Time::getHighResolutionTicksPerSecond());
    audioHeader.numSamples  = static_cast<uint16_t>(toRead);
    audioHeader.numChannels = numChannels;
    audioHeader.activePreset = gsf::PresetID::Flat; // Will be overridden per-client if needed

    std::memcpy(packet.data() + headerSize, &audioHeader, payloadHeaderSize);

    // Convert float to 24-bit PCM (interleaved L R L R ...)
    uint8_t* pcmData = packet.data() + headerSize + payloadHeaderSize;
    for (int i = 0; i < toRead; ++i)
    {
        gsf::floatTo24bit(tempL[i], pcmData + (i * 2) * 3);
        gsf::floatTo24bit(tempR[i], pcmData + (i * 2 + 1) * 3);
    }

    // Send to all connected clients
    std::lock_guard<std::mutex> lock(clientMutex);
    for (auto& client : clients)
    {
        if (client && client->isConnected())
        {
            // Update preset in audio header for this client
            auto* audioHdr = reinterpret_cast<gsf::AudioDataPayload*>(
                packet.data() + headerSize);
            audioHdr->activePreset = client->getPreset();

            client->sendAudioPacket(packet.data(), totalSize);
        }
    }
}

void StreamServer::sendHeartbeats()
{
    gsf::PacketHeader header {};
    header.magic       = gsf::kProtocolMagic;
    header.version     = gsf::kProtocolVersion;
    header.type        = gsf::PacketType::Heartbeat;
    header.payloadSize = sizeof(gsf::HeartbeatPayload);
    header.sequenceNum = sequenceNumber++;

    gsf::HeartbeatPayload heartbeat {};
    heartbeat.timestampUs = static_cast<uint64_t>(
        juce::Time::getHighResolutionTicks() * 1000000
        / juce::Time::getHighResolutionTicksPerSecond());
    heartbeat.clientCount = static_cast<uint8_t>(getClientCount());

    std::lock_guard<std::mutex> lock(clientMutex);
    for (auto& client : clients)
    {
        if (client && client->isConnected())
            client->sendControlPacket(header, &heartbeat, sizeof(heartbeat));
    }
}

void StreamServer::cleanupStaleClients()
{
    auto now = juce::Time::currentTimeMillis();

    std::lock_guard<std::mutex> lock(clientMutex);
    clients.erase(
        std::remove_if(clients.begin(), clients.end(),
            [now](const std::unique_ptr<ClientSession>& c)
            {
                if (!c) return true;
                if (!c->isConnected()) return true;
                if (c->isStale(now, kClientTimeoutMs))
                {
                    DBG("GSF FIGHTER: Removing stale client - " << c->getInfo().deviceName);
                    return true;
                }
                return false;
            }),
        clients.end());

    if (discovery)
        discovery->updateClientCount(static_cast<uint8_t>(clients.size()));
}
