/*
  ==============================================================================
   GSF FIGHTER - Stream Server
   Manages UDP/TCP streaming to connected mobile clients.
  ==============================================================================
*/

#pragma once

#include <juce_core/juce_core.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include "ClientSession.h"
#include "DeviceDiscovery.h"
#include "GSFProtocol.h"
#include <vector>
#include <memory>
#include <mutex>

class StreamServer : public juce::Thread
{
public:
    StreamServer();
    ~StreamServer() override;

    // Configuration
    void configure(uint32_t sampleRate, uint16_t bitDepth, uint16_t numChannels, int samplesPerBlock);

    // Start/stop server
    void start(const std::array<uint8_t, 4>& connectionCode);
    void stop();
    bool isRunning() const { return running.load(); }

    // Push audio from processBlock (called from audio thread)
    void pushAudioBlock(const float* left, const float* right, int numSamples);

    // Client management
    int getClientCount() const;
    bool hasClients() const { return getClientCount() > 0; }
    std::vector<ClientInfo> getClientList() const;
    void disconnectClient(int clientIndex);
    void disconnectAll();

    // Preset management per client
    void setClientPreset(int clientIndex, gsf::PresetID preset);

private:
    void run() override; // Thread main loop

    void handleIncomingConnections();
    void sendAudioToClients();
    void sendHeartbeats();
    void cleanupStaleClients();

    // Network
    std::unique_ptr<juce::DatagramSocket> udpSocket;
    std::unique_ptr<juce::StreamingSocket> tcpServer;
    std::unique_ptr<DeviceDiscovery> discovery;

    // Client sessions
    mutable std::mutex clientMutex;
    std::vector<std::unique_ptr<ClientSession>> clients;

    // Audio config
    uint32_t sampleRate   = 48000;
    uint16_t bitDepth     = 24;
    uint16_t numChannels  = 2;
    int      samplesPerBlock = 512;

    // Connection code
    std::array<uint8_t, 4> connCode {};

    // Audio ring buffer (lock-free, audio thread -> network thread)
    juce::AbstractFifo audioFifo { 131072 };
    std::vector<float> fifoL, fifoR;

    // State
    std::atomic<bool> running { false };
    uint32_t sequenceNumber = 0;

    // Timing
    juce::int64 lastHeartbeatTime = 0;
    static constexpr int kHeartbeatIntervalMs = 2000;
    static constexpr int kClientTimeoutMs     = 10000;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StreamServer)
};
