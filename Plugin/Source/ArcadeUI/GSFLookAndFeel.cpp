/*
  ==============================================================================
   GSF FIGHTER - Arcade Look & Feel Implementation
  ==============================================================================
*/

#include "GSFLookAndFeel.h"

namespace gsf::ui
{

GSFLookAndFeel::GSFLookAndFeel()
{
    // Set default colours
    setColour(juce::ResizableWindow::backgroundColourId, Colours::Black);
    setColour(juce::TextButton::buttonColourId, Colours::MidGrey);
    setColour(juce::TextButton::buttonOnColourId, Colours::Red);
    setColour(juce::TextButton::textColourOffId, Colours::White);
    setColour(juce::TextButton::textColourOnId, Colours::Yellow);
    setColour(juce::Label::textColourId, Colours::White);
    setColour(juce::ComboBox::backgroundColourId, Colours::DarkGrey);
    setColour(juce::ComboBox::textColourId, Colours::White);
    setColour(juce::ComboBox::outlineColourId, Colours::LightGrey);
}

juce::Font GSFLookAndFeel::getArcadeFont(float height) const
{
    // Use a bold monospace font reminiscent of arcade machines
    // In production, bundle a custom pixel/arcade font
    return juce::Font(juce::FontOptions(height, juce::Font::bold));
}

juce::Font GSFLookAndFeel::getTitleFont(float height) const
{
    return juce::Font(juce::FontOptions(height, juce::Font::bold));
}

void GSFLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button,
                                           const juce::Colour& backgroundColour,
                                           bool isHighlighted, bool isDown)
{
    auto bounds = button.getLocalBounds().toFloat().reduced(1.0f);
    auto baseColour = backgroundColour;

    if (isDown)
        baseColour = baseColour.brighter(0.2f);
    else if (isHighlighted)
        baseColour = baseColour.brighter(0.1f);

    // Arcade button: solid fill with beveled edge
    g.setColour(baseColour.darker(0.3f));
    g.fillRoundedRectangle(bounds, 4.0f);

    auto innerBounds = bounds.reduced(2.0f);
    if (isDown)
        innerBounds = innerBounds.translated(0, 1.0f);

    // Gradient from top-light to bottom-dark
    g.setGradientFill(juce::ColourGradient(
        baseColour.brighter(0.15f), innerBounds.getX(), innerBounds.getY(),
        baseColour.darker(0.1f), innerBounds.getX(), innerBounds.getBottom(),
        false));
    g.fillRoundedRectangle(innerBounds, 3.0f);

    // Glow when toggled on
    if (button.getToggleState())
    {
        auto glowColour = button.findColour(juce::TextButton::buttonOnColourId).withAlpha(0.3f);
        drawGlow(g, bounds, glowColour, 12.0f);
    }

    // Border
    g.setColour(isHighlighted ? Colours::Yellow : Colours::LightGrey);
    g.drawRoundedRectangle(bounds, 4.0f, 1.5f);
}

void GSFLookAndFeel::drawButtonText(juce::Graphics& g, juce::TextButton& button,
                                     bool isHighlighted, bool isDown)
{
    auto font = getArcadeFont(juce::jmin(16.0f, button.getHeight() * 0.55f));
    g.setFont(font);

    auto colour = button.getToggleState()
                      ? button.findColour(juce::TextButton::textColourOnId)
                      : button.findColour(juce::TextButton::textColourOffId);

    if (isDown)
        colour = colour.brighter(0.2f);

    g.setColour(colour);

    auto textBounds = button.getLocalBounds();
    if (isDown)
        textBounds = textBounds.translated(0, 1);

    g.drawFittedText(button.getButtonText(), textBounds, juce::Justification::centred, 1);
}

void GSFLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& label)
{
    g.fillAll(label.findColour(juce::Label::backgroundColourId));

    auto font = getArcadeFont(juce::jmin(16.0f, label.getHeight() * 0.7f));
    g.setFont(font);
    g.setColour(label.findColour(juce::Label::textColourId));

    auto textArea = label.getBorderSize().subtractedFrom(label.getLocalBounds());
    g.drawFittedText(label.getText(), textArea, label.getJustificationType(),
                     juce::jmax(1, static_cast<int>(textArea.getHeight() / font.getHeight())),
                     label.getMinimumHorizontalScale());
}

void GSFLookAndFeel::drawComboBox(juce::Graphics& g, int width, int height,
                                   bool isButtonDown, int, int, int, int,
                                   juce::ComboBox& box)
{
    auto bounds = juce::Rectangle<int>(0, 0, width, height).toFloat().reduced(1.0f);

    g.setColour(box.findColour(juce::ComboBox::backgroundColourId));
    g.fillRoundedRectangle(bounds, 3.0f);

    g.setColour(box.findColour(juce::ComboBox::outlineColourId));
    g.drawRoundedRectangle(bounds, 3.0f, 1.5f);

    // Arrow
    auto arrowZone = juce::Rectangle<float>(width - 25.0f, 0.0f, 20.0f, static_cast<float>(height));
    juce::Path arrow;
    arrow.addTriangle(arrowZone.getX() + 3.0f, arrowZone.getCentreY() - 3.0f,
                      arrowZone.getRight() - 3.0f, arrowZone.getCentreY() - 3.0f,
                      arrowZone.getCentreX(), arrowZone.getCentreY() + 4.0f);

    g.setColour(Colours::Yellow);
    g.fillPath(arrow);
}

void GSFLookAndFeel::drawGlow(juce::Graphics& g, juce::Rectangle<float> bounds,
                               juce::Colour colour, float radius)
{
    for (float i = radius; i > 0; i -= 2.0f)
    {
        float alpha = colour.getFloatAlpha() * (1.0f - i / radius) * 0.5f;
        g.setColour(colour.withAlpha(alpha));
        g.drawRoundedRectangle(bounds.expanded(i), 4.0f + i, 1.5f);
    }
}

void GSFLookAndFeel::drawScanlines(juce::Graphics& g, juce::Rectangle<int> area, float opacity)
{
    g.setColour(juce::Colour::fromFloatRGBA(1.0f, 1.0f, 1.0f, opacity));
    for (int y = area.getY(); y < area.getBottom(); y += 3)
    {
        g.drawHorizontalLine(y, static_cast<float>(area.getX()),
                             static_cast<float>(area.getRight()));
    }
}

void GSFLookAndFeel::drawArcadeBorder(juce::Graphics& g, juce::Rectangle<int> bounds,
                                       juce::Colour colour, int thickness)
{
    // Outer glow
    g.setColour(colour.withAlpha(0.15f));
    g.drawRect(bounds.expanded(thickness), thickness);

    // Main border
    g.setColour(colour);
    g.drawRect(bounds, thickness);

    // Inner highlight (top-left shine)
    g.setColour(colour.brighter(0.3f).withAlpha(0.5f));
    g.drawHorizontalLine(bounds.getY(), static_cast<float>(bounds.getX()),
                         static_cast<float>(bounds.getRight()));
    g.drawVerticalLine(bounds.getX(), static_cast<float>(bounds.getY()),
                       static_cast<float>(bounds.getBottom()));
}

} // namespace gsf::ui
