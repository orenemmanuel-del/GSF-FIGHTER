/*
  ==============================================================================
   GSF FIGHTER - Plugin Editor
   Main plugin UI: Street Fighter arcade aesthetic.
   Layout:
     ┌────────────────────────────────────┐
     │       GSF FIGHTER (logo)           │
     ├──────────────┬─────────────────────┤
     │  CONNECTION  │   PRESET SELECT     │
     │   PANEL      │   (Fighter Grid)    │
     │  Code: 1234  │                     │
     │  [FIGHT!]    │                     │
     │  ═══ L ═══   │                     │
     │  ═══ R ═══   │                     │
     ├──────────────┴─────────────────────┤
     │         SCANLINES + STATUS         │
     └────────────────────────────────────┘
  ==============================================================================
*/

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"
#include "ArcadeUI/GSFLookAndFeel.h"
#include "ArcadeUI/ConnectionPanel.h"
#include "ArcadeUI/PresetPanel.h"
#include "ArcadeUI/FightScreen.h"

class GSFFighterEditor : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    explicit GSFFighterEditor(GSFFighterProcessor& processor);
    ~GSFFighterEditor() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    void timerCallback() override;

private:
    GSFFighterProcessor& processorRef;

    // Look & Feel
    gsf::ui::GSFLookAndFeel arcadeLookAndFeel;

    // UI Panels
    gsf::ui::ConnectionPanel connectionPanel;
    gsf::ui::PresetPanel     presetPanel;
    gsf::ui::FightScreen     fightScreen;

    // Status bar
    juce::Label statusLabel;

    // Animation
    float logoGlowPhase = 0.0f;
    int scanlineOffset = 0;

    // Drawing helpers
    void drawLogo(juce::Graphics& g, juce::Rectangle<int> bounds);
    void drawBackground(juce::Graphics& g, juce::Rectangle<int> bounds);
    void drawStatusBar(juce::Graphics& g, juce::Rectangle<int> bounds);

    static constexpr int kPluginWidth  = 700;
    static constexpr int kPluginHeight = 520;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GSFFighterEditor)
};
