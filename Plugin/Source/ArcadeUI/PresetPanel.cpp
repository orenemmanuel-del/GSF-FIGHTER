/*
  ==============================================================================
   GSF FIGHTER - Preset Panel Implementation
  ==============================================================================
*/

#include "PresetPanel.h"

namespace gsf::ui
{

PresetPanel::PresetPanel()
{
    presetSlots = {{
        { gsf::PresetID::Flat,           "FLAT",          "BYPASS",    Colours::LightGrey,  "X" },
        { gsf::PresetID::iPhoneSpeaker,  "iPHONE",        "RYU",      Colours::Blue,       "R" },
        { gsf::PresetID::AirPodsPro,     "AIRPODS",       "CHUN-LI",  Colours::Purple,     "C" },
        { gsf::PresetID::Voiture,        "VOITURE",       "GUILE",    Colours::Green,      "G" },
        { gsf::PresetID::ClubSystem,     "CLUB",          "AKUMA",    Colours::Red,        "A" },
        { gsf::PresetID::CheapEarbuds,   "CHEAP",         "BLANKA",   Colours::Yellow,     "B" },
        { gsf::PresetID::StudioMonitors, "STUDIO",        "SAGAT",    Colours::Orange,     "S" },
    }};

    for (int i = 0; i < 7; ++i)
    {
        auto& slot = presetSlots[i];
        auto btn = std::make_unique<ArcadeButton>(slot.shortName, ArcadeButton::Style::Round);
        btn->setClickingTogglesState(false);

        const int idx = i;
        btn->onClick = [this, idx]()
        {
            activePreset = presetSlots[idx].id;
            updateButtonStates();
            if (onPresetChanged)
                onPresetChanged(activePreset);
        };

        addAndMakeVisible(*btn);
        presetButtons[i] = std::move(btn);
    }

    updateButtonStates();
}

void PresetPanel::setActivePreset(gsf::PresetID preset)
{
    activePreset = preset;
    updateButtonStates();
    repaint();
}

void PresetPanel::updateButtonStates()
{
    for (int i = 0; i < 7; ++i)
    {
        bool isActive = presetSlots[i].id == activePreset;
        presetButtons[i]->setToggleState(isActive, juce::dontSendNotification);
    }
    repaint();
}

void PresetPanel::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();

    // Background
    g.setColour(Colours::DarkGrey);
    g.fillRoundedRectangle(bounds.toFloat(), 5.0f);
    GSFLookAndFeel::drawArcadeBorder(g, bounds, Colours::Blue);

    // Header
    auto header = bounds.removeFromTop(32).reduced(8, 4);
    g.setFont(juce::Font(juce::FontOptions(18.0f, juce::Font::bold)));
    g.setColour(Colours::Yellow);
    g.drawFittedText("SELECT YOUR FIGHTER", header, juce::Justification::centred, 1);

    // Active preset info
    auto infoArea = bounds.removeFromBottom(50).reduced(8, 4);
    g.setFont(juce::Font(juce::FontOptions(14.0f, juce::Font::bold)));

    // Find active slot
    for (const auto& slot : presetSlots)
    {
        if (slot.id == activePreset)
        {
            g.setColour(slot.colour);
            g.drawFittedText("ACTIVE: " + slot.name, infoArea.removeFromTop(20),
                             juce::Justification::centred, 1);
            g.setColour(Colours::TextDim);
            g.setFont(juce::Font(juce::FontOptions(11.0f, juce::Font::bold)));
            g.drawFittedText(gsf::presetName(activePreset), infoArea,
                             juce::Justification::centred, 1);
            break;
        }
    }

    // Draw fighter silhouettes for each preset
    auto gridArea = bounds.reduced(8, 4);
    int cols = 4;
    int rows = 2;
    int cellW = gridArea.getWidth() / cols;
    int cellH = gridArea.getHeight() / rows;

    for (int i = 0; i < 7; ++i)
    {
        int col = i % cols;
        int row = i / cols;
        auto cell = juce::Rectangle<int>(
            gridArea.getX() + col * cellW,
            gridArea.getY() + row * cellH,
            cellW, cellH).reduced(2);

        bool isActive = presetSlots[i].id == activePreset;
        drawFighterSilhouette(g, cell.removeFromTop(cell.getHeight() - 40), i, isActive);
    }
}

