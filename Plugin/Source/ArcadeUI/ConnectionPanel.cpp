/*
  ==============================================================================
   GSF FIGHTER — Input Matrix Panel Implementation
  ==============================================================================
*/

#include "ConnectionPanel.h"
#include <cmath>

namespace gsf::ui
{

ConnectionPanel::ConnectionPanel()
{
    addAndMakeVisible(startStopButton);
    addAndMakeVisible(qualityStandard);
    addAndMakeVisible(qualityHiRes);
    addAndMakeVisible(meterL);
    addAndMakeVisible(meterR);

    // Start/Stop is invisible — the whole status area is clickable through the button
    startStopButton.setAlpha(0.0f);
    startStopButton.setClickingTogglesState(true);
    startStopButton.onClick = [this]()
    {
        if (onStartStop) onStartStop();
    };

    qualityStandard.setClickingTogglesState(true);
    qualityHiRes.setClickingTogglesState(true);
    qualityStandard.setToggleState(true, juce::dontSendNotification);

    qualityStandard.onClick = [this]()
    {
        qualityStandard.setToggleState(true, juce::dontSendNotification);
        qualityHiRes.setToggleState(false, juce::dontSendNotification);
        repaint();
    };
    qualityHiRes.onClick = [this]()
    {
        qualityStandard.setToggleState(false, juce::dontSendNotification);
        qualityHiRes.setToggleState(true, juce::dontSendNotification);
        repaint();
    };

    startTimerHz(60);
}

void ConnectionPanel::setConnectionCode(const std::array<uint8_t, 4>& code) { connectionCode = code; repaint(); }
void ConnectionPanel::setIPAddress(const juce::String& ip)                 { ipAddress = ip;      repaint(); }
void ConnectionPanel::setClientCount(int count)                             { clientCount = count; repaint(); }

void ConnectionPanel::setStreaming(bool streaming)
{
    isStreaming = streaming;
    startStopButton.setToggleState(streaming, juce::dontSendNotification);
    repaint();
}

void ConnectionPanel::setLevels(float peakL, float peakR, float rmsL, float rmsR)
{
    meterL.setLevel(peakL, rmsL);
    meterR.setLevel(peakR, rmsR);
}

void ConnectionPanel::timerCallback()
{
    pulsePhase += 0.1f;
    if (isStreaming) repaint();
}

void ConnectionPanel::paint(juce::Graphics& g)
{
    // --- Top left: DEVICE STATUS ---
    auto statusLabelArea = statusArea.withHeight(14);
    drawSectionLabel(g, statusLabelArea, "DEVICE STATUS");
    drawStatusButton(g, statusArea.withTrimmedTop(18));

    // --- Top right: STREAM QUALITY label (buttons placed by resized()) ---
    auto qualityLabelArea = qualityArea.withHeight(14);
    drawSectionLabel(g, qualityLabelArea, "STREAM QUALITY");

    // --- Meters card background ---
    g.setColour(Colours::MidGrey);
    g.fillRect(metersArea.toFloat());
    g.setColour(Colours::BorderSubtle);
    g.drawRect(metersArea.toFloat(), 1.0f);

    // --- Stats row ---
    drawStatsRow(g, statsArea);
}

void ConnectionPanel::drawSectionLabel(juce::Graphics& g, juce::Rectangle<int> area,
                                       const juce::String& label)
{
    g.setColour(Colours::TextDim);
    g.setFont(juce::Font(juce::FontOptions(juce::Font::getDefaultMonospacedFontName(),
                                           9.0f, juce::Font::plain)));
    g.drawFittedText(label, area, juce::Justification::centredLeft, 1);
}

void ConnectionPanel::drawStatusButton(juce::Graphics& g, juce::Rectangle<int> area)
{
    auto btnRect = area.withHeight(28);

    // Fill (card bg)
    g.setColour(Colours::MidGrey);
    g.fillRect(btnRect.toFloat());

    // Border (cyan)
    g.setColour(isStreaming ? Colours::Cyan : Colours::Cyan.withAlpha(0.6f));
    g.drawRect(btnRect.toFloat(), 1.0f);

    // Status dot
    auto dotCentre = juce::Point<float>(btnRect.getX() + 14.0f, btnRect.getCentreY());
    float dotAlpha = isStreaming ? (0.6f + 0.4f * std::sin(pulsePhase)) : 0.8f;
    g.setColour((isStreaming ? Colours::Cyan : Colours::Cyan).withAlpha(dotAlpha));
    g.fillEllipse(dotCentre.x - 3.0f, dotCentre.y - 3.0f, 6.0f, 6.0f);

    // Text
    juce::String label = isStreaming
        ? "CONNECTED — " + juce::String(clientCount) + " CLIENT" + (clientCount != 1 ? "S" : "")
        : "READY — WAITING FOR PHONE";

    g.setColour(Colours::Cyan);
    g.setFont(juce::Font(juce::FontOptions(juce::Font::getDefaultMonospacedFontName(),
                                           10.0f, juce::Font::plain)));
    auto textArea = btnRect.withTrimmedLeft(28);
    g.drawFittedText(label, textArea, juce::Justification::centredLeft, 1);

    // IP on the right
    g.setColour(Colours::TextDim);
    g.drawFittedText(ipAddress, btnRect.withTrimmedRight(10),
                     juce::Justification::centredRight, 1);
}

void ConnectionPanel::drawStatsRow(juce::Graphics& g, juce::Rectangle<int> area)
{
    const int numCells = 3;
    const int cellW = area.getWidth() / numCells;

    auto drawCell = [&](juce::Rectangle<int> cell, const juce::String& label,
                        const juce::String& value)
    {
        g.setColour(Colours::TextDim);
        g.setFont(juce::Font(juce::FontOptions(juce::Font::getDefaultMonospacedFontName(),
                                               8.0f, juce::Font::plain)));
        g.drawFittedText(label, cell.withHeight(12),
                         juce::Justification::centredLeft, 1);

        g.setColour(Colours::White);
        g.setFont(juce::Font(juce::FontOptions(juce::Font::getDefaultMonospacedFontName(),
                                               11.0f, juce::Font::plain)));
        g.drawFittedText(value, cell.withTrimmedTop(12),
                         juce::Justification::centredLeft, 1);
    };

    juce::String portValue    = juce::String(47701);
    int bufPct = (int) juce::jlimit(0.0f, 100.0f, 98.0f - (float) clientCount);
    juce::String bufferValue  = juce::String(bufPct) + "%";
    juce::String dropoutValue = juce::String(0);

    auto c0 = juce::Rectangle<int>(area.getX(),             area.getY(), cellW, area.getHeight());
    auto c1 = juce::Rectangle<int>(area.getX() + cellW,     area.getY(), cellW, area.getHeight());
    auto c2 = juce::Rectangle<int>(area.getX() + cellW * 2, area.getY(), cellW, area.getHeight());

    drawCell(c0.reduced(10, 4), "NETWORK PORT", portValue);
    drawCell(c1.reduced(10, 4), "BUFFER HEALTH", bufferValue);
    drawCell(c2.reduced(10, 4), "DROPOUTS",      dropoutValue);

    // Vertical dividers
    g.setColour(Colours::BorderSubtle);
    g.drawVerticalLine(c1.getX(), (float) area.getY() + 6.0f,
                       (float) area.getBottom() - 6.0f);
    g.drawVerticalLine(c2.getX(), (float) area.getY() + 6.0f,
                       (float) area.getBottom() - 6.0f);
}

void ConnectionPanel::resized()
{
    auto bounds = getLocalBounds();

    // Top row: status (left) + quality (right), ~46 px tall
    auto topRow = bounds.removeFromTop(46);
    statusArea  = topRow.removeFromLeft(topRow.getWidth() - 230);
    topRow.removeFromLeft(12);
    qualityArea = topRow;

    // The quality toggle buttons (below the label)
    auto qualityButtonRow = qualityArea.withTrimmedTop(18).withHeight(26);
    int btnW = (qualityButtonRow.getWidth() - 8) / 2;
    qualityStandard.setBounds(qualityButtonRow.withWidth(btnW));
    qualityHiRes.setBounds(qualityButtonRow.withTrimmedLeft(btnW + 8).withWidth(btnW));

    // Invisible click surface for start/stop covers the status button
    startStopButton.setBounds(statusArea.withTrimmedTop(18).withHeight(28));

    bounds.removeFromTop(12);

    // Stats row at the bottom
    statsArea = bounds.removeFromBottom(44);

    bounds.removeFromBottom(10);

    // Meters card fills the remaining space
    metersArea = bounds;

    auto inner = metersArea.reduced(16);
    int mh = 34;
    int gap = 14;
    meterL.setBounds(inner.removeFromTop(mh));
    inner.removeFromTop(gap);
    meterR.setBounds(inner.removeFromTop(mh));
}

} // namespace gsf::ui
