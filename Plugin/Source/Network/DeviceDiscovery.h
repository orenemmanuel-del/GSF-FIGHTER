/*
  ==============================================================================
   GSF FIGHTER - Device Discovery
   Broadcasts plugin availability via UDP for mobile clients to find.
  ==============================================================================
*/

#pragma once

#include <juce_core/juce_core.h>
#include "GSFProtocol.h"
#include <array>

class DeviceDiscovery : public juce::Thread
{
public:
    DeviceDiscovery();
    ~DeviceDiscovery() override;

    void startBroadcasting(const std::array<uint8_t, 4>& connectionCode,
                           uint32_t sampleRate,
                           uint16_t bitDepth,
                           uint16_t numChannels,
                           uint8_t currentClients,
                           uint8_t maxClients);
    void stopBroadcasting();

    void updateClientCount(uint8_t count);

    // Get the local IP address for display
    static juce::String getLocalIPAddress();

private:
    void run() override;

    std::unique_ptr<juce::DatagramSocket> broadcastSocket;
    gsf::DiscoveryPayload payload {};
    std::atomic<bool> broadcasting { false };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeviceDiscovery)
};
