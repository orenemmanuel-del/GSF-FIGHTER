/*
  ==============================================================================
   GSF FIGHTER - Preset Panel
   Select translation presets with fighter-style character selection UI.
  ==============================================================================
*/

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "GSFLookAndFeel.h"
#include "ArcadeButton.h"
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

    void setActivePreset(gsf::PresetID preset);
    gsf::PresetID getActivePreset() const { return activePreset; }

    std::function<void(gsf::PresetID)> onPresetChanged;

private:
    gsf::PresetID activePreset = gsf::PresetID::Flat;

    struct PresetSlot
    {
        gsf::PresetID id;
        juce::String  name;
        juce::String  shortName;   // Fighter name
        juce::Colour  colour;
        juce::String  silhouette;  // Character identifier
    };

    std::array<PresetSlot, 7> presetSlots;
    std::array<std::unique_ptr<ArcadeButton>, 7> presetButtons;

    void updateButtonStates();

    // Character silhouette drawing
    void drawFighterSilhouette(juce::Graphics& g, juce::Rectangle<int> bounds,
                                int presetIndex, bool isActive);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetPanel)
};

} // namespace gsf::ui
