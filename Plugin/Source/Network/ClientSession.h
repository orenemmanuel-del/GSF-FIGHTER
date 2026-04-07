/*
  ==============================================================================
   GSF FIGHTER - Client Session
   Represents a single connected mobile client.
  ==============================================================================
*/

#pragma once

#include <juce_core/juce_core.h>
#include "GSFProtocol.h"
#include <memory>
#include <atomic>

struct ClientInfo
{
    juce::String    deviceName;
    juce::String    ipAddress;
    int             port = 0;
    gsf::PresetID   activePreset = gsf::PresetID::Flat;
    bool            connected    = false;
    juce::int64     lastHeartbeat = 0;
};

class ClientSession
{
public:
    ClientSession(const juce::String& deviceName,
                  const juce::String& ipAddress,
                  int port,
                  std::unique_ptr<juce::StreamingSocket> socket);
    ~ClientSession();

    // Send audio data to this client
    bool sendAudioPacket(const uint8_t* packetData, int packetSize);

    // Send control packet
    bool sendControlPacket(const gsf::PacketHeader& header, const void* payload, int payloadSize);

    // Heartbeat
    void updateHeartbeat();
    bool isStale(juce::int64 currentTimeMs, int timeoutMs) const;

    // Info
    const ClientInfo& getInfo() const { return info; }
    void setPreset(gsf::PresetID preset) { info.activePreset = preset; }
    gsf::PresetID getPreset() const { return info.activePreset; }

    bool isConnected() const { return info.connected; }
    void disconnect();

private:
    ClientInfo info;
    std::unique_ptr<juce::StreamingSocket> tcpSocket;
    std::mutex sendMutex;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ClientSession)
};
