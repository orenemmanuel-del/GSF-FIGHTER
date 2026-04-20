/*
  ==============================================================================
   GSF FIGHTER — Input Matrix Panel (Oscilloscope Design)
   Main content area: device status, stream quality toggle, VU meters, stats.
  ==============================================================================
*/

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "GSFLookAndFeel.h"
#include "ArcadeButton.h"
#include "HealthBarMeter.h"

namespace gsf::ui
{

class ConnectionPanel : public juce::Component, public juce::Timer
{
public:
    ConnectionPanel();
    ~ConnectionPanel() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;
    void timerCallback() override;

    void setConnectionCode(const std::array<uint8_t, 4>& code);
    void setIPAddress(const juce::String& ip);
    void setClientCount(int count);
    void setStreaming(bool streaming);
    void setLevels(float peakL, float peakR, float rmsL, float rmsR);

    std::function<void()> onStartStop;
    std::function<void()> onRegenCode;

private:
    std::array<uint8_t, 4> connectionCode {};
    juce::String ipAddress = "0.0.0.0";
    int clientCount = 0;
    bool isStreaming = false;
    float pulsePhase = 0.0f;

    ArcadeButton startStopButton { "START STREAM", ArcadeButton::Style::Hadouken };
    ArcadeButton qualityStandard { "48K / 24-BIT", ArcadeButton::Style::Shoryuken };
    ArcadeButton qualityHiRes    { "96K / HI-RES", ArcadeButton::Style::Shoryuken };

    HealthBarMeter meterL { "L", true };
    HealthBarMeter meterR { "R", true };

    // Layout rectangles cached in resized()
    juce::Rectangle<int> statusArea;
    juce::Rectangle<int> qualityArea;
    juce::Rectangle<int> metersArea;
    juce::Rectangle<int> statsArea;

    void drawStatusButton(juce::Graphics& g, juce::Rectangle<int> area);
    void drawStatsRow(juce::Graphics& g, juce::Rectangle<int> area);
    void drawSectionLabel(juce::Graphics& g, juce::Rectangle<int> area,
                          const juce::String& label);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConnectionPanel)
};

} // namespace gsf::ui
