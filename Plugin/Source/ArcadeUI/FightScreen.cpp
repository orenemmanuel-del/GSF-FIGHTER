/*
  ==============================================================================
   GSF FIGHTER — Connecting Overlay (Oscilloscope Design)
   Minimal cyan overlay that fades out after a connect/disconnect event.
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
    phase = Phase::Fight;
    animating = true;
    frameCount = 0;
    textScale = 1.0f;
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
    textScale = 1.0f;
    alpha = 1.0f;
    setVisible(true);
    startTimerHz(60);
    repaint();
}

void FightScreen::timerCallback()
{
    frameCount++;

    const int hold = 30;
    const int fade = 20;

    if (frameCount < hold)
    {
        alpha = 1.0f;
    }
    else if (frameCount < hold + fade)
    {
        alpha = 1.0f - (float)(frameCount - hold) / (float) fade;
    }
    else
    {
        alpha = 0.0f;
        animating = false;
        phase = Phase::Idle;
        setVisible(false);
        stopTimer();
    }

    repaint();
}

void FightScreen::paint(juce::Graphics& g)
{
    if (!animating) return;

    auto bounds = getLocalBounds();

    // Dark scrim
    g.setColour(Colours::Black.withAlpha(alpha * 0.75f));
    g.fillRect(bounds);

    juce::String text = (phase == Phase::KO) ? "SESSION ENDED" : "CONNECTED";
    juce::Colour col  = (phase == Phase::KO) ? Colours::Red   : Colours::Cyan;

    // Text
    g.setColour(col.withAlpha(alpha));
    g.setFont(juce::Font(juce::FontOptions(juce::Font::getDefaultMonospacedFontName(),
                                           32.0f, juce::Font::bold)));
    g.drawFittedText(text, bounds, juce::Justification::centred, 1);

    // Subtle underline
    int textW = 220;
    int underlineY = bounds.getCentreY() + 26;
    g.drawHorizontalLine(underlineY,
                         (float) bounds.getCentreX() - textW * 0.5f,
                         (float) bounds.getCentreX() + textW * 0.5f);
}

} // namespace gsf::ui
