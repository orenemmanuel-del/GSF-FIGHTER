/*
  ==============================================================================
   GSF FIGHTER — Button (Oscilloscope Design)
   Flat rectangular toggles + simple accent border.
  ==============================================================================
*/

#include "ArcadeButton.h"
#include <cmath>

namespace gsf::ui
{

ArcadeButton::ArcadeButton(const juce::String& text, Style s)
    : juce::TextButton(text), style(s)
{
    setColour(juce::TextButton::textColourOffId, Colours::TextSecondary);
    setColour(juce::TextButton::textColourOnId,  Colours::Black);
}

void ArcadeButton::setPulsing(bool shouldPulse)
{
    pulsing = shouldPulse;
    if (pulsing) pulsePhase = 0.0f;
    repaint();
}

juce::Colour ArcadeButton::getBaseColour() const
{
    return Colours::MidGrey;
}

juce::Colour ArcadeButton::getGlowColour() const
{
    return Colours::CyanGlow;
}

void ArcadeButton::paintButton(juce::Graphics& g, bool isHighlighted, bool isDown)
{
    auto bounds = getLocalBounds().toFloat().reduced(0.5f);
    const bool isOn = getToggleState();

    juce::Colour fill;
    juce::Colour border;
    juce::Colour textCol;

    if (isOn)
    {
        fill    = Colours::Cyan;
        border  = Colours::Cyan;
        textCol = Colours::Black;
    }
    else if (isDown || isHighlighted)
    {
        fill    = Colours::LightGrey;
        border  = Colours::BorderSubtle;
        textCol = Colours::White;
    }
    else
    {
        fill    = Colours::MidGrey;
        border  = Colours::BorderSubtle;
        textCol = Colours::TextDim;
    }

    // Circular style: draw as circle
    if (style == Style::Round)
    {
        float diameter = juce::jmin(bounds.getWidth(), bounds.getHeight());
        auto circle = bounds.withSizeKeepingCentre(diameter, diameter);
        g.setColour(fill);
        g.fillEllipse(circle);
        g.setColour(border);
        g.drawEllipse(circle, 1.0f);
    }
    else
    {
        g.setColour(fill);
        g.fillRect(bounds);
        g.setColour(border);
        g.drawRect(bounds, 1.0f);
    }

    // Text
    g.setColour(textCol);
    auto fontSize = juce::jmin(11.0f, getHeight() * 0.42f);
    g.setFont(juce::Font(juce::FontOptions(juce::Font::getDefaultMonospacedFontName(),
                                           fontSize, juce::Font::plain)));
    g.drawFittedText(getButtonText(), getLocalBounds(),
                     juce::Justification::centred, 1);

    juce::ignoreUnused(isDown, isHighlighted);
}

} // namespace gsf::ui
