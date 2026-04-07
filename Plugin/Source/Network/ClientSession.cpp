/*
  ==============================================================================
   GSF FIGHTER - Client Session Implementation
  ==============================================================================
*/

#include "ClientSession.h"

ClientSession::ClientSession(const juce::String& deviceName,
                             const juce::String& ipAddress,
                             int port,
                             std::unique_ptr<juce::StreamingSocket> socket)
    : tcpSocket(std::move(socket))
{
    info.deviceName    = deviceName;
    info.ipAddress     = ipAddress;
    info.port          = port;
    info.connected     = true;
    info.activePreset  = gsf::PresetID::Flat;
    info.lastHeartbeat = juce::Time::currentTimeMillis();
}

ClientSession::~ClientSession()
{
    disconnect();
}

bool ClientSession::sendAudioPacket(const uint8_t* packetData, int packetSize)
{
    if (!info.connected || tcpSocket == nullptr)
        return false;

    std::lock_guard<std::mutex> lock(sendMutex);

    int bytesSent = tcpSocket->write(packetData, packetSize);
    if (bytesSent < 0)
    {
        info.connected = false;
        return false;
    }

    return bytesSent == packetSize;
}

bool ClientSession::sendControlPacket(const gsf::PacketHeader& header, const void* payload, int payloadSize)
{
    if (!info.connected || tcpSocket == nullptr)
        return false;

    std::lock_guard<std::mutex> lock(sendMutex);

    // Send header
    int sent = tcpSocket->write(&header, sizeof(header));
    if (sent != sizeof(header))
    {
        info.connected = false;
        return false;
    }

    // Send payload
    if (payload != nullptr && payloadSize > 0)
    {
        sent = tcpSocket->write(payload, payloadSize);
        if (sent != payloadSize)
        {
            info.connected = false;
            return false;
        }
    }

    return true;
}

void ClientSession::updateHeartbeat()
{
    info.lastHeartbeat = juce::Time::currentTimeMillis();
}

bool ClientSession::isStale(juce::int64 currentTimeMs, int timeoutMs) const
{
    return (currentTimeMs - info.lastHeartbeat) > timeoutMs;
}

void ClientSession::disconnect()
{
    info.connected = false;
    if (tcpSocket != nullptr)
    {
        // Send disconnect packet
        gsf::PacketHeader header {};
        header.magic       = gsf::kProtocolMagic;
        header.version     = gsf::kProtocolVersion;
        header.type        = gsf::PacketType::Disconnect;
        header.payloadSize = 0;
        header.sequenceNum = 0;

        std::lock_guard<std::mutex> lock(sendMutex);
        tcpSocket->write(&header, sizeof(header));
        tcpSocket->close();
    }
}
