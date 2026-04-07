/*
  ==============================================================================
   GSF FIGHTER - Translation Engine Implementation
   6 presets by Gesaffelstein simulating real-world playback systems.
  ==============================================================================
*/

#include "TranslationEngine.h"

// ---------------------------------------------------------------------------
// Preset definitions — each one shapes the mix to reveal how it translates
// on a specific playback system.
// ---------------------------------------------------------------------------
const std::array<TranslationPreset, 7> TranslationEngine::presets = {{
    // 0: FLAT — no processing
    {
        "FLAT (NO FILTER)", gsf::PresetID::Flat,
        {{
            { 100.0f,  0.0f, 0.707f, TranslationPreset::BandParams::LowShelf },
            { 400.0f,  0.0f, 1.0f,   TranslationPreset::BandParams::Peak },
            { 1000.0f, 0.0f, 1.0f,   TranslationPreset::BandParams::Peak },
            { 4000.0f, 0.0f, 1.0f,   TranslationPreset::BandParams::Peak },
            { 10000.0f,0.0f, 0.707f, TranslationPreset::BandParams::HighShelf },
        }},
        1.0f, 0.0f, 0.0f, 20000.0f
    },

    // 1: iPhone Speaker — tiny speaker, no sub, harsh mids, limited stereo
    {
        "GSF iPHONE FIGHTER", gsf::PresetID::iPhoneSpeaker,
        {{
            { 80.0f,  -18.0f, 0.5f,  TranslationPreset::BandParams::LowShelf },  // Kill sub bass
            { 350.0f,  -3.0f, 1.2f,  TranslationPreset::BandParams::Peak },       // Boxy reduction
            { 1200.0f,  4.0f, 0.8f,  TranslationPreset::BandParams::Peak },       // Midrange push (phone resonance)
            { 3500.0f,  5.0f, 1.5f,  TranslationPreset::BandParams::Peak },       // Presence / harshness
            { 12000.0f,-6.0f, 0.707f,TranslationPreset::BandParams::HighShelf },  // Roll off air
        }},
        0.3f,    // Very narrow stereo
        0.15f,   // Slight saturation (speaker distortion)
        200.0f,  // High-pass at 200Hz
        14000.0f // Low-pass at 14kHz
    },

    // 2: AirPods Pro — good bass extension, ANC characteristics, intimate
    {
        "GSF AIRPODS FIGHTER", gsf::PresetID::AirPodsPro,
        {{
            { 60.0f,   3.0f, 0.707f, TranslationPreset::BandParams::LowShelf },   // Slight bass boost (ANC seal)
            { 250.0f, -1.5f, 1.0f,   TranslationPreset::BandParams::Peak },       // Clean up low mids
            { 1000.0f, 0.0f, 1.0f,   TranslationPreset::BandParams::Peak },       // Flat mids
            { 5000.0f, 2.0f, 1.2f,   TranslationPreset::BandParams::Peak },       // Presence boost
            { 12000.0f,1.5f, 0.707f, TranslationPreset::BandParams::HighShelf },  // Air
        }},
        1.1f,    // Slightly enhanced stereo (in-ear)
        0.0f,    // No saturation
        20.0f,   // Full range
        19000.0f
    },

    // 3: Voiture (Car) — subwoofer emphasis, road noise masking, cabin resonance
    {
        "GSF VOITURE FIGHTER", gsf::PresetID::Voiture,
        {{
            { 50.0f,   6.0f, 0.5f,  TranslationPreset::BandParams::LowShelf },   // Sub boost (car sub)
            { 200.0f,  3.0f, 0.8f,  TranslationPreset::BandParams::Peak },       // Cabin resonance
            { 800.0f, -2.0f, 1.0f,  TranslationPreset::BandParams::Peak },       // Road noise masking dip
            { 3000.0f, 2.0f, 1.2f,  TranslationPreset::BandParams::Peak },       // Vocal clarity boost
            { 10000.0f,-4.0f,0.707f,TranslationPreset::BandParams::HighShelf },  // Rolled off highs (cabin absorption)
        }},
        0.7f,    // Reduced stereo (car cabin)
        0.05f,   // Tiny saturation
        25.0f,
        16000.0f
    },

    // 4: Club System — massive sub, loud, compressed, PA curve
    {
        "GSF CLUB FIGHTER", gsf::PresetID::ClubSystem,
        {{
            { 40.0f,   8.0f, 0.4f,  TranslationPreset::BandParams::LowShelf },   // Sub pressure
            { 120.0f,  4.0f, 0.7f,  TranslationPreset::BandParams::Peak },       // Kick/bass weight
            { 500.0f, -3.0f, 1.0f,  TranslationPreset::BandParams::Peak },       // Scoop mids (club curve)
            { 2500.0f, 2.0f, 1.5f,  TranslationPreset::BandParams::Peak },       // Vocal cut-through
            { 8000.0f, -2.0f,0.707f,TranslationPreset::BandParams::HighShelf },  // Tamed highs (PA protection)
        }},
        1.3f,    // Wide stereo (big room)
        0.2f,    // PA saturation
        20.0f,
        18000.0f
    },

    // 5: Cheap Earbuds — thin, no bass, harsh highs, poor isolation
    {
        "GSF CHEAP FIGHTER", gsf::PresetID::CheapEarbuds,
        {{
            { 100.0f, -12.0f, 0.5f,  TranslationPreset::BandParams::LowShelf },  // No bass
            { 400.0f,  -2.0f, 1.0f,   TranslationPreset::BandParams::Peak },      // Thin low mids
            { 1500.0f,  3.0f, 0.8f,   TranslationPreset::BandParams::Peak },      // Nasal mids boost
            { 4000.0f,  6.0f, 1.5f,   TranslationPreset::BandParams::Peak },      // Harsh presence
            { 10000.0f, 4.0f, 0.707f, TranslationPreset::BandParams::HighShelf }, // Sizzly highs
        }},
        0.8f,    // Reduced stereo (poor drivers)
        0.1f,    // Slight distortion
        120.0f,  // High-pass (no extension)
        17000.0f
    },

    // 6: Studio Monitors — flat reference with slight room interaction
    {
        "GSF STUDIO FIGHTER", gsf::PresetID::StudioMonitors,
        {{
            { 50.0f,   0.5f, 0.707f, TranslationPreset::BandParams::LowShelf },  // Slight low end room gain
            { 300.0f, -1.0f, 1.5f,   TranslationPreset::BandParams::Peak },      // Desk reflection dip
            { 1000.0f, 0.0f, 1.0f,   TranslationPreset::BandParams::Peak },      // Flat
            { 4000.0f, 0.5f, 1.0f,   TranslationPreset::BandParams::Peak },      // Slight presence
            { 14000.0f,0.0f, 0.707f, TranslationPreset::BandParams::HighShelf },  // Neutral air
        }},
        1.0f,    // Normal stereo
        0.0f,    // No saturation
        30.0f,
        20000.0f
    },
}};

