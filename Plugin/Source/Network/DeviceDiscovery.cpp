/*
  ==============================================================================
   GSF FIGHTER - Device Discovery Implementation
  ==============================================================================
*/

#include "DeviceDiscovery.h"

DeviceDiscovery::DeviceDiscovery()
    : Thread("GSF Discovery")
{
}

DeviceDiscovery::~DeviceDiscovery()
{
    stopBroadcasting();
}

void DeviceDiscovery::startBroadcasting(const std::array<uint8_t, 4>& connectionCode,
                                         uint32_t sampleRate,
                                         uint16_t bitDepth,
                                         uint16_t numChannels,
                                         uint8_t currentClients,
                                         uint8_t maxClients)
{
    // Fill payload
    std::memset(&payload, 0, sizeof(payload));

    auto hostName = juce::SystemStats::getComputerName();
    hostName.copyToUTF8(payload.hostName, sizeof(payload.hostName) - 1);

    // Try to detect DAW name
    auto dawName = juce::JUCEApplication::isStandaloneApp()
                       ? juce::String("Standalone")
                       : juce::String("DAW");

    // Check for known host process names
    auto processName = juce::File::getSpecialLocation(
        juce::File::currentExecutableFile).getFileNameWithoutExtension();

    if (processName.containsIgnoreCase("Logic"))
        dawName = "Logic Pro";
    else if (processName.containsIgnoreCase("Pro Tools"))
        dawName = "Pro Tools";
    else if (processName.containsIgnoreCase("Ableton"))
        dawName = "Ableton Live";
    else if (processName.containsIgnoreCase("FL"))
        dawName = "FL Studio";
    else if (processName.containsIgnoreCase("Cubase"))
        dawName = "Cubase";
    else if (processName.containsIgnoreCase("Studio One"))
        dawName = "Studio One";
    else if (processName.containsIgnoreCase("Reaper"))
        dawName = "REAPER";
    else if (processName.containsIgnoreCase("Bitwig"))
        dawName = "Bitwig Studio";

    dawName.copyToUTF8(payload.dawName, sizeof(payload.dawName) - 1);

    payload.sampleRate     = sampleRate;
    payload.bitDepth       = bitDepth;
    payload.numChannels    = numChannels;
    payload.currentClients = currentClients;
    payload.maxClients     = maxClients;
    std::memcpy(payload.connectionCode, connectionCode.data(), 4);

    broadcasting.store(true);
    startThread(juce::Thread::Priority::low);
}

void DeviceDiscovery::stopBroadcasting()
{
    broadcasting.store(false);
    stopThread(2000);

    if (broadcastSocket)
    {
        broadcastSocket->shutdown();
        broadcastSocket.reset();
    }
}

void DeviceDiscovery::updateClientCount(uint8_t count)
{
    payload.currentClients = count;
}

juce::String DeviceDiscovery::getLocalIPAddress()
{
    auto addresses = juce::IPAddress::getAllAddresses(false); // IPv4 only

    for (const auto& addr : addresses)
    {
        auto str = addr.toString();
        if (str.startsWith("192.168.") || str.startsWith("10.") || str.startsWith("172."))
            return str;
    }

    return "127.0.0.1";
}

void DeviceDiscovery::run()
{
    broadcastSocket = std::make_unique<juce::DatagramSocket>(false);
    broadcastSocket->bindToPort(0); // Bind to any available port
    broadcastSocket->setEnablePortReuse(true);

    // Build discovery packet
    gsf::PacketHeader header {};
    header.magic       = gsf::kProtocolMagic;
    header.version     = gsf::kProtocolVersion;
    header.type        = gsf::PacketType::Discovery;
    header.payloadSize = sizeof(gsf::DiscoveryPayload);
    header.sequenceNum = 0;

    std::vector<uint8_t> packet(sizeof(header) + sizeof(payload));

    while (!threadShouldExit() && broadcasting.load())
    {
        // Update sequence number
        header.sequenceNum++;

        // Assemble packet
        std::memcpy(packet.data(), &header, sizeof(header));
        std::memcpy(packet.data() + sizeof(header), &payload, sizeof(payload));

        // Broadcast on subnet
        broadcastSocket->write("255.255.255.255", gsf::kDiscoveryPort,
                               packet.data(), static_cast<int>(packet.size()));

        // Also broadcast on common subnets
        broadcastSocket->write("192.168.1.255", gsf::kDiscoveryPort,
                               packet.data(), static_cast<int>(packet.size()));
        broadcastSocket->write("192.168.0.255", gsf::kDiscoveryPort,
                               packet.data(), static_cast<int>(packet.size()));
        broadcastSocket->write("10.0.0.255", gsf::kDiscoveryPort,
                               packet.data(), static_cast<int>(packet.size()));

        // Wait before next broadcast
        wait(gsf::kDiscoveryIntervalMs);
    }
}
