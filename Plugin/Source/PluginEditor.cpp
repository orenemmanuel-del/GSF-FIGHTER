/*
  ==============================================================================
   GSF FIGHTER - Plugin Editor Implementation
  ==============================================================================
*/

#include "PluginEditor.h"
#include "Network/DeviceDiscovery.h"
#include <cmath>

GSFFighterEditor::GSFFighterEditor(GSFFighterProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    setLookAndFeel(&arcadeLookAndFeel);
    setSize(kPluginWidth, kPluginHeight);

    // Connection panel
    addAndMakeVisible(connectionPanel);
    connectionPanel.setConnectionCode(processorRef.getConnectionCode());
    connectionPanel.setIPAddress(DeviceDiscovery::getLocalIPAddress());
    connectionPanel.setStreaming(processorRef.isStreaming());

    connectionPanel.onStartStop = [this]()
    {
        bool shouldStream = !processorRef.isStreaming();
        processorRef.setStreaming(shouldStream);
        connectionPanel.setStreaming(shouldStream);

        if (shouldStream)
            fightScreen.triggerFightSequence();
        else
            fightScreen.triggerKOSequence();
    };

    connectionPanel.onRegenCode = [this]()
    {
        processorRef.regenerateConnectionCode();
        connectionPanel.setConnectionCode(processorRef.getConnectionCode());
    };

    // Preset panel
    addAndMakeVisible(presetPanel);
    presetPanel.setActivePreset(processorRef.getTranslationEngine().getCurrentPreset());

    presetPanel.onPresetChanged = [this](gsf::PresetID preset)
    {
        processorRef.getTranslationEngine().setPreset(preset);
    };

    // Fight screen overlay (on top)
    addAndMakeVisible(fightScreen);
    fightScreen.setVisible(false);

    // Status label
    addAndMakeVisible(statusLabel);
    statusLabel.setColour(juce::Label::textColourId, gsf::ui::Colours::TextDim);
    statusLabel.setJustificationType(juce::Justification::centred);

    // Start UI timer
    startTimerHz(30);
}

GSFFighterEditor::~GSFFighterEditor()
{
    setLookAndFeel(nullptr);
}

void GSFFighterEditor::timerCallback()
{
    // Update metering
    connectionPanel.setLevels(
        processorRef.getPeakLevel(0), processorRef.getPeakLevel(1),
        processorRef.getRmsLevel(0),  processorRef.getRmsLevel(1));

    // Update client count
    connectionPanel.setClientCount(processorRef.getConnectedClientCount());

    // Animation
    logoGlowPhase += 0.06f;
    scanlineOffset = (scanlineOffset + 1) % 3;

    // Status text
    if (processorRef.isStreaming())
    {
        int clients = processorRef.getConnectedClientCount();
        auto presetName = gsf::presetName(processorRef.getTranslationEngine().getCurrentPreset());
        statusLabel.setText(
            juce::String("STREAMING // ") + juce::String(clients) + " CLIENT"
            + (clients != 1 ? "S" : "") + " // " + presetName,
            juce::dontSendNotification);
        statusLabel.setColour(juce::Label::textColourId, gsf::ui::Colours::Green);
    }
    else
    {
        statusLabel.setText("INSERT ON MASTER BUS // PRESS FIGHT! TO START",
                            juce::dontSendNotification);
        statusLabel.setColour(juce::Label::textColourId, gsf::ui::Colours::TextDim);
    }

    repaint();
}

void GSFFighterEditor::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();

    // Background
    drawBackground(g, bounds);

    // Logo header
    auto logoArea = bounds.removeFromTop(70);
    drawLogo(g, logoArea);

    // Status bar at bottom
    auto statusArea = bounds.removeFromBottom(28);
    drawStatusBar(g, statusArea);

    // Scanlines over everything
    gsf::ui::GSFLookAndFeel::drawScanlines(g, getLocalBounds(), 0.02f);
}

void GSFFighterEditor::resized()
{
    auto bounds = getLocalBounds();

    // Logo header
    bounds.removeFromTop(70);

    // Status bar
    auto statusArea = bounds.removeFromBottom(28);
    statusLabel.setBounds(statusArea.reduced(8, 2));

    // Main content area
    auto content = bounds.reduced(6, 4);

    // Left: Connection panel (40%)
    auto leftPanel = content.removeFromLeft(content.getWidth() * 4 / 10);
    connectionPanel.setBounds(leftPanel.reduced(2));

    // Right: Preset panel (60%)
    presetPanel.setBounds(content.reduced(2));

    // Fight screen overlay (full size)
    fightScreen.setBounds(getLocalBounds());
}

