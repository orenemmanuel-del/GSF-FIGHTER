/*
  ==============================================================================
   GSF FIGHTER - Fight Screen
   "READY?" -> "FIGHT!" overlay animation when streaming starts.
  ==============================================================================
*/

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "GSFLookAndFeel.h"

namespace gsf::ui
{

class FightScreen : public juce::Component, public juce::Timer
{
public:
    FightScreen();
    ~FightScreen() override = default;

    void triggerFightSequence();
    void triggerKOSequence();

    void paint(juce::Graphics& g) override;
    void timerCallback() override;

    bool isAnimating() const { return animating; }

private:
    enum class Phase
    {
        Idle,
        Ready,
        Fight,
        KO,
        FadeOut
    };

    Phase phase = Phase::Idle;
    bool animating = false;
    int frameCount = 0;
    float textScale = 1.0f;
    float alpha = 0.0f;

    // Timing (in frames at 60fps)
    static constexpr int kReadyDuration = 60;  // 1 second
    static constexpr int kFightDuration = 45;  // 0.75 seconds
    static constexpr int kKODuration    = 90;  // 1.5 seconds
    static constexpr int kFadeOutDuration = 20; // 0.33 seconds

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FightScreen)
};

} // namespace gsf::ui
