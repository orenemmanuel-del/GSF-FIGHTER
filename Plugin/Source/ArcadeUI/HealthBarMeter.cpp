/*
  ==============================================================================
   GSF FIGHTER — VU Meter (Oscilloscope Design)
   Flat cyan bar with red clip zone, dB scale marks.
  ==============================================================================
*/

#include "HealthBarMeter.h"
#include <cmath>

namespace gsf::ui
{

HealthBarMeter::HealthBarMeter(const juce::String& lbl, bool ltr)
    : label(lbl), leftToRight(ltr)
{
    startTimerHz(60);
}

void HealthBarMeter::setLevel(float peak, float rms)
{
    currentPeak = peak;
    currentRms  = rms;
}

void HealthBarMeter::timerCallback()
{
    // Attack fast, release slow
    const float attack = 0.7f;
    const float release = 0.88f;

    if (currentPeak > displayPeak)
        displayPeak = displayPeak + (currentPeak - displayPeak) * attack;
    else
        displayPeak *= release;

    if (currentRms > displayRms)
        displayRms = displayRms + (currentRms - displayRms) * attack;
    else
        displayRms *= release;

    if (currentPeak > peakHold)
    {
        peakHold = currentPeak;
        peakHoldCounter = kPeakHoldFrames;
    }
    else if (peakHoldCounter > 0)
    {
        --peakHoldCounter;
    }
    else
    {
        peakHold *= 0.98f;
    }

    repaint();
}

void HealthBarMeter::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();

    // Layout: [L] label (left), scale (top right), bar (bottom)
    auto labelArea = bounds.removeFromLeft(20);
    g.setColour(Colours::TextSecondary);
    g.setFont(juce::Font(juce::FontOptions(juce::Font::getDefaultMonospacedFontName(),
                                           12.0f, juce::Font::plain)));
    g.drawFittedText(label, labelArea, juce::Justification::centred, 1);

    bounds.removeFromLeft(6);

    // Scale marks (top)
    auto scaleArea = bounds.removeFromTop(12);
    const float dbMarks[] = { -40.0f, -30.0f, -20.0f, -10.0f, -6.0f, -3.0f, 0.0f };
    const int numMarks = (int) (sizeof(dbMarks) / sizeof(float));

    auto dbToPos = [&](float db) -> float
    {
        // Map -60..0 dB to 0..1 with slight non-linearity
        float norm = juce::jlimit(0.0f, 1.0f, (db + 60.0f) / 60.0f);
        return norm;
    };

    g.setColour(Colours::TextDim);
    g.setFont(juce::Font(juce::FontOptions(juce::Font::getDefaultMonospacedFontName(),
                                           8.0f, juce::Font::plain)));
    for (int i = 0; i < numMarks; ++i)
    {
        float x = scaleArea.getX() + dbToPos(dbMarks[i]) * scaleArea.getWidth();
        juce::String txt = dbMarks[i] == 0.0f
                               ? juce::String("0")
                               : juce::String((int) dbMarks[i]);
        g.drawText(txt, (int) x - 12, scaleArea.getY(), 24, scaleArea.getHeight(),
                   juce::Justification::centred, false);
    }

    // Bar background
    auto barArea = bounds.withTrimmedTop(2).withHeight(16).toFloat();
    g.setColour(Colours::Black);
    g.fillRect(barArea);

    // Level fill (level is already 0..1 peak)
    float level = juce::jlimit(0.0f, 1.0f, displayPeak);
    float fillW = level * barArea.getWidth();

    // Split into cyan (0..90%) and red (90..100%)
    float clipThreshold = 0.9f * barArea.getWidth();

    if (fillW > 0.0f)
    {
        float cyanW = juce::jmin(fillW, clipThreshold);
        juce::Rectangle<float> cyanRect { barArea.getX(), barArea.getY(),
                                          cyanW, barArea.getHeight() };
        g.setColour(Colours::Cyan);
        g.fillRect(cyanRect);

        if (fillW > clipThreshold)
        {
            float redW = fillW - clipThreshold;
            juce::Rectangle<float> redRect { barArea.getX() + clipThreshold,
                                             barArea.getY(), redW, barArea.getHeight() };
            g.setColour(Colours::Red);
            g.fillRect(redRect);
        }
    }

    // Clip threshold tick mark
    float clipX = barArea.getX() + clipThreshold;
    g.setColour(Colours::Red.withAlpha(0.5f));
    g.drawVerticalLine((int) clipX, barArea.getY(), barArea.getBottom());
}

} // namespace gsf::ui
