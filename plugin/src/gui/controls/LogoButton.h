#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class LogoButton : public juce::Button {
public:
  LogoButton() : juce::Button("LogoButton") {}

  void paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown) override {
    auto bounds = getLocalBounds().toFloat();

    g.setColour(juce::Colours::transparentBlack);
    g.fillRect(bounds);

    juce::Colour lineColor = juce::Colours::transparentBlack;

    g.setColour(lineColor);
    float w = bounds.getWidth(), h = bounds.getHeight();

    juce::Path lines;
    float lineThickness = 2.5f;
    float spacing = h * 0.2f; // Space between lines

    lines.startNewSubPath(w * 0.2f, h * 0.4f);
    lines.lineTo(w * 0.8f, h * 0.4f);

    lines.startNewSubPath(w * 0.2f, h * 0.6f);
    lines.lineTo(w * 0.8f, h * 0.6f);

    g.strokePath(lines, juce::PathStrokeType(lineThickness));
  }
};
