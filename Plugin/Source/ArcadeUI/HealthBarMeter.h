/*
  ==============================================================================
   GSF FIGHTER - Health Bar Meter
   Audio level meter styled as a Street Fighter health/energy bar.
   Green -> Yellow -> Orange -> Red as level increases (like health depleting).
  ==============================================================================
*/

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "GSFLookAndFeel.h"
#include <atomic>

namespace gsf::ui
{

class HealthBarMeter : public juce::Component, public juce::Timer
{
public:
    HealthBarMeter(const juce::String& label = "P1", bool isLeftToRight = true);
    ~HealthBarMeter() override = default;

    void setLevel(float peak, float rms);
    void paint(juce::Graphics& g) override;
    void timerCallback() override;

    // Style
    void setBarColour(juce::Colour colour) { barColour = colour; }
    void setLabel(const juce::String& newLabel) { label = newLabel; repaint(); }

private:
    float currentPeak = 0.0f;
    float currentRms  = 0.0f;
    float displayPeak = 0.0f;
    float displayRms  = 0.0f;
    float peakHold    = 0.0f;
    int   peakHoldCounter = 0;

    juce::String label;
    juce::Colour barColour = Colours::Green;
    bool leftToRight;

    // Decay smoothing
    static constexpr float kDecayRate = 0.92f;
    static constexpr int   kPeakHoldFrames = 30;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HealthBarMeter)
};

} // namespace gsf::ui
