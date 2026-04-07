/*
  ==============================================================================
   GSF FIGHTER - Fight Screen Implementation
  ==============================================================================
*/

#include "FightScreen.h"
#include <cmath>

namespace gsf::ui
{

FightScreen::FightScreen()
{
    setInterceptsMouseClicks(false, false);
}

void FightScreen::triggerFightSequence()
{
    phase = Phase::Ready;
    animating = true;
    frameCount = 0;
    textScale = 3.0f;
    alpha = 1.0f;
    setVisible(true);
    startTimerHz(60);
    repaint();
}

void FightScreen::triggerKOSequence()
{
    phase = Phase::KO;
    animating = true;
    frameCount = 0;
    textScale = 3.0f;
    alpha = 1.0f;
    setVisible(true);
    startTimerHz(60);
    repaint();
}

void FightScreen::timerCallback()
{
    frameCount++;

    switch (phase)
    {
        case Phase::Ready:
            textScale = 1.0f + 2.0f * std::exp(-frameCount * 0.1f);
            alpha = 1.0f;
            if (frameCount >= kReadyDuration)
            {
                phase = Phase::Fight;
                frameCount = 0;
                textScale = 4.0f;
            }
            break;

        case Phase::Fight:
            textScale = 1.0f + 3.0f * std::exp(-frameCount * 0.15f);
            alpha = 1.0f;
            if (frameCount >= kFightDuration)
            {
                phase = Phase::FadeOut;
                frameCount = 0;
            }
            break;

        case Phase::KO:
            textScale = 1.0f + 2.0f * std::exp(-frameCount * 0.08f);
            alpha = 1.0f;
            if (frameCount >= kKODuration)
            {
                phase = Phase::FadeOut;
                frameCount = 0;
            }
            break;

        case Phase::FadeOut:
            alpha = 1.0f - static_cast<float>(frameCount) / kFadeOutDuration;
            if (alpha <= 0.0f)
            {
                alpha = 0.0f;
                phase = Phase::Idle;
                animating = false;
                setVisible(false);
                stopTimer();
            }
            break;

        case Phase::Idle:
            stopTimer();
            return;
    }

    repaint();
}

void FightScreen::paint(juce::Graphics& g)
{
    if (!animating)
        return;

    auto bounds = getLocalBounds();

    // Semi-transparent black overlay
    g.setColour(Colours::Black.withAlpha(alpha * 0.7f));
    g.fillRect(bounds);

    // Scanline effect
    GSFLookAndFeel::drawScanlines(g, bounds, 0.05f * alpha);

    // Text
    juce::String text;
    juce::Colour textColour;

    switch (phase)
    {
        case Phase::Ready:
            text = "READY?";
            textColour = Colours::Yellow;
            break;
        case Phase::Fight:
            text = "FIGHT!";
            textColour = Colours::Red;
            break;
        case Phase::KO:
            text = "K.O.!";
            textColour = Colours::Red;
            break;
        case Phase::FadeOut:
            text = (phase == Phase::KO) ? "K.O.!" : "FIGHT!";
            textColour = Colours::Red;
            break;
        default:
            return;
    }

    // Draw text with shadow and glow
    float fontSize = bounds.getHeight() * 0.2f * textScale;
    auto font = juce::Font(juce::FontOptions(fontSize, juce::Font::bold));
    g.setFont(font);

    auto textBounds = bounds.reduced(20);

    // Shadow
    g.setColour(Colours::Black.withAlpha(alpha * 0.8f));
    g.drawFittedText(text, textBounds.translated(4, 4), juce::Justification::centred, 1);

    // Glow layers
    for (int i = 3; i >= 0; --i)
    {
        float glowAlpha = alpha * 0.15f * (4 - i) / 4.0f;
        g.setColour(textColour.withAlpha(glowAlpha));
        g.drawFittedText(text, textBounds.expanded(i * 3), juce::Justification::centred, 1);
    }

    // Main text
    g.setColour(textColour.withAlpha(alpha));
    g.drawFittedText(text, textBounds, juce::Justification::centred, 1);

    // White inner glow
    g.setColour(juce::Colours::white.withAlpha(alpha * 0.5f));
    auto innerFont = juce::Font(juce::FontOptions(fontSize * 0.98f, juce::Font::bold));
    g.setFont(innerFont);
    g.drawFittedText(text, textBounds, juce::Justification::centred, 1);
}

} // namespace gsf::ui
