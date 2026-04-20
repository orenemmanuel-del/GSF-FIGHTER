/*
  ==============================================================================
   GSF FIGHTER — Sidebar Navigation Panel Implementation
  ==============================================================================
*/

#include "PresetPanel.h"
#include <cmath>

namespace gsf::ui
{

PresetPanel::PresetPanel()
{
    navItems = {{
        { "INPUT MATRIX",    gsf::PresetID::Flat          },
        { "OUTPUT ROUTING",  gsf::PresetID::iPhoneSpeaker  },
        { "CLOCK SOURCE",    gsf::PresetID::AirPodsPro    },
        { "BUFFER SIZE",     gsf::PresetID::Voiture       },
        { "NETWORK SYNC",    gsf::PresetID::StudioMonitors },
    }};
    setMouseCursor(juce::MouseCursor::PointingHandCursor);
}

void PresetPanel::setActivePreset(gsf::PresetID preset)
{
    activePreset = preset;
    for (size_t i = 0; i < navItems.size(); ++i)
    {
        if (navItems[i].associatedPreset == preset)
        {
            selectedIndex = (int) i;
            break;
        }
    }
    repaint();
}

void PresetPanel::resized()
{
    auto bounds = getLocalBounds();
    headerArea = bounds.removeFromTop(80);

    const int itemH = 40;
    bounds.removeFromTop(4);
    for (size_t i = 0; i < navItems.size(); ++i)
    {
        itemBounds[i] = bounds.removeFromTop(itemH);
    }
}

void PresetPanel::mouseMove(const juce::MouseEvent& e)
{
    int newHover = -1;
    for (size_t i = 0; i < itemBounds.size(); ++i)
    {
        if (itemBounds[i].contains(e.getPosition()))
        {
            newHover = (int) i;
            break;
        }
    }
    if (newHover != hoverIndex)
    {
        hoverIndex = newHover;
        repaint();
    }
}

void PresetPanel::mouseExit(const juce::MouseEvent&)
{
    if (hoverIndex != -1)
    {
        hoverIndex = -1;
        repaint();
    }
}

void PresetPanel::mouseDown(const juce::MouseEvent& e)
{
    for (size_t i = 0; i < itemBounds.size(); ++i)
    {
        if (itemBounds[i].contains(e.getPosition()))
        {
            selectedIndex = (int) i;
            activePreset = navItems[i].associatedPreset;
            if (onPresetChanged) onPresetChanged(activePreset);
            repaint();
            return;
        }
    }
}

void PresetPanel::paint(juce::Graphics& g)
{
    drawHeader(g);

    for (int i = 0; i < (int) navItems.size(); ++i)
        drawNavItem(g, i);
}

void PresetPanel::drawHeader(juce::Graphics& g)
{
    auto area = headerArea.reduced(18, 14);

    // "ENGINE ALPHA" dim label
    g.setColour(Colours::TextDim);
    g.setFont(juce::Font(juce::FontOptions(juce::Font::getDefaultMonospacedFontName(),
                                           9.0f, juce::Font::plain)));
    auto engineLine = area.removeFromTop(12);
    g.drawFittedText("ENGINE ALPHA", engineLine, juce::Justification::centredLeft, 1);
    area.removeFromTop(4);

    // "MONITOR_01" big title
    g.setColour(Colours::White);
    g.setFont(juce::Font(juce::FontOptions(juce::Font::getDefaultMonospacedFontName(),
                                           16.0f, juce::Font::plain)));
    auto titleLine = area.removeFromTop(20);
    g.drawFittedText("MONITOR_01", titleLine, juce::Justification::centredLeft, 1);
    area.removeFromTop(2);

    // "48KHZ / 24-BIT" cyan subtitle
    g.setColour(Colours::Cyan);
    g.setFont(juce::Font(juce::FontOptions(juce::Font::getDefaultMonospacedFontName(),
                                           9.0f, juce::Font::plain)));
    g.drawFittedText("48KHZ / 24-BIT", area.removeFromTop(12),
                     juce::Justification::centredLeft, 1);

    // Bottom divider
    g.setColour(Colours::BorderSubtle);
    g.drawHorizontalLine(headerArea.getBottom() - 1,
                         (float) headerArea.getX() + 14.0f,
                         (float) headerArea.getRight() - 14.0f);
}

void PresetPanel::drawNavItem(juce::Graphics& g, int index)
{
    auto area = itemBounds[(size_t) index];
    const bool isSelected = (index == selectedIndex);
    const bool isHover    = (index == hoverIndex) && !isSelected;

    // Background
    if (isSelected)
    {
        g.setColour(Colours::Cyan);
        g.fillRect(area);
    }
    else if (isHover)
    {
        g.setColour(Colours::LightGrey);
        g.fillRect(area);
    }

    auto iconColour = isSelected
                          ? Colours::Black
                          : (isHover ? Colours::White : Colours::TextDim);

    auto textColour = iconColour;

    // Icon zone (left 44 px)
    auto iconArea = area.withWidth(44);
    drawNavIcon(g, iconArea.reduced(12), index, iconColour);

    // Label
    g.setColour(textColour);
    g.setFont(juce::Font(juce::FontOptions(juce::Font::getDefaultMonospacedFontName(),
                                           9.0f,
                                           isSelected ? juce::Font::bold : juce::Font::plain)));
    auto labelArea = area.withTrimmedLeft(44).withTrimmedRight(12);
    g.drawFittedText(navItems[(size_t) index].label, labelArea,
                     juce::Justification::centredLeft, 1);
}

void PresetPanel::drawNavIcon(juce::Graphics& g, juce::Rectangle<int> area,
                              int index, juce::Colour colour)
{
    g.setColour(colour);
    auto c = area.toFloat();

    switch (index)
    {
        case 0: // INPUT MATRIX: 4 small squares
        {
            float size = 5.0f;
            float gap  = 3.0f;
            float total = 2.0f * size + gap;
            float x0 = c.getCentreX() - total * 0.5f;
            float y0 = c.getCentreY() - total * 0.5f;
            for (int row = 0; row < 2; ++row)
                for (int col = 0; col < 2; ++col)
                    g.fillRect(juce::Rectangle<float>(x0 + col * (size + gap),
                                                      y0 + row * (size + gap),
                                                      size, size));
            break;
        }
        case 1: // OUTPUT ROUTING: arrows
        {
            juce::Path arrow;
            float cx = c.getCentreX();
            float cy = c.getCentreY();
            arrow.addLineSegment(juce::Line<float>(cx - 6.0f, cy, cx + 6.0f, cy), 1.3f);
            arrow.addLineSegment(juce::Line<float>(cx + 3.0f, cy - 3.0f, cx + 6.0f, cy), 1.3f);
            arrow.addLineSegment(juce::Line<float>(cx + 3.0f, cy + 3.0f, cx + 6.0f, cy), 1.3f);
            g.strokePath(arrow, juce::PathStrokeType(1.3f));
            break;
        }
        case 2: // CLOCK SOURCE: circle with hands
        {
            float r = 7.0f;
            g.drawEllipse(c.getCentreX() - r, c.getCentreY() - r, r * 2.0f, r * 2.0f, 1.3f);
            g.drawLine(c.getCentreX(), c.getCentreY(),
                       c.getCentreX(), c.getCentreY() - r * 0.6f, 1.3f);
            g.drawLine(c.getCentreX(), c.getCentreY(),
                       c.getCentreX() + r * 0.5f, c.getCentreY(), 1.3f);
            break;
        }
        case 3: // BUFFER SIZE: gear
        {
            float r = 6.0f;
            g.drawEllipse(c.getCentreX() - r, c.getCentreY() - r, r * 2.0f, r * 2.0f, 1.3f);
            for (int i = 0; i < 6; ++i)
            {
                float a = (float) i * juce::MathConstants<float>::pi / 3.0f;
                float x1 = c.getCentreX() + std::cos(a) * (r + 1.0f);
                float y1 = c.getCentreY() + std::sin(a) * (r + 1.0f);
                float x2 = c.getCentreX() + std::cos(a) * (r + 3.0f);
                float y2 = c.getCentreY() + std::sin(a) * (r + 3.0f);
                g.drawLine(x1, y1, x2, y2, 1.3f);
            }
            break;
        }
        case 4: // NETWORK SYNC: nodes
        {
            g.fillEllipse(c.getCentreX() - 7.0f, c.getCentreY() - 2.0f, 3.0f, 3.0f);
            g.fillEllipse(c.getCentreX() + 4.0f, c.getCentreY() - 5.0f, 3.0f, 3.0f);
            g.fillEllipse(c.getCentreX() + 4.0f, c.getCentreY() + 3.0f, 3.0f, 3.0f);
            g.drawLine(c.getCentreX() - 5.5f, c.getCentreY() - 0.5f,
                       c.getCentreX() + 5.5f, c.getCentreY() - 3.5f, 1.0f);
            g.drawLine(c.getCentreX() - 5.5f, c.getCentreY() - 0.5f,
                       c.getCentreX() + 5.5f, c.getCentreY() + 4.5f, 1.0f);
            break;
        }
        default: break;
    }
}

} // namespace gsf::ui
