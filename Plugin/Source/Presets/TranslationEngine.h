/*
  ==============================================================================
   GSF FIGHTER - Translation Engine
   Applies EQ/acoustic simulation for each "Fighter" preset.
   These presets simulate how a mix translates on different playback systems.
  ==============================================================================
*/

#pragma once

#include <juce_dsp/juce_dsp.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include "GSFProtocol.h"
#include <array>

struct TranslationPreset
{
    const char* name;
    gsf::PresetID id;

    // 5-band parametric EQ: Low Shelf, Low-Mid, Mid, High-Mid, High Shelf
    struct BandParams
    {
        float frequency;    // Hz
        float gainDb;       // dB
        float q;            // Q factor
        enum Type { LowShelf, Peak, HighShelf } type;
    };

    std::array<BandParams, 5> bands;

    // Additional simulation params
    float stereoWidth;      // 0.0 = mono, 1.0 = normal, 2.0 = wide
    float saturationAmount; // 0.0 = none, 1.0 = heavy
    float lowCutHz;         // High-pass filter frequency (0 = off)
    float highCutHz;        // Low-pass filter frequency (20000 = off)
};

class TranslationEngine
{
public:
    TranslationEngine();

    void prepare(double sampleRate, int samplesPerBlock);
    void process(juce::AudioBuffer<float>& buffer);
    void setPreset(gsf::PresetID preset);
    gsf::PresetID getCurrentPreset() const { return currentPresetId; }
    const TranslationPreset& getPresetData(gsf::PresetID id) const;

    // Process a block into a separate output buffer (for per-client preset processing)
    void processForPreset(const float* inputL, const float* inputR,
                          float* outputL, float* outputR,
                          int numSamples, gsf::PresetID preset);

private:
    void rebuildFilters();

    gsf::PresetID currentPresetId = gsf::PresetID::Flat;
    double sampleRate = 48000.0;
    int blockSize = 512;

    // 5-band EQ per channel
    using IIRFilter = juce::dsp::IIR::Filter<float>;
    using IIRCoeffs = juce::dsp::IIR::Coefficients<float>;

    // Per-channel filter chains
    struct ChannelFilters
    {
        std::array<IIRFilter, 5> eqBands;
        IIRFilter lowCut;
        IIRFilter highCut;
    };

    std::array<ChannelFilters, 2> channelFilters; // L, R

    // Static preset definitions
    static const std::array<TranslationPreset, 7> presets;
};
