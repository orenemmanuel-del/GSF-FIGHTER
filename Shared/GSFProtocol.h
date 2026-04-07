/*
  ==============================================================================
   GSF FIGHTER - Shared Network Protocol
   Streaming protocol definition for plugin <-> mobile communication.
  ==============================================================================
*/

#pragma once

#include <cstdint>
#include <cstring>
#include <array>

namespace gsf
{

// -------------------------------------------------------------------
// Protocol constants
// -------------------------------------------------------------------
static constexpr uint16_t kDiscoveryPort    = 47700;
static constexpr uint16_t kStreamPort       = 47701;
static constexpr uint16_t kControlPort      = 47702;
static constexpr uint32_t kProtocolMagic    = 0x47534646; // "GSFF"
static constexpr uint8_t  kProtocolVersion  = 1;
static constexpr int      kMaxClients       = 8;
static constexpr int      kAudioBufferSize  = 512;
static constexpr int      kJitterBufferMs   = 30;
static constexpr int      kDiscoveryIntervalMs = 1000;

// -------------------------------------------------------------------
// Packet types
// -------------------------------------------------------------------
enum class PacketType : uint8_t
{
    Discovery       = 0x01,   // Plugin broadcasts availability
    ConnectRequest  = 0x02,   // Mobile requests connection
    ConnectAccept   = 0x03,   // Plugin accepts connection
    ConnectReject   = 0x04,   // Plugin rejects connection
    AudioData       = 0x10,   // Audio stream packet
    AudioConfig     = 0x11,   // Sample rate / bit depth config
    PresetChange    = 0x20,   // Translation preset change
    PresetSync      = 0x21,   // Current preset state
    Heartbeat       = 0x30,   // Keep-alive
    Disconnect      = 0xFF    // Graceful disconnect
};

// -------------------------------------------------------------------
// Translation preset IDs
// -------------------------------------------------------------------
enum class PresetID : uint8_t
{
    Flat            = 0,
    iPhoneSpeaker   = 1,
    AirPodsPro      = 2,
    Voiture         = 3,   // Car system
    ClubSystem      = 4,
    CheapEarbuds    = 5,
    StudioMonitors  = 6,
    Count           = 7
};

inline const char* presetName(PresetID id)
{
    switch (id)
    {
        case PresetID::Flat:            return "FLAT (NO FILTER)";
        case PresetID::iPhoneSpeaker:   return "GSF iPHONE FIGHTER";
        case PresetID::AirPodsPro:      return "GSF AIRPODS FIGHTER";
        case PresetID::Voiture:         return "GSF VOITURE FIGHTER";
        case PresetID::ClubSystem:      return "GSF CLUB FIGHTER";
        case PresetID::CheapEarbuds:    return "GSF CHEAP FIGHTER";
        case PresetID::StudioMonitors:  return "GSF STUDIO FIGHTER";
        default:                        return "UNKNOWN";
    }
}

// -------------------------------------------------------------------
// Packet header (12 bytes, network byte order)
// -------------------------------------------------------------------
#pragma pack(push, 1)
struct PacketHeader
{
    uint32_t    magic;          // kProtocolMagic
    uint8_t     version;        // kProtocolVersion
    PacketType  type;           // Packet type
    uint16_t    payloadSize;    // Size of payload after header
    uint32_t    sequenceNum;    // Monotonic sequence number
};

struct DiscoveryPayload
{
    char        hostName[64];       // DAW host name
    char        dawName[32];        // e.g. "Logic Pro X", "Pro Tools"
    uint32_t    sampleRate;
    uint16_t    bitDepth;
    uint16_t    numChannels;
    uint8_t     connectionCode[4];  // 4-digit PIN
    uint8_t     currentClients;
    uint8_t     maxClients;
};

struct ConnectRequestPayload
{
    char        deviceName[64];     // e.g. "iPhone 15 Pro"
    uint8_t     connectionCode[4];  // Must match PIN
    PresetID    requestedPreset;
};

struct AudioConfigPayload
{
    uint32_t    sampleRate;
    uint16_t    bitDepth;
    uint16_t    numChannels;
    uint16_t    samplesPerBlock;
    uint16_t    reserved;
};

struct AudioDataPayload
{
    uint64_t    timestampUs;        // Microsecond timestamp
    uint16_t    numSamples;
    uint16_t    numChannels;
    PresetID    activePreset;
    uint8_t     reserved[3];
    // Followed by interleaved 24-bit PCM samples
    // Size = numSamples * numChannels * 3 bytes
};

struct PresetChangePayload
{
    PresetID    preset;
};

struct HeartbeatPayload
{
    uint64_t    timestampUs;
    uint8_t     clientCount;
};
#pragma pack(pop)

// -------------------------------------------------------------------
// Utility: pack 24-bit sample from float
// -------------------------------------------------------------------
inline void floatTo24bit(float sample, uint8_t* dest)
{
    // Clamp to [-1, 1]
    if (sample > 1.0f) sample = 1.0f;
    if (sample < -1.0f) sample = -1.0f;

    int32_t intSample = static_cast<int32_t>(sample * 8388607.0f); // 2^23 - 1
    dest[0] = static_cast<uint8_t>((intSample >> 0)  & 0xFF);
    dest[1] = static_cast<uint8_t>((intSample >> 8)  & 0xFF);
    dest[2] = static_cast<uint8_t>((intSample >> 16) & 0xFF);
}

inline float float_from24bit(const uint8_t* src)
{
    int32_t intSample = static_cast<int32_t>(src[0])
                      | (static_cast<int32_t>(src[1]) << 8)
                      | (static_cast<int32_t>(src[2]) << 16);
    // Sign extend from 24-bit
    if (intSample & 0x800000)
        intSample |= 0xFF000000;
    return static_cast<float>(intSample) / 8388607.0f;
}

} // namespace gsf
