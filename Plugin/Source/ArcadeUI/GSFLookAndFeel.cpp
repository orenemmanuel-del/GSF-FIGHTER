/*
  ==============================================================================
   GSF FIGHTER — Carbon Look & Feel Implementation
  ==============================================================================
*/

#include "GSFLookAndFeel.h"
#include <cmath>

namespace gsf::ui
{

GSFLookAndFeel::GSFLookAndFeel()
{
    setColour(juce::ResizableWindow::backgroundColourId, Colours::Black);
    setColour(juce::TextButton::buttonColourId,    Colours::MidGrey);
    setColour(juce::TextButton::buttonOnColourId,  Colours::Cyan);
    setColour(juce::TextButton::textColourOffId,   Colours::TextSecondary);
    setColour(juce::TextButton::textColourOnId,    Colours::Black);
    setColour(juce::Label::textColourId,           Colours::White);
    setColour(juce::ComboBox::backgroundColourId,  Colours::MidGrey);
    setColour(juce::ComboBox::textColourId,        Colours::White);
    setColour(juce::ComboBox::outlineColourId,     Colours::BorderSubtle);
}

juce::Font GSFLookAndFeel::getMonoFont(float height, bool bold) const
{
    juce::FontOptions opts { juce::Font::getDefaultMonospacedFontName(), height,
                             bold ? juce::Font::bold : juce::Font::plain };
    return juce::Font(opts);
}

void GSFLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button,
                                          const juce::Colour& backgroundColour,
                                          bool isHighlighted, bool isDown)
{
    auto bounds = button.getLocalBounds().toFloat().reduced(0.5f);
    const bool isOn = button.getToggleState();

    juce::Colour fill;
    if (isOn)
        fill = Colours::Cyan;
    else if (isDown)
        fill = Colours::LightGrey;
    else if (isHighlighted)
        fill = Colours::LightGrey;
    else
        fill = Colours::MidGrey;

    g.setColour(fill);
    g.fillRect(bounds);

    g.setColour(isOn ? Colours::Cyan : Colours::BorderSubtle);
    g.drawRect(bounds, 1.0f);

    juce::ignoreUnused(backgroundColour);
}

void GSFLookAndFeel::drawButtonText(juce::Graphics& g, juce::TextButton& button,
                                    bool isHighlighted, bool isDown)
{
    const bool isOn = button.getToggleState();
    auto textCol = isOn ? Colours::Black
                        : (isHighlighted ? Colours::White : Colours::TextSecondary);

    g.setColour(textCol);
    g.setFont(getMonoFont(juce::jmin(11.0f, button.getHeight() * 0.42f), false));
    g.drawFittedText(button.getButtonText(),
                     button.getLocalBounds(),
                     juce::Justification::centred, 1);

    juce::ignoreUnused(isDown);
}

void GSFLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& label)
{
    g.fillAll(label.findColour(juce::Label::backgroundColourId));
    g.setColour(label.findColour(juce::Label::textColourId));
    g.setFont(getMonoFont(juce::jmin(12.0f, label.getHeight() * 0.7f)));

    auto textArea = label.getBorderSize().subtractedFrom(label.getLocalBounds());
    g.drawFittedText(label.getText(), textArea, label.getJustificationType(), 1);
}

void GSFLookAndFeel::drawComboBox(juce::Graphics& g, int width, int height,
                                  bool isButtonDown, int, int, int, int,
                                  juce::ComboBox& box)
{
    auto bounds = juce::Rectangle<int>(0, 0, width, height).toFloat().reduced(0.5f);
    g.setColour(box.findColour(juce::ComboBox::backgroundColourId));
    g.fillRect(bounds);
    g.setColour(box.findColour(juce::ComboBox::outlineColourId));
    g.drawRect(bounds, 1.0f);

    juce::Path arrow;
    auto arrowZone = juce::Rectangle<float>(width - 18.0f, 0.0f, 12.0f, (float) height);
    arrow.addTriangle(arrowZone.getX(),        arrowZone.getCentreY() - 2.0f,
                      arrowZone.getRight(),    arrowZone.getCentreY() - 2.0f,
                      arrowZone.getCentreX(),  arrowZone.getCentreY() + 4.0f);
    g.setColour(Colours::Cyan);
    g.fillPath(arrow);

    juce::ignoreUnused(isButtonDown);
}

void GSFLookAndFeel::drawGlow(juce::Graphics& g, juce::Rectangle<float> bounds,
                              juce::Colour colour, float radius)
{
    for (float i = radius; i > 0.0f; i -= 1.5f)
    {
        float alpha = colour.getFloatAlpha() * (1.0f - i / radius) * 0.4f;
        g.setColour(colour.withAlpha(alpha));
        g.drawRect(bounds.expanded(i), 1.0f);
    }
}

void GSFLookAndFeel::drawArcadeBorder(juce::Graphics& g, juce::Rectangle<int> bounds,
                                      juce::Colour colour, int thickness)
{
    g.setColour(colour);
    g.drawRect(bounds, thickness);
}

void GSFLookAndFeel::drawWifiIcon(juce::Graphics& g, juce::Rectangle<float> bounds,
                                  juce::Colour colour, float pulsePhase)
{
    auto centre = juce::Point<float>(bounds.getCentreX(), bounds.getBottom() - 2.0f);
    float baseRadius = juce::jmin(bounds.getWidth(), bounds.getHeight()) * 0.25f;

    for (int i = 0; i < 3; ++i)
    {
        float phaseOffset = (float) i * 0.5f;
        float pulse = 0.5f + 0.5f * std::sin(pulsePhase + phaseOffset);
        float alpha = 0.3f + 0.6f * pulse;
        float r = baseRadius * (1.0f + (float) i * 0.9f);

        g.setColour(colour.withAlpha(alpha));
        juce::Path arc;
        arc.addCentredArc(centre.x, centre.y, r, r, 0.0f,
                          -juce::MathConstants<float>::halfPi - 0.9f,
                          -juce::MathConstants<float>::halfPi + 0.9f, true);
        g.strokePath(arc, juce::PathStrokeType(1.5f, juce::PathStrokeType::curved,
                                               juce::PathStrokeType::rounded));
    }

    g.setColour(colour);
    g.fillEllipse(centre.x - 1.5f, centre.y - 1.5f, 3.0f, 3.0f);
}

} // namespace gsf::ui
