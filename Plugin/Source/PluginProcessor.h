/*
  ==============================================================================
   GSF FIGHTER - Plugin Processor
   Master bus insert: captures audio and streams to mobile clients.
  ==============================================================================
*/

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include "Network/StreamServer.h"
#include "Presets/TranslationEngine.h"
#include "GSFProtocol.h"

class GSFFighterProcessor : public juce::AudioProcessor
{
public:
    GSFFighterProcessor();
    ~GSFFighterProcessor() override;

    // AudioProcessor interface
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    // --- GSF FIGHTER API ---
    StreamServer& getStreamServer() { return streamServer; }
    TranslationEngine& getTranslationEngine() { return translationEngine; }

    // Connection code (4-digit PIN)
    std::array<uint8_t, 4> getConnectionCode() const { return connectionCode; }
    void regenerateConnectionCode();

    // Metering (thread-safe, for UI)
    float getPeakLevel(int channel) const;
    float getRmsLevel(int channel) const;

    // Streaming state
    bool isStreaming() const { return streaming.load(); }
    void setStreaming(bool shouldStream);
    int getConnectedClientCount() const;

private:
    StreamServer        streamServer;
    TranslationEngine   translationEngine;

    std::array<uint8_t, 4> connectionCode;
    std::atomic<bool>   streaming { false };

    // Metering
    std::atomic<float>  peakL { 0.0f }, peakR { 0.0f };
    std::atomic<float>  rmsL  { 0.0f }, rmsR  { 0.0f };

    // Ring buffer for network thread
    juce::AbstractFifo  audioFifo { 65536 };
    std::vector<float>  fifoBufferL, fifoBufferR;

    double currentSampleRate = 48000.0;
    int    currentBlockSize  = 512;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GSFFighterProcessor)
};
