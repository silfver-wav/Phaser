#include "PluginEditor.h"
#include "PluginProcessor.h"
#include "utils/Utils.h"

AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(
    AudioPluginAudioProcessor& p)
    : AudioProcessorEditor(&p),
      processorRef(p),
      outputComponent(p.parameters),
      filterComponent(p.parameters),
      lfoComponent(p.parameters)
{
    addAndMakeVisible(outputComponent);
    addAndMakeVisible(filterComponent);
    addAndMakeVisible(lfoComponent);

    setResizable(true, true);
    setResizeLimits(450, 450, 2000, 1600);
    getConstrainer()->setFixedAspectRatio(2.0f);
    setSize(478, 470);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

void AudioPluginAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(Colours::primaryColour.brighter(0.35f));
}

void AudioPluginAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();

    auto bottomArea = area.reduced(Layout::padding);
    auto outputArea = bottomArea.removeFromRight(bottomArea.proportionOfWidth(0.2f));
    outputComponent.setBounds(outputArea.reduced(Layout::padding));
    filterComponent.setBounds(bottomArea.removeFromTop(bottomArea.proportionOfHeight(0.5f)).reduced(Layout::padding));
    lfoComponent.setBounds(bottomArea.reduced(Layout::padding));
}
