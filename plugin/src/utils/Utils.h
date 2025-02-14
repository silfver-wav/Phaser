#pragma once
#include <juce_graphics/juce_graphics.h>

namespace Colours {
  inline juce::Colour primaryColour = juce::Colour::fromRGB(124, 186, 122);
  inline juce::Colour secondaryColour = juce::Colour::fromRGB(176, 222, 175);
inline juce::Colour thirdaryColour = primaryColour.darker(0.3f); // juce::Colour::fromRGB(148, 145, 156);
};

namespace Layout {
  inline int padding = 2;
  inline float radius = 10.0f;
}