/*
  ==============================================================================
   GSF FIGHTER — Plugin Editor (Oscilloscope Design)
   Layout (900x500):
     ┌─────────────────────────────────────────────┐
     │          HEADER (44px)                      │
     ├──────────┬──────────────────────────────────┤
     │          │  DEVICE STATUS   STREAM QUALITY  │
     │ SIDEBAR  ├──────────────────────────────────┤
     │ (200px)  │        VU METERS L/R             │
     │          ├──────────────────────────────────┤
     │          │  PORT │ BUFFER │ DROPOUTS        │
     ├──────────┴──────────────────────────────────┤
     │          BOTTOM NAV (44px)                  │
     └─────────────────────────────────────────────┘
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

    gsf::ui::GSFLookAndFeel oscLookAndFeel;

    // Main content panels
    gsf::ui::ConnectionPanel inputMatrixPanel;
    gsf::ui::PresetPanel     sidebarPanel;
    gsf::ui::FightScreen     overlayScreen;

    // Animation
    float wifiPhase = 0.0f;

    // Drawing helpers
    void drawHeader(juce::Graphics& g, juce::Rectangle<int> bounds);
    void drawBottomNav(juce::Graphics& g, juce::Rectangle<int> bounds);

    static constexpr int kPluginWidth   = 900;
    static constexpr int kPluginHeight  = 500;
    static constexpr int kHeaderHeight  = 44;
    static constexpr int kSidebarWidth  = 200;
    static constexpr int kFooterHeight  = 44;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GSFFighterEditor)
};