TranslationEngine::TranslationEngine()
{
}

void TranslationEngine::prepare(double sr, int blockSz)
{
    sampleRate = sr;
    blockSize  = blockSz;

    for (auto& ch : channelFilters)
    {
        for (auto& band : ch.eqBands)
            band.reset();
        ch.lowCut.reset();
        ch.highCut.reset();
    }

    rebuildFilters();
}

void TranslationEngine::setPreset(gsf::PresetID preset)
{
    if (preset == currentPresetId)
        return;

    currentPresetId = preset;
    rebuildFilters();
}

const TranslationPreset& TranslationEngine::getPresetData(gsf::PresetID id) const
{
    auto idx = static_cast<int>(id);
    if (idx < 0 || idx >= static_cast<int>(presets.size()))
        return presets[0];
    return presets[idx];
}

void TranslationEngine::rebuildFilters()
{
    const auto& preset = getPresetData(currentPresetId);

    for (int ch = 0; ch < 2; ++ch)
    {
        auto& filters = channelFilters[ch];

        for (int b = 0; b < 5; ++b)
        {
            const auto& band = preset.bands[b];

            juce::ReferenceCountedObjectPtr<IIRCoeffs> coeffs;

            switch (band.type)
            {
                case TranslationPreset::BandParams::LowShelf:
                    coeffs = IIRCoeffs::makeLowShelf(sampleRate, band.frequency, band.q,
                                juce::Decibels::decibelsToGain(band.gainDb));
                    break;
                case TranslationPreset::BandParams::HighShelf:
                    coeffs = IIRCoeffs::makeHighShelf(sampleRate, band.frequency, band.q,
                                juce::Decibels::decibelsToGain(band.gainDb));
                    break;
                case TranslationPreset::BandParams::Peak:
                default:
                    coeffs = IIRCoeffs::makePeakFilter(sampleRate, band.frequency, band.q,
                                juce::Decibels::decibelsToGain(band.gainDb));
                    break;
            }

            filters.eqBands[b].coefficients = coeffs;
        }

        // Low cut (high-pass)
        if (preset.lowCutHz > 10.0f)
        {
            auto hpCoeffs = IIRCoeffs::makeHighPass(sampleRate, preset.lowCutHz, 0.707f);
            filters.lowCut.coefficients = hpCoeffs;
        }
        else
        {
            // Bypass: set to allpass-like
            auto bypassCoeffs = IIRCoeffs::makeAllPass(sampleRate, 1000.0f, 0.707f);
            filters.lowCut.coefficients = bypassCoeffs;
        }

        // High cut (low-pass)
        if (preset.highCutHz < 19500.0f)
        {
            auto lpCoeffs = IIRCoeffs::makeLowPass(sampleRate, preset.highCutHz, 0.707f);
            filters.highCut.coefficients = lpCoeffs;
        }
        else
        {
            auto bypassCoeffs = IIRCoeffs::makeAllPass(sampleRate, 1000.0f, 0.707f);
            filters.highCut.coefficients = bypassCoeffs;
        }
    }
}