void PresetPanel::resized()
{
    auto bounds = getLocalBounds();
    bounds.removeFromTop(32);  // Header
    bounds.removeFromBottom(50); // Info

    auto gridArea = bounds.reduced(8, 4);
    int cols = 4;
    int rows = 2;
    int cellW = gridArea.getWidth() / cols;
    int cellH = gridArea.getHeight() / rows;

    for (int i = 0; i < 7; ++i)
    {
        int col = i % cols;
        int row = i / cols;
        auto cell = juce::Rectangle<int>(
            gridArea.getX() + col * cellW,
            gridArea.getY() + row * cellH,
            cellW, cellH).reduced(4);

        // Button at bottom of cell
        presetButtons[i]->setBounds(cell.removeFromBottom(36));
    }
}

void PresetPanel::drawFighterSilhouette(juce::Graphics& g, juce::Rectangle<int> bounds,
                                         int presetIndex, bool isActive)
{
    auto& slot = presetSlots[presetIndex];
    auto centre = bounds.getCentre().toFloat();
    float size = juce::jmin(bounds.getWidth(), bounds.getHeight()) * 0.6f;

    // Fighter silhouette (simplified geometric representation)
    juce::Path silhouette;

    // Different pose for each fighter
    switch (presetIndex)
    {
        case 0: // FLAT - X mark (bypass)
            silhouette.addLineSegment(juce::Line<float>(
                centre.x - size * 0.3f, centre.y - size * 0.3f,
                centre.x + size * 0.3f, centre.y + size * 0.3f), 3.0f);
            silhouette.addLineSegment(juce::Line<float>(
                centre.x + size * 0.3f, centre.y - size * 0.3f,
                centre.x - size * 0.3f, centre.y + size * 0.3f), 3.0f);
            break;

        case 1: // iPhone - hadouken pose
        case 2: // AirPods - kick pose
        case 3: // Voiture - guard pose
        case 4: // Club - power pose
        case 5: // Cheap - crouch pose
        case 6: // Studio - standing pose
        {
            // Head
            float headY = centre.y - size * 0.35f;
            silhouette.addEllipse(centre.x - size * 0.1f, headY - size * 0.1f,
                                  size * 0.2f, size * 0.2f);
            // Body
            silhouette.addRectangle(centre.x - size * 0.08f, headY + size * 0.1f,
                                    size * 0.16f, size * 0.35f);
            // Arms (different poses)
            float armY = headY + size * 0.15f;
            if (presetIndex == 1 || presetIndex == 4)
            {
                // Arms extended forward (hadouken/power)
                silhouette.addRectangle(centre.x + size * 0.08f, armY,
                                        size * 0.3f, size * 0.06f);
                silhouette.addRectangle(centre.x - size * 0.08f - size * 0.15f, armY + size * 0.08f,
                                        size * 0.15f, size * 0.06f);
            }
            else
            {
                // Arms at sides
                silhouette.addRectangle(centre.x + size * 0.08f, armY,
                                        size * 0.06f, size * 0.25f);
                silhouette.addRectangle(centre.x - size * 0.14f, armY,
                                        size * 0.06f, size * 0.25f);
            }
            // Legs
            float legY = headY + size * 0.45f;
            if (presetIndex == 2)
            {
                // Kick pose
                silhouette.addRectangle(centre.x - size * 0.06f, legY,
                                        size * 0.06f, size * 0.3f);
                silhouette.addRectangle(centre.x + size * 0.02f, legY,
                                        size * 0.3f, size * 0.06f);
            }
            else
            {
                silhouette.addRectangle(centre.x - size * 0.1f, legY,
                                        size * 0.08f, size * 0.3f);
                silhouette.addRectangle(centre.x + size * 0.02f, legY,
                                        size * 0.08f, size * 0.3f);
            }
            break;
        }
    }

    // Draw silhouette
    auto drawColour = isActive ? slot.colour : slot.colour.withAlpha(0.3f);
    g.setColour(drawColour);
    g.fillPath(silhouette);

    if (isActive)
    {
        // Glow around silhouette
        g.setColour(slot.colour.withAlpha(0.15f));
        g.strokePath(silhouette, juce::PathStrokeType(4.0f));
    }

    // Name label
    g.setFont(juce::Font(juce::FontOptions(9.0f, juce::Font::bold)));
    g.setColour(isActive ? Colours::White : Colours::TextDim);
    g.drawFittedText(slot.name,
                     juce::Rectangle<int>(bounds.getX(), bounds.getBottom() - 14,
                                           bounds.getWidth(), 14),
                     juce::Justification::centred, 1);
}

} // namespace gsf::ui
