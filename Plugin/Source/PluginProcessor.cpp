/*
  ==============================================================================
   GSF FIGHTER - Plugin Processor Implementation
  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

GSFFighterProcessor::GSFFighterProcessor()
    : AudioProcessor(BusesProperties()
                     .withInput("Input", juce::AudioChannelSet::stereo(), true)
                     .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{
    regenerateConnectionCode();
    fifoBufferL.resize(65536, 0.0f);
    fifoBufferR.resize(65536, 0.0f);
}

GSFFighterProcessor::~GSFFighterProcessor()
{
    streamServer.stop();
}

void GSFFighterProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    currentBlockSize  = samplesPerBlock;

    translationEngine.prepare(sampleRate, samplesPerBlock);
    streamServer.configure(static_cast<uint32_t>(sampleRate), 24, 2, samplesPerBlock);

    audioFifo.reset();
}

void GSFFighterProcessor::releaseResources()
{
    // Keep server running even when DAW stops playback
}

bool GSFFighterProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    // Only support stereo
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    if (layouts.getMainInputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    return true;
}

void GSFFighterProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    const int numSamples  = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();

    // Pass-through: the plugin must NOT alter the master bus audio
    // We only read the audio and stream it.

    if (numChannels < 2 || numSamples == 0)
        return;

    const float* dataL = buffer.getReadPointer(0);
    const float* dataR = buffer.getReadPointer(1);

    // --- Metering ---
    {
        float pL = 0.0f, pR = 0.0f;
        float sumL = 0.0f, sumR = 0.0f;

        for (int i = 0; i < numSamples; ++i)
        {
            float absL = std::abs(dataL[i]);
            float absR = std::abs(dataR[i]);
            if (absL > pL) pL = absL;
            if (absR > pR) pR = absR;
            sumL += dataL[i] * dataL[i];
            sumR += dataR[i] * dataR[i];
        }

        peakL.store(pL, std::memory_order_relaxed);
        peakR.store(pR, std::memory_order_relaxed);
        rmsL.store(std::sqrt(sumL / static_cast<float>(numSamples)), std::memory_order_relaxed);
        rmsR.store(std::sqrt(sumR / static_cast<float>(numSamples)), std::memory_order_relaxed);
    }

    // --- Stream audio to connected clients ---
    if (streaming.load(std::memory_order_relaxed) && streamServer.hasClients())
    {
        // Write into FIFO for network thread
        const auto scope = audioFifo.write(numSamples);

        if (scope.blockSize1 > 0)
        {
            std::memcpy(fifoBufferL.data() + scope.startIndex1, dataL, sizeof(float) * scope.blockSize1);
            std::memcpy(fifoBufferR.data() + scope.startIndex1, dataR, sizeof(float) * scope.blockSize1);
        }
        if (scope.blockSize2 > 0)
        {
            std::memcpy(fifoBufferL.data() + scope.startIndex2, dataL + scope.blockSize1, sizeof(float) * scope.blockSize2);
            std::memcpy(fifoBufferR.data() + scope.startIndex2, dataR + scope.blockSize1, sizeof(float) * scope.blockSize2);
        }

        // Send directly from the audio callback for minimum latency
        // The StreamServer handles packetization on its own thread
        streamServer.pushAudioBlock(dataL, dataR, numSamples);
    }
}

juce::AudioProcessorEditor* GSFFighterProcessor::createEditor()
{
    return new GSFFighterEditor(*this);
}

void GSFFighterProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    juce::ValueTree state("GSFFighter");
    state.setProperty("connectionCode",
        juce::String::charToString(static_cast<juce::juce_wchar>(connectionCode[0] + '0'))
      + juce::String::charToString(static_cast<juce::juce_wchar>(connectionCode[1] + '0'))
      + juce::String::charToString(static_cast<juce::juce_wchar>(connectionCode[2] + '0'))
      + juce::String::charToString(static_cast<juce::juce_wchar>(connectionCode[3] + '0')),
        nullptr);
    state.setProperty("streaming", streaming.load(), nullptr);

    juce::MemoryOutputStream stream(destData, true);
    state.writeToStream(stream);
}

void GSFFighterProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    auto state = juce::ValueTree::readFromData(data, static_cast<size_t>(sizeInBytes));
    if (state.isValid())
    {
        auto codeStr = state.getProperty("connectionCode").toString();
        if (codeStr.length() == 4)
        {
            for (int i = 0; i < 4; ++i)
                connectionCode[i] = static_cast<uint8_t>(codeStr[i] - '0');
        }
    }
}

void GSFFighterProcessor::regenerateConnectionCode()
{
    juce::Random rng;
    for (auto& digit : connectionCode)
        digit = static_cast<uint8_t>(rng.nextInt(10));
}

float GSFFighterProcessor::getPeakLevel(int channel) const
{
    return channel == 0 ? peakL.load(std::memory_order_relaxed)
                        : peakR.load(std::memory_order_relaxed);
}

float GSFFighterProcessor::getRmsLevel(int channel) const
{
    return channel == 0 ? rmsL.load(std::memory_order_relaxed)
                        : rmsR.load(std::memory_order_relaxed);
}

void GSFFighterProcessor::setStreaming(bool shouldStream)
{
    streaming.store(shouldStream, std::memory_order_relaxed);

    if (shouldStream)
        streamServer.start(connectionCode);
    else
        streamServer.stop();
}

int GSFFighterProcessor::getConnectedClientCount() const
{
    return streamServer.getClientCount();
}

// --- Plugin instantiation ---
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GSFFighterProcessor();
}
