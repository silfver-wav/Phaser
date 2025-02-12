#pragma once

#include "../../../utils/Utils.h"

#include <juce_gui_basics/juce_gui_basics.h>

class TransparentComboBoxLookAndFeel : public juce::LookAndFeel_V4 {
public:

  void drawComboBox(juce::Graphics &g, int width, int height, bool isButtonDown,
                    int buttonX, int buttonY, int buttonW, int buttonH, juce::ComboBox &box) override {
    auto cornerSize = box.findParentComponentOfClass<juce::ChoicePropertyComponent>() != nullptr ? 0.0f : 3.0f;
    juce::Rectangle<int> boxBounds(0, 0, width, height);

    g.setColour(juce::Colours::transparentBlack);
    g.fillRoundedRectangle(boxBounds.toFloat(), cornerSize);

    juce::Rectangle<int> arrowZone(width - 25, height / 3, 20, height / 3);
    juce::Path path;
    float arrowSize = 2.5f;

    path.startNewSubPath((float)arrowZone.getX() + 4.0f, (float)arrowZone.getCentreY() - arrowSize);
    path.lineTo((float)arrowZone.getCentreX(), (float)arrowZone.getCentreY() + arrowSize);
    path.lineTo((float)arrowZone.getRight() - 4.0f, (float)arrowZone.getCentreY() - arrowSize);

    g.setColour (box.findColour (juce::ComboBox::arrowColourId).withAlpha ((box.isEnabled() ? 0.9f : 0.2f)));
    g.strokePath(path, juce::PathStrokeType(1.5f));
  }


  void drawPopupMenuItem (juce::Graphics &g, const juce::Rectangle<int> &area,
                          bool isSeparator, bool isActive, bool isHighlighted, bool isTicked,
                          bool hasSubMenu, const juce::String &text, const juce::String &shortcutKey,
                          const juce::Drawable *icon, const juce::Colour *textColour) override {
    if (isSeparator) return; // No separator

    auto colour = isHighlighted ? Colours::primaryColour : juce::Colours::white;
    g.setColour(colour);
    juce::Font font = juce::FontOptions("Arial", 12.0f, juce::Font::plain);
    g.setFont (font);
    g.drawText (text, area.reduced(10, 0), juce::Justification::centredLeft, true);
  }

  void drawPopupMenuBackground (juce::Graphics &g, int width, int height) override {
    g.setColour(juce::Colours::black);
    g.fillRect(0, 0, width, height);
  }

  void drawComboBoxTextWhenNothingSelected (juce::Graphics& g, juce::ComboBox& box, juce::Label& label) override
  {
    g.setColour (juce::Colours::white);

    auto font = label.getLookAndFeel().getLabelFont (label);

    g.setFont (font);

    auto textArea = getLabelBorderSize (label).subtractedFrom (label.getLocalBounds());


    g.drawFittedText (box.getTextWhenNothingSelected(), textArea, juce::Justification::centred,
                      juce::jmax (1, (int) ((float) textArea.getHeight() / font.getHeight())),
                      label.getMinimumHorizontalScale());
  }

  void positionComboBoxText(juce::ComboBox& box, juce::Label& label) override
  {
    label.setBounds (0, 0,
                     box.getWidth() - 20,
                     box.getHeight());

    label.setFont (getComboBoxFont (box));
    label.setJustificationType(juce::Justification::centred);
  }
};
