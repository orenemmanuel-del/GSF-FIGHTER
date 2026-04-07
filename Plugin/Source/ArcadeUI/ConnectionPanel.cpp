/*
  ==============================================================================
   GSF FIGHTER - Connection Panel Implementation
  ==============================================================================
*/

#include "ConnectionPanel.h"
#include <cmath>

namespace gsf::ui
{

ConnectionPanel::ConnectionPanel()
{
    addAndMakeVisible(startStopButton);
    addAndMakeVisible(regenButton);
    addAndMakeVisible(meterL);
    addAndMakeVisible(meterR);

    startStopButton.setClickingTogglesState(true);
    startStopButton.onClick = [this]()
    {
        isStreaming = startStopButton.getToggleState();
        startStopButton.setButtonText(isStreaming ? "K.O.!" : "FIGHT!");
        startStopButton.setStyle(isStreaming ? ArcadeButton::Style::KO : ArcadeButton::Style::Hadouken);
        if (onStartStop) onStartStop();
    };

    regenButton.onClick = [this]()
    {
        if (onRegenCode) onRegenCode();
    };

    startTimerHz(30);
}

void ConnectionPanel::setConnectionCode(const std::array<uint8_t, 4>& code)
{
    connectionCode = code;
    repaint();
}

void ConnectionPanel::setIPAddress(const juce::String& ip)
{
    ipAddress = ip;
    repaint();
}

void ConnectionPanel::setClientCount(int count)
{
    clientCount = count;
    repaint();
}

void ConnectionPanel::setStreaming(bool streaming)
{
    isStreaming = streaming;
    startStopButton.setToggleState(streaming, juce::dontSendNotification);
    startStopButton.setButtonText(streaming ? "K.O.!" : "FIGHT!");
    startStopButton.setStyle(streaming ? ArcadeButton::Style::KO : ArcadeButton::Style::Hadouken);
    repaint();
}

void ConnectionPanel::setLevels(float peakL, float peakR, float rmsL, float rmsR)
{
    meterL.setLevel(peakL, rmsL);
    meterR.setLevel(peakR, rmsR);
}

void ConnectionPanel::timerCallback()
{
    fightGlowPhase += 0.08f;
    if (isStreaming)
        repaint();
}

void ConnectionPanel::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();

    // Background
    g.setColour(Colours::DarkGrey);
    g.fillRoundedRectangle(bounds.toFloat(), 5.0f);

    // Border with glow when streaming
    if (isStreaming)
    {
        float glowIntensity = 0.3f + 0.15f * std::sin(fightGlowPhase);
        GSFLookAndFeel::drawGlow(g, bounds.toFloat(), Colours::Red.withAlpha(glowIntensity), 10.0f);
    }
    GSFLookAndFeel::drawArcadeBorder(g, bounds, isStreaming ? Colours::Red : Colours::LightGrey);

    // --- HEADER: "ROUND 1 - READY?" or "FIGHT!" ---
    auto headerArea = bounds.removeFromTop(40).reduced(8, 4);
    g.setFont(juce::Font(juce::FontOptions(22.0f, juce::Font::bold)));

    if (isStreaming)
    {
        g.setColour(Colours::Red);
        g.drawFittedText("STREAMING", headerArea.removeFromLeft(headerArea.getWidth() / 2),
                         juce::Justification::centredLeft, 1);
        g.setColour(Colours::Yellow);
        g.drawFittedText(juce::String(clientCount) + " FIGHTER" + (clientCount != 1 ? "S" : ""),
                         headerArea, juce::Justification::centredRight, 1);
    }
    else
    {
        g.setColour(Colours::Blue);
        g.drawFittedText("READY?", headerArea, juce::Justification::centred, 1);
    }

    // --- CONNECTION CODE ---
    auto codeArea = bounds.removeFromTop(70).reduced(8, 4);
    g.setFont(juce::Font(juce::FontOptions(12.0f, juce::Font::bold)));
    g.setColour(Colours::TextDim);
    g.drawFittedText("CONNECTION CODE", codeArea.removeFromTop(16),
                     juce::Justification::centred, 1);

    // Draw big code digits
    auto digitArea = codeArea.reduced(20, 0);
    int digitWidth = digitArea.getWidth() / 4;

    g.setFont(juce::Font(juce::FontOptions(36.0f, juce::Font::bold)));
    for (int i = 0; i < 4; ++i)
    {
        auto digitRect = digitArea.removeFromLeft(digitWidth);
        // Digit background
        auto digitBg = digitRect.reduced(4, 2);
        g.setColour(Colours::Black);
        g.fillRoundedRectangle(digitBg.toFloat(), 4.0f);
        g.setColour(Colours::LightGrey);
        g.drawRoundedRectangle(digitBg.toFloat(), 4.0f, 1.0f);

        // Digit text
        g.setColour(Colours::Yellow);
        g.drawFittedText(juce::String(static_cast<int>(connectionCode[i])),
                         digitBg, juce::Justification::centred, 1);
    }

    // --- IP ADDRESS ---
    auto ipArea = bounds.removeFromTop(22).reduced(8, 0);
    g.setFont(juce::Font(juce::FontOptions(11.0f, juce::Font::bold)));
    g.setColour(Colours::TextDim);
    g.drawFittedText("IP: " + ipAddress, ipArea, juce::Justification::centred, 1);
}

void ConnectionPanel::resized()
{
    auto bounds = getLocalBounds();

    // Reserve bottom for buttons and meters
    auto bottomArea = bounds.removeFromBottom(120);

    // Meters
    auto meterArea = bottomArea.removeFromTop(30).reduced(8, 2);
    meterL.setBounds(meterArea.removeFromTop(14));
    meterR.setBounds(meterArea);

    // Buttons
    auto buttonArea = bottomArea.reduced(8, 4);
    auto startArea = buttonArea.removeFromTop(50);
    startStopButton.setBounds(startArea.reduced(4));

    auto regenArea = buttonArea.removeFromTop(36);
    regenButton.setBounds(regenArea.reduced(20, 2));
}

} // namespace gsf::ui
