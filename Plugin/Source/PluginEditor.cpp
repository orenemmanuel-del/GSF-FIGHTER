/*
  ==============================================================================
   GSF FIGHTER — Plugin Editor (Oscilloscope Design)
  ==============================================================================
*/

#include "PluginEditor.h"
#include "Network/DeviceDiscovery.h"
#include <cmath>

using namespace gsf::ui;

GSFFighterEditor::GSFFighterEditor(GSFFighterProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    setLookAndFeel(&oscLookAndFeel);
    setResizable(true, true);
    setResizeLimits(800, 450, 1000, 560);
    setSize(kPluginWidth, kPluginHeight);

    addAndMakeVisible(inputMatrixPanel);
    inputMatrixPanel.setConnectionCode(processorRef.getConnectionCode());
    inputMatrixPanel.setIPAddress(DeviceDiscovery::getLocalIPAddress());
    inputMatrixPanel.setStreaming(processorRef.isStreaming());

    inputMatrixPanel.onStartStop = [this]()
    {
        bool shouldStream = !processorRef.isStreaming();
        processorRef.setStreaming(shouldStream);
        inputMatrixPanel.setStreaming(shouldStream);

        if (shouldStream)
            overlayScreen.triggerFightSequence();
        else
            overlayScreen.triggerKOSequence();
    };

    inputMatrixPanel.onRegenCode = [this]()
    {
        processorRef.regenerateConnectionCode();
        inputMatrixPanel.setConnectionCode(processorRef.getConnectionCode());
    };

    addAndMakeVisible(sidebarPanel);
    sidebarPanel.setActivePreset(processorRef.getTranslationEngine().getCurrentPreset());
    sidebarPanel.onPresetChanged = [this](gsf::PresetID preset)
    {
        processorRef.getTranslationEngine().setPreset(preset);
    };

    addChildComponent(overlayScreen);
    overlayScreen.setVisible(false);

    startTimerHz(60);
}

GSFFighterEditor::~GSFFighterEditor()
{
    setLookAndFeel(nullptr);
}

void GSFFighterEditor::timerCallback()
{
    inputMatrixPanel.setLevels(
        processorRef.getPeakLevel(0), processorRef.getPeakLevel(1),
        processorRef.getRmsLevel(0),  processorRef.getRmsLevel(1));
    inputMatrixPanel.setClientCount(processorRef.getConnectedClientCount());

    wifiPhase += 0.08f;
    repaint();
}

void GSFFighterEditor::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();

    // Background deep
    g.fillAll(Colours::Black);

    // Header
    auto header = bounds.removeFromTop(kHeaderHeight);
    drawHeader(g, header);

    // Footer
    auto footer = bounds.removeFromBottom(kFooterHeight);
    drawBottomNav(g, footer);

    // Sidebar background
    auto sidebarArea = bounds.withWidth(kSidebarWidth);
    g.setColour(Colours::DarkGrey);
    g.fillRect(sidebarArea);
    g.setColour(Colours::BorderSubtle);
    g.drawVerticalLine(sidebarArea.getRight() - 1,
                       (float) sidebarArea.getY(),
                       (float) sidebarArea.getBottom());
}

void GSFFighterEditor::resized()
{
    auto bounds = getLocalBounds();
    bounds.removeFromTop(kHeaderHeight);
    bounds.removeFromBottom(kFooterHeight);

    auto sidebarArea = bounds.removeFromLeft(kSidebarWidth);
    sidebarPanel.setBounds(sidebarArea);

    inputMatrixPanel.setBounds(bounds.reduced(20, 16));

    overlayScreen.setBounds(getLocalBounds());
}

void GSFFighterEditor::drawHeader(juce::Graphics& g, juce::Rectangle<int> bounds)
{
    // Panel background
    g.setColour(Colours::DarkGrey);
    g.fillRect(bounds);

    // Bottom border
    g.setColour(Colours::BorderSubtle);
    g.drawHorizontalLine(bounds.getBottom() - 1,
                         (float) bounds.getX(),
                         (float) bounds.getRight());

    auto content = bounds.reduced(16, 0);

    // --- Left: icon + OSCILLOSCOPE title ---
    auto leftArea = content.removeFromLeft(260);
    auto iconArea = leftArea.removeFromLeft(28).withSizeKeepingCentre(14, 20);

    // Draw "|||" icon (3 vertical bars) in cyan
    g.setColour(Colours::Cyan);
    int barW = 3, gap = 2;
    for (int i = 0; i < 3; ++i)
    {
        int x = iconArea.getX() + i * (barW + gap);
        g.fillRect(juce::Rectangle<int>(x, iconArea.getY() + 2,
                                        barW, iconArea.getHeight() - 4));
    }

    leftArea.removeFromLeft(8);
    g.setColour(Colours::White);
    g.setFont(oscLookAndFeel.getMonoFont(13.0f, true));
    g.drawFittedText("OSCILLOSCOPE", leftArea, juce::Justification::centredLeft, 1);

    // --- Right: version + label + wifi ---
    auto rightArea = content;
    auto wifiZone  = rightArea.removeFromRight(28);

    g.setColour(Colours::TextSecondary);
    g.setFont(oscLookAndFeel.getMonoFont(10.0f));
    rightArea.removeFromRight(12);

    g.drawFittedText("PHONECHECK MONITOR",
                     rightArea.removeFromRight(160),
                     juce::Justification::centredRight, 1);
    rightArea.removeFromRight(12);
    g.drawFittedText("V 1.2.4",
                     rightArea.removeFromRight(60),
                     juce::Justification::centredRight, 1);

    // Wifi icon (pulsing cyan when streaming)
    auto wifiBounds = wifiZone.toFloat().withSizeKeepingCentre(22.0f, 22.0f);
    auto wifiCol = processorRef.isStreaming()
                       ? Colours::Cyan
                       : Colours::TextDim;
    GSFLookAndFeel::drawWifiIcon(g, wifiBounds, wifiCol,
                                 processorRef.isStreaming() ? wifiPhase : 0.0f);
}

