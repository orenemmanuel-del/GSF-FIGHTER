/*
  ==============================================================================
   GSF FIGHTER - Arcade Button Implementation
  ==============================================================================
*/

#include "ArcadeButton.h"
#include <cmath>

namespace gsf::ui
{

ArcadeButton::ArcadeButton(const juce::String& text, Style s)
    : juce::TextButton(text), style(s)
{
    setColour(juce::TextButton::textColourOffId, Colours::White);
    setColour(juce::TextButton::textColourOnId, Colours::Yellow);
}

void ArcadeButton::setPulsing(bool shouldPulse)
{
    pulsing = shouldPulse;
    if (pulsing)
        pulsePhase = 0.0f;
    repaint();
}

juce::Colour ArcadeButton::getBaseColour() const
{
    switch (style)
    {
        case Style::Hadouken:  return Colours::Red;
        case Style::Shoryuken: return Colours::Blue;
        case Style::Tatsumaki: return Colours::Yellow;
        case Style::KO:        return Colours::Red;
        case Style::Round:     return Colours::MidGrey;
        default:               return Colours::MidGrey;
    }
}

juce::Colour ArcadeButton::getGlowColour() const
{
    switch (style)
    {
        case Style::Hadouken:  return Colours::RedGlow;
        case Style::Shoryuken: return Colours::BlueGlow;
        case Style::Tatsumaki: return Colours::YellowGlow;
        case Style::KO:        return Colours::RedGlow;
        case Style::Round:     return Colours::BlueGlow;
        default:               return Colours::BlueGlow;
    }
}

void ArcadeButton::paintButton(juce::Graphics& g, bool isHighlighted, bool isDown)
{
    auto bounds = getLocalBounds().toFloat().reduced(2.0f);
    auto baseCol = getBaseColour();
    auto glowCol = getGlowColour();

    if (isDown)
        baseCol = baseCol.darker(0.2f);
    else if (isHighlighted)
        baseCol = baseCol.brighter(0.15f);

    bool isOn = getToggleState();

    // === CIRCULAR PAD STYLE ===
    if (style == Style::Round)
    {
        float diameter = juce::jmin(bounds.getWidth(), bounds.getHeight());
        auto circleBounds = bounds.withSizeKeepingCentre(diameter, diameter);

        // Shadow
        g.setColour(Colours::Black);
        g.fillEllipse(circleBounds.translated(2.0f, 2.0f));

        // Base
        g.setGradientFill(juce::ColourGradient(
            baseCol.brighter(0.3f), circleBounds.getCentreX(), circleBounds.getY(),
            baseCol.darker(0.2f), circleBounds.getCentreX(), circleBounds.getBottom(),
            false));
        auto drawBounds = isDown ? circleBounds.translated(0, 1.0f) : circleBounds;
        g.fillEllipse(drawBounds);

        // Highlight
        g.setColour(juce::Colours::white.withAlpha(0.15f));
        g.fillEllipse(drawBounds.reduced(diameter * 0.15f)
                          .withHeight(diameter * 0.3f));

        // Glow when on
        if (isOn || pulsing)
        {
            GSFLookAndFeel::drawGlow(g, drawBounds, glowCol.withAlpha(0.4f), 15.0f);
        }

        // Border
        g.setColour(baseCol.brighter(0.4f));
        g.drawEllipse(drawBounds, 1.5f);
    }
    // === RECTANGULAR ARCADE BUTTON ===
    else
    {
        float cornerRadius = 6.0f;

        // Outer shadow
        g.setColour(Colours::Black.withAlpha(0.6f));
        g.fillRoundedRectangle(bounds.translated(2.0f, 3.0f), cornerRadius);

        // Main button body
        auto drawBounds = isDown ? bounds.translated(0, 1.5f) : bounds;

        // Gradient fill
        g.setGradientFill(juce::ColourGradient(
            baseCol.brighter(0.2f), drawBounds.getX(), drawBounds.getY(),
            baseCol.darker(0.3f), drawBounds.getX(), drawBounds.getBottom(),
            false));
        g.fillRoundedRectangle(drawBounds, cornerRadius);

        // Top highlight stripe
        g.setColour(juce::Colours::white.withAlpha(0.1f));
        auto highlightRect = drawBounds.removeFromTop(drawBounds.getHeight() * 0.3f);
        g.fillRoundedRectangle(highlightRect, cornerRadius);

        // Glow effect
        if (isOn || isHighlighted || pulsing)
        {
            float glowAlpha = 0.3f;
            if (pulsing)
            {
                pulsePhase += 0.05f;
                glowAlpha = 0.2f + 0.2f * std::sin(pulsePhase);
            }
            GSFLookAndFeel::drawGlow(g, bounds, glowCol.withAlpha(glowAlpha), 12.0f);
        }

        // Border
        g.setColour(isOn ? Colours::Yellow : baseCol.brighter(0.5f));
        g.drawRoundedRectangle(bounds, cornerRadius, isOn ? 2.0f : 1.5f);
    }

    // === TEXT ===
    auto textBounds = getLocalBounds();
    if (isDown)
        textBounds = textBounds.translated(0, 2);

    auto textCol = isOn ? findColour(juce::TextButton::textColourOnId)
                        : findColour(juce::TextButton::textColourOffId);
    g.setColour(textCol);
    g.setFont(juce::Font(juce::FontOptions(juce::jmin(15.0f, getHeight() * 0.35f), juce::Font::bold)));
    g.drawFittedText(getButtonText(), textBounds, juce::Justification::centred, 2);
}

} // namespace gsf::ui