void TranslationEngine::process(juce::AudioBuffer<float>& buffer)
{
    if (currentPresetId == gsf::PresetID::Flat)
        return; // No processing

    const auto& preset = getPresetData(currentPresetId);
    const int numSamples = buffer.getNumSamples();

    for (int ch = 0; ch < juce::jmin(buffer.getNumChannels(), 2); ++ch)
    {
        float* data = buffer.getWritePointer(ch);
        auto& filters = channelFilters[ch];

        // Apply 5-band EQ
        for (auto& band : filters.eqBands)
        {
            for (int i = 0; i < numSamples; ++i)
                data[i] = band.processSample(data[i]);
        }

        // Apply low cut
        for (int i = 0; i < numSamples; ++i)
            data[i] = filters.lowCut.processSample(data[i]);

        // Apply high cut
        for (int i = 0; i < numSamples; ++i)
            data[i] = filters.highCut.processSample(data[i]);

        // Apply saturation
        if (preset.saturationAmount > 0.001f)
        {
            const float sat = preset.saturationAmount;
            for (int i = 0; i < numSamples; ++i)
            {
                float x = data[i];
                // Soft clipping via tanh approximation
                x = std::tanh(x * (1.0f + sat * 3.0f)) / (1.0f + sat * 2.0f);
                data[i] = x;
            }
        }
    }

    // Apply stereo width
    if (std::abs(preset.stereoWidth - 1.0f) > 0.01f && buffer.getNumChannels() >= 2)
    {
        float* dataL = buffer.getWritePointer(0);
        float* dataR = buffer.getWritePointer(1);
        const float width = preset.stereoWidth;

        for (int i = 0; i < numSamples; ++i)
        {
            float mid  = (dataL[i] + dataR[i]) * 0.5f;
            float side = (dataL[i] - dataR[i]) * 0.5f;
            side *= width;
            dataL[i] = mid + side;
            dataR[i] = mid - side;
        }
    }
}

void TranslationEngine::processForPreset(const float* inputL, const float* inputR,
                                          float* outputL, float* outputR,
                                          int numSamples, gsf::PresetID preset)
{
    // Copy input to output
    std::memcpy(outputL, inputL, sizeof(float) * numSamples);
    std::memcpy(outputR, inputR, sizeof(float) * numSamples);

    if (preset == gsf::PresetID::Flat)
        return;

    // Create temp buffer and process
    juce::AudioBuffer<float> tempBuffer(2, numSamples);
    tempBuffer.copyFrom(0, 0, outputL, numSamples);
    tempBuffer.copyFrom(1, 0, outputR, numSamples);

    auto savedPreset = currentPresetId;
    setPreset(preset);
    process(tempBuffer);
    setPreset(savedPreset);

    std::memcpy(outputL, tempBuffer.getReadPointer(0), sizeof(float) * numSamples);
    std::memcpy(outputR, tempBuffer.getReadPointer(1), sizeof(float) * numSamples);
}
