/*
  ==============================================================================
   GSF FIGHTER — Sidebar Navigation Panel (Oscilloscope Design)
   Left sidebar: ENGINE ALPHA block + 5 navigation items.
  ==============================================================================
*/

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "GSFLookAndFeel.h"
#include "GSFProtocol.h"

namespace gsf::ui
{

class PresetPanel : public juce::Component
{
public:
    PresetPanel();
    ~PresetPanel() override = default;

    void paint(juce::Graphics& g) override;
    void resized() override;
    void mouseMove(const juce::MouseEvent&) override;
    void mouseExit(const juce::MouseEvent&) override;
    void mouseDown(const juce::MouseEvent&) override;

    void setActivePreset(gsf::PresetID preset);
    gsf::PresetID getActivePreset() const { return activePreset; }

    std::function<void(gsf::PresetID)> onPresetChanged;

private:
    struct NavItem
    {
        juce::String label;
        gsf::PresetID associatedPreset;
    };

    std::array<NavItem, 5> navItems;

    gsf::PresetID activePreset = gsf::PresetID::Flat;
    int selectedIndex = 0;
    int hoverIndex    = -1;

    juce::Rectangle<int> headerArea;
    std::array<juce::Rectangle<int>, 5> itemBounds;

    void drawHeader(juce::Graphics& g);
    void drawNavItem(juce::Graphics& g, int index);
    void drawNavIcon(juce::Graphics& g, juce::Rectangle<int> area,
                     int index, juce::Colour colour);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetPanel)
};

} // namespace gsf::ui