void GSFFighterEditor::drawBottomNav(juce::Graphics& g, juce::Rectangle<int> bounds)
{
    // Panel background
    g.setColour(Colours::DarkGrey);
    g.fillRect(bounds);

    // Top border
    g.setColour(Colours::BorderSubtle);
    g.drawHorizontalLine(bounds.getY(),
                         (float) bounds.getX(),
                         (float) bounds.getRight());

    // 4 icons evenly spaced
    const int numIcons = 4;
    const int cellW = bounds.getWidth() / numIcons;

    auto drawBars = [&](juce::Rectangle<int> cell, juce::Colour col)
    {
        auto c = cell.withSizeKeepingCentre(18, 16);
        g.setColour(col);
        g.fillRect(juce::Rectangle<int>(c.getX(),       c.getBottom() - 6, 3, 6));
        g.fillRect(juce::Rectangle<int>(c.getX() + 5,   c.getBottom() - 10, 3, 10));
        g.fillRect(juce::Rectangle<int>(c.getX() + 10,  c.getBottom() - 14, 3, 14));
        g.fillRect(juce::Rectangle<int>(c.getX() + 15,  c.getBottom() - 8, 3, 8));
    };

    auto drawCurve = [&](juce::Rectangle<int> cell, juce::Colour col)
    {
        auto c = cell.withSizeKeepingCentre(22, 16).toFloat();
        juce::Path p;
        p.startNewSubPath(c.getX(), c.getCentreY());
        p.quadraticTo(c.getCentreX() - 4.0f, c.getY(),
                      c.getCentreX(),        c.getCentreY());
        p.quadraticTo(c.getCentreX() + 4.0f, c.getBottom(),
                      c.getRight(),          c.getCentreY());
        g.setColour(col);
        g.strokePath(p, juce::PathStrokeType(1.5f));
    };

    auto drawGear = [&](juce::Rectangle<int> cell, juce::Colour col)
    {
        auto c = cell.withSizeKeepingCentre(18, 18).toFloat();
        g.setColour(col);
        g.drawEllipse(c, 1.5f);
        g.fillEllipse(c.withSizeKeepingCentre(5.0f, 5.0f));
        for (int i = 0; i < 6; ++i)
        {
            float a = (float) i * juce::MathConstants<float>::pi / 3.0f;
            float x1 = c.getCentreX() + std::cos(a) * 7.0f;
            float y1 = c.getCentreY() + std::sin(a) * 7.0f;
            float x2 = c.getCentreX() + std::cos(a) * 10.0f;
            float y2 = c.getCentreY() + std::sin(a) * 10.0f;
            g.drawLine(x1, y1, x2, y2, 1.5f);
        }
    };

    auto drawPower = [&](juce::Rectangle<int> cell, juce::Colour col)
    {
        auto c = cell.withSizeKeepingCentre(18, 18).toFloat();
        g.setColour(col);
        juce::Path arc;
        arc.addCentredArc(c.getCentreX(), c.getCentreY(), 8.0f, 8.0f, 0.0f,
                          -2.4f, 2.4f, true);
        g.strokePath(arc, juce::PathStrokeType(1.5f));
        g.drawLine(c.getCentreX(), c.getY() - 1.0f,
                   c.getCentreX(), c.getCentreY() - 1.0f, 1.5f);
    };

    // First (meters) = active cyan
    auto cell0 = juce::Rectangle<int>(bounds.getX(),           bounds.getY(), cellW, bounds.getHeight());
    auto cell1 = juce::Rectangle<int>(bounds.getX() + cellW,   bounds.getY(), cellW, bounds.getHeight());
    auto cell2 = juce::Rectangle<int>(bounds.getX() + cellW*2, bounds.getY(), cellW, bounds.getHeight());
    auto cell3 = juce::Rectangle<int>(bounds.getX() + cellW*3, bounds.getY(), cellW, bounds.getHeight());

    drawBars(cell0,  Colours::Cyan);
    drawCurve(cell1, Colours::TextDim);
    drawGear(cell2,  Colours::TextDim);
    drawPower(cell3, Colours::TextDim);
}
