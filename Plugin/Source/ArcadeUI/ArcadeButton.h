/*
  ==============================================================================
   GSF FIGHTER - Arcade Button
   Big chunky button styled like a Street Fighter arcade pad button.
  ==============================================================================
*/

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "GSFLookAndFeel.h"

namespace gsf::ui
{

class ArcadeButton : public juce::TextButton
{
public:
    enum class Style
    {
        Hadouken,   // Red/orange glow - primary action
        Shoryuken,  // Blue glow - secondary action
        Tatsumaki,  // Yellow glow - tertiary
        KO,         // Red pulse - danger/disconnect
        Round       // Circular pad button
    };

    ArcadeButton(const juce::String& text, Style style = Style::Hadouken);
    ~ArcadeButton() override = default;

    void paintButton(juce::Graphics& g, bool isHighlighted, bool isDown) override;

    void setStyle(Style newStyle) { style = newStyle; repaint(); }
    void setPulsing(bool shouldPulse);

private:
    Style style;
    bool pulsing = false;
    float pulsePhase = 0.0f;

    juce::Colour getBaseColour() const;
    juce::Colour getGlowColour() const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ArcadeButton)
};

} // namespace gsf::ui
