/*
  ==============================================================================
   GSF FIGHTER — Carbon Look & Feel
   IBM Carbon-inspired dark monitoring aesthetic.
   Palette: Gray 100 dark theme, Blue 40 interactive, Red 50 clip.
  ==============================================================================
*/

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace gsf::ui
{

namespace Colours
{
    // Backgrounds (Carbon Gray 100 dark theme)
    const juce::Colour Black            { 0xFF161616 }; // Gray 100
    const juce::Colour DarkGrey         { 0xFF262626 }; // Gray 90
    const juce::Colour MidGrey          { 0xFF393939 }; // Gray 80
    const juce::Colour LightGrey        { 0xFF4C4C4C }; // Gray 70 (hover)

    // Interactive — Blue
    const juce::Colour Cyan             { 0xFF78A9FF }; // Blue 40 (dark theme)
    const juce::Colour CyanDim          { 0xFF4589FF }; // Blue 50
    const juce::Colour CyanGlow         { 0x2278A9FF };

    // Red (clip / error)
    const juce::Colour Red              { 0xFFFA4D56 }; // Red 50
    const juce::Colour RedDark          { 0xFF620000 }; // Red 90
    const juce::Colour RedGlow          { 0x40FA4D56 };

    // Text
    const juce::Colour White            { 0xFFF4F4F4 }; // Gray 10
    const juce::Colour TextSecondary    { 0xFFC6C6C6 }; // Gray 30
    const juce::Colour TextDim          { 0xFF6F6F6F }; // Gray 60

    // Borders
    const juce::Colour BorderSubtle     { 0xFF393939 }; // Gray 80

    // Support
    const juce::Colour Success          { 0xFF42BE65 }; // Green 40
    const juce::Colour Warning          { 0xFFF1C21B }; // Yellow 30

    // Legacy aliases
    const juce::Colour Blue             = Cyan;
    const juce::Colour BlueDark         = CyanDim;
    const juce::Colour BlueGlow         = CyanGlow;
    const juce::Colour Yellow           = Warning;
    const juce::Colour YellowGlow       = CyanGlow;
    const juce::Colour Green            = Success;
    const juce::Colour GreenDark        { 0xFF24A148 };
    const juce::Colour Orange           = Red;
    const juce::Colour Purple           = Cyan;

    const juce::Colour ScanlineOverlay  { 0x00FFFFFF }; // disabled
}

class GSFLookAndFeel : public juce::LookAndFeel_V4
{
public:
    GSFLookAndFeel();
    ~GSFLookAndFeel() override = default;

    juce::Font getMonoFont(float height, bool bold = false) const;

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

    static void drawGlow(juce::Graphics& g, juce::Rectangle<float> bounds,
                         juce::Colour colour, float radius = 6.0f);
    static void drawScanlines(juce::Graphics&, juce::Rectangle<int>, float) {}
    static void drawArcadeBorder(juce::Graphics& g, juce::Rectangle<int> bounds,
                                 juce::Colour colour, int thickness = 1);
    static void drawWifiIcon(juce::Graphics& g, juce::Rectangle<float> bounds,
                             juce::Colour colour, float pulsePhase);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GSFLookAndFeel)
};

} // namespace gsf::ui
