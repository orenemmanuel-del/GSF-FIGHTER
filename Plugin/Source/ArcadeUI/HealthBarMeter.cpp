/*
  ==============================================================================
   GSF FIGHTER - Health Bar Meter Implementation
  ==============================================================================
*/

#include "HealthBarMeter.h"

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
    // Smooth decay
    if (currentPeak > displayPeak)
        displayPeak = currentPeak;
    else
        displayPeak = displayPeak * kDecayRate;

    if (currentRms > displayRms)
        displayRms = currentRms;
    else
        displayRms = displayRms * kDecayRate;

    // Peak hold
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
    const int labelWidth = 36;
    const int barHeight  = bounds.getHeight();

    // Background
    g.setColour(Colours::DarkGrey);
    g.fillRoundedRectangle(bounds.toFloat(), 3.0f);

    // Label area (fighter name like "P1", "P2", "L", "R")
    auto labelBounds = leftToRight
        ? bounds.removeFromLeft(labelWidth)
        : bounds.removeFromRight(labelWidth);

    g.setColour(Colours::Red);
    g.fillRoundedRectangle(labelBounds.toFloat(), 3.0f);
    g.setColour(Colours::Yellow);
    g.setFont(juce::Font(juce::FontOptions(14.0f, juce::Font::bold)));
    g.drawFittedText(label, labelBounds, juce::Justification::centred, 1);

    // Bar area
    auto barBounds = bounds.reduced(3, 4).toFloat();
    const float barWidth = barBounds.getWidth();

    // Background segments (like SF health bar segments)
    g.setColour(Colours::Black.brighter(0.05f));
    g.fillRoundedRectangle(barBounds, 2.0f);

    // Draw segments
    const int numSegments = 32;
    const float segWidth = barWidth / numSegments;
    const float gapWidth = 1.0f;

    float rmsWidth = displayRms * barWidth;
    float peakWidth = displayPeak * barWidth;

    for (int i = 0; i < numSegments; ++i)
    {
        float segStart = leftToRight
            ? barBounds.getX() + i * segWidth
            : barBounds.getRight() - (i + 1) * segWidth;

        float segEnd = segStart + segWidth - gapWidth;
        float segPos = i * segWidth;

        juce::Rectangle<float> segRect;
        if (leftToRight)
            segRect = { segStart, barBounds.getY(), segWidth - gapWidth, barBounds.getHeight() };
        else
            segRect = { segStart + gapWidth, barBounds.getY(), segWidth - gapWidth, barBounds.getHeight() };

        // Determine colour based on position
        float normalizedPos = static_cast<float>(i) / numSegments;
        juce::Colour segColour;

        if (normalizedPos < 0.5f)
            segColour = Colours::Green;
        else if (normalizedPos < 0.7f)
            segColour = Colours::Yellow;
        else if (normalizedPos < 0.85f)
            segColour = Colours::Orange;
        else
            segColour = Colours::Red;

        // Draw based on level
        if (segPos < rmsWidth)
        {
            // Filled (RMS level)
            g.setColour(segColour);
            g.fillRoundedRectangle(segRect, 1.0f);
        }
        else if (segPos < peakWidth)
        {
            // Peak area (dimmer)
            g.setColour(segColour.withAlpha(0.3f));
            g.fillRoundedRectangle(segRect, 1.0f);
        }
        else
        {
            // Empty
            g.setColour(Colours::Black.brighter(0.08f));
            g.fillRoundedRectangle(segRect, 1.0f);
        }
    }

    // Peak hold indicator
    if (peakHold > 0.01f)
    {
        float holdPos = peakHold * barWidth;
        float holdX = leftToRight
            ? barBounds.getX() + holdPos
            : barBounds.getRight() - holdPos;

        g.setColour(Colours::White);
        g.drawLine(holdX, barBounds.getY(), holdX, barBounds.getBottom(), 2.0f);
    }

    // Border
    g.setColour(Colours::LightGrey);
    g.drawRoundedRectangle(getLocalBounds().toFloat(), 3.0f, 1.0f);
}

} // namespace gsf::ui
