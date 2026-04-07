/*
  ==============================================================================
   GSF FIGHTER - Arcade Look & Feel
   Street Fighter inspired dark arcade aesthetic.
   Palette: blood red, electric blue, neon yellow, matte black (Gesaffelstein).
  ==============================================================================
*/

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace gsf::ui
{

// Colour palette
namespace Colours
{
    const juce::Colour Black       { 0xFF0A0A0A };  // Gesaffelstein matte black
    const juce::Colour DarkGrey    { 0xFF1A1A1A };
    const juce::Colour MidGrey     { 0xFF2A2A2A };
    const juce::Colour LightGrey   { 0xFF444444 };

    const juce::Colour Red         { 0xFFFF1E1E };  // Blood red (Ryu's headband)
    const juce::Colour RedDark     { 0xFF8B0000 };
    const juce::Colour RedGlow     { 0x60FF1E1E };

    const juce::Colour Blue        { 0xFF00C8FF };  // Electric blue (Chun-Li)
    const juce::Colour BlueDark    { 0xFF0066AA };
    const juce::Colour BlueGlow    { 0x6000C8FF };

    const juce::Colour Yellow      { 0xFFFFE500 };  // Neon yellow (KO flash)
    const juce::Colour YellowGlow  { 0x60FFE500 };

    const juce::Colour Green       { 0xFF00FF66 };  // Health bar
    const juce::Colour GreenDark   { 0xFF00AA44 };

    const juce::Colour Orange      { 0xFFFF6600 };  // Warning
    const juce::Colour Purple      { 0xFF8800FF };  // Hadouken purple

    const juce::Colour White       { 0xFFEEEEEE };
    const juce::Colour TextDim     { 0xFF888888 };

    const juce::Colour ScanlineOverlay { 0x08FFFFFF }; // Subtle CRT scanline
}

class GSFLookAndFeel : public juce::LookAndFeel_V4
{
public:
    GSFLookAndFeel();
    ~GSFLookAndFeel() override = default;

    // Font
    juce::Font getArcadeFont(float height) const;
    juce::Font getTitleFont(float height) const;

    // LookAndFeel overrides
    void drawButtonBackground(juce::Graphics&, juce::Button&,
                              const juce::Colour& backgroundColour,
                              bool shouldDrawButtonAsHighlighted,
                              bool shouldDrawButtonAsDown) override;

    void drawButtonText(juce::Graphics&, juce::TextButton&,
                        bool shouldDrawButtonAsHighlighted,
                        bool shouldDrawButtonAsDown) override;

    void drawLabel(juce::Graphics&, juce::Label&) override;

    void drawComboBox(juce::Graphics&, int width, int height,
                      bool isButtonDown, int, int, int, int,
                      juce::ComboBox&) override;

    // Utility: draw glow effect
    static void drawGlow(juce::Graphics& g, juce::Rectangle<float> bounds,
                          juce::Colour colour, float radius = 8.0f);

    // Utility: draw scanlines over area
    static void drawScanlines(juce::Graphics& g, juce::Rectangle<int> area,
                               float opacity = 0.03f);

    // Utility: draw pixel border (arcade style)
    static void drawArcadeBorder(juce::Graphics& g, juce::Rectangle<int> bounds,
                                  juce::Colour colour, int thickness = 2);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GSFLookAndFeel)
};

} // namespace gsf::ui