void GSFFighterEditor::drawBackground(juce::Graphics& g, juce::Rectangle<int> bounds)
{
    // Dark industrial gradient
    g.setGradientFill(juce::ColourGradient(
        gsf::ui::Colours::Black, bounds.getCentreX(), bounds.getY(),
        gsf::ui::Colours::DarkGrey.darker(0.5f), bounds.getCentreX(), bounds.getBottom(),
        false));
    g.fillRect(bounds);

    // Subtle grid pattern (industrial / arcade PCB)
    g.setColour(gsf::ui::Colours::MidGrey.withAlpha(0.06f));
    for (int x = bounds.getX(); x < bounds.getRight(); x += 20)
        g.drawVerticalLine(x, static_cast<float>(bounds.getY()), static_cast<float>(bounds.getBottom()));
    for (int y = bounds.getY(); y < bounds.getBottom(); y += 20)
        g.drawHorizontalLine(y, static_cast<float>(bounds.getX()), static_cast<float>(bounds.getRight()));

    // Corner decorations (arcade cabinet bolts)
    float boltSize = 8.0f;
    auto drawBolt = [&](float x, float y)
    {
        g.setColour(gsf::ui::Colours::LightGrey.withAlpha(0.3f));
        g.fillEllipse(x - boltSize / 2, y - boltSize / 2, boltSize, boltSize);
        g.setColour(gsf::ui::Colours::Black);
        g.drawEllipse(x - boltSize / 2, y - boltSize / 2, boltSize, boltSize, 1.0f);
        // Cross slot
        g.drawLine(x - 2, y, x + 2, y, 1.0f);
        g.drawLine(x, y - 2, x, y + 2, 1.0f);
    };

    float margin = 12.0f;
    drawBolt(bounds.getX() + margin, bounds.getY() + margin);
    drawBolt(bounds.getRight() - margin, bounds.getY() + margin);
    drawBolt(bounds.getX() + margin, bounds.getBottom() - margin);
    drawBolt(bounds.getRight() - margin, bounds.getBottom() - margin);
}

void GSFFighterEditor::drawLogo(juce::Graphics& g, juce::Rectangle<int> bounds)
{
    // Background bar
    g.setColour(gsf::ui::Colours::Black);
    g.fillRect(bounds);

    // Red accent line at bottom
    g.setColour(gsf::ui::Colours::Red);
    g.fillRect(bounds.getX(), bounds.getBottom() - 3, bounds.getWidth(), 3);

    // Logo glow
    float glowIntensity = 0.3f + 0.1f * std::sin(logoGlowPhase);
    gsf::ui::GSFLookAndFeel::drawGlow(g, bounds.toFloat().reduced(4),
                                       gsf::ui::Colours::Red.withAlpha(glowIntensity), 8.0f);

    // Main title
    auto titleArea = bounds.reduced(10, 5);

    // "GSF" in red
    g.setFont(juce::Font(juce::FontOptions(42.0f, juce::Font::bold)));
    auto gsfArea = titleArea.removeFromLeft(120);

    // Shadow
    g.setColour(gsf::ui::Colours::Black);
    g.drawFittedText("GSF", gsfArea.translated(2, 2), juce::Justification::centredRight, 1);

    // Red text with yellow border effect
    g.setColour(gsf::ui::Colours::Red);
    g.drawFittedText("GSF", gsfArea, juce::Justification::centredRight, 1);

    // "FIGHTER" in yellow
    auto fighterArea = titleArea.removeFromLeft(220);
    g.setFont(juce::Font(juce::FontOptions(42.0f, juce::Font::bold)));

    g.setColour(gsf::ui::Colours::Black);
    g.drawFittedText("FIGHTER", fighterArea.translated(2, 2), juce::Justification::centredLeft, 1);

    g.setColour(gsf::ui::Colours::Yellow);
    g.drawFittedText("FIGHTER", fighterArea, juce::Justification::centredLeft, 1);

    // Subtitle / version
    g.setFont(juce::Font(juce::FontOptions(11.0f, juce::Font::bold)));
    g.setColour(gsf::ui::Colours::Blue);
    g.drawFittedText("MASTER BUS STREAMING SYSTEM  v1.0",
                     titleArea.reduced(4), juce::Justification::centredRight, 1);

    // "x GESAFFELSTEIN" attribution
    g.setFont(juce::Font(juce::FontOptions(9.0f, juce::Font::bold)));
    g.setColour(gsf::ui::Colours::TextDim);
    g.drawFittedText("x GESAFFELSTEIN",
                     bounds.removeFromBottom(14).reduced(10, 0),
                     juce::Justification::centredRight, 1);
}

void GSFFighterEditor::drawStatusBar(juce::Graphics& g, juce::Rectangle<int> bounds)
{
    // Dark bar
    g.setColour(gsf::ui::Colours::Black.brighter(0.03f));
    g.fillRect(bounds);

    // Top accent line
    g.setColour(gsf::ui::Colours::LightGrey.withAlpha(0.3f));
    g.drawHorizontalLine(bounds.getY(), static_cast<float>(bounds.getX()),
                         static_cast<float>(bounds.getRight()));

    // Blinking dot for streaming
    if (processorRef.isStreaming())
    {
        float dotAlpha = 0.5f + 0.5f * std::sin(logoGlowPhase * 2.0f);
        g.setColour(gsf::ui::Colours::Red.withAlpha(dotAlpha));
        g.fillEllipse(bounds.getX() + 10.0f, bounds.getCentreY() - 4.0f, 8.0f, 8.0f);
    }
}
