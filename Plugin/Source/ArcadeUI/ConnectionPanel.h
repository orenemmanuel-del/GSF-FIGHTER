/*
  ==============================================================================
   GSF FIGHTER - Connection Panel
   Shows connection code, connected clients, and stream controls.
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

    // State updates
    void setConnectionCode(const std::array<uint8_t, 4>& code);
    void setIPAddress(const juce::String& ip);
    void setClientCount(int count);
    void setStreaming(bool streaming);
    void setLevels(float peakL, float peakR, float rmsL, float rmsR);

    // Callbacks
    std::function<void()> onStartStop;
    std::function<void()> onRegenCode;

private:
    std::array<uint8_t, 4> connectionCode {};
    juce::String ipAddress = "0.0.0.0";
    int clientCount = 0;
    bool isStreaming = false;

    ArcadeButton startStopButton { "FIGHT!", ArcadeButton::Style::Hadouken };
    ArcadeButton regenButton     { "NEW CODE", ArcadeButton::Style::Shoryuken };

    HealthBarMeter meterL { "L", true };
    HealthBarMeter meterR { "R", false };

    // Animation
    float fightGlowPhase = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConnectionPanel)
};

} // namespace gsf::ui
