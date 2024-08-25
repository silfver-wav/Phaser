/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ParamsHelper.h"

//==============================================================================
PhaserAudioProcessorEditor::PhaserAudioProcessorEditor (PhaserAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    initGUI();

    setSize(1000, 500);
    setResizable(true, true);
    setResizeLimits(900, 350, 1500, 500);
    getConstrainer()->setFixedAspectRatio(2.0);

    resized();
}

PhaserAudioProcessorEditor::~PhaserAudioProcessorEditor()
{
}

//==============================================================================
void PhaserAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    
    // g.fillAll (juce::Colours::black.brighter(0.2));
    g.fillAll(juce::Colour::fromRGB(55, 55, 55));

    g.setColour (juce::Colours::black);
    g.setFont (15.0f);
}

void PhaserAudioProcessorEditor::resized()
{
    // Allpass filters Box
    juce::FlexBox nrStagesBox = createFlexBox(nrStagesLabel, nrStagesSlider);
    juce::FlexBox centerBox = createFlexBox(centerLabel, centerSlider);
    juce::FlexBox spreadBox = createFlexBox(spreadLabel, spreadSlider);
    juce::FlexBox feedbackBox = createFlexBox(feedbackLabel, feedbackSlider);

    juce::FlexBox filtersBox;
    filtersBox.flexDirection = juce::FlexBox::Direction::row;
    filtersBox.items.add(juce::FlexItem(nrStagesBox).withFlex(1));
    filtersBox.items.add(juce::FlexItem(centerBox).withFlex(1));
    filtersBox.items.add(juce::FlexItem(spreadBox).withFlex(1));
    filtersBox.items.add(juce::FlexItem(feedbackBox).withFlex(1));


    juce::FlexBox polarityBox;
    polarityBox.justifyContent = juce::FlexBox::JustifyContent::center;
    polarityBox.flexDirection = juce::FlexBox::Direction::column;

    polarityBox.items.add(juce::FlexItem(invertPolarityLabel).withWidth(50).withHeight(25));
    polarityBox.items.add(juce::FlexItem(invertPolarityButton).withWidth(50).withHeight(20).withMargin({ 0, 0, 0, 14 }));

    filtersBox.items.add(juce::FlexItem(polarityBox).withMinWidth(50.0f).withMinHeight(50.0f).withMargin({ 0, 25, 0, 0 }));

    // Lfo Box
    juce::FlexBox lfoFreqBox = createFlexBox(lfoFreqLabel, lfoFreqSlider);
    juce::FlexBox lfoDepthBox = createFlexBox(lfoDepthLabel, lfoDepthSlider);
    juce::FlexBox stereoBox = createFlexBox(stereoLabel, stereoSlider);

    juce::FlexBox lfoBox;
    lfoBox.flexDirection = juce::FlexBox::Direction::row;
    lfoBox.items.add(juce::FlexItem(lfoFreqBox).withFlex(1));
    lfoBox.items.add(juce::FlexItem(lfoDepthBox).withFlex(1));
    lfoBox.items.add(juce::FlexItem(stereoBox).withFlex(1));

    auto localBounds = getLocalBounds().reduced(20);

    // Output Box
    juce::FlexBox depthBox = createFlexBox(depthLabel, depthSlider);
    juce::FlexBox gainBox = createFlexBox(gainLabel, gainSlider);
    juce::FlexBox outputBox;
    outputBox.flexDirection = juce::FlexBox::Direction::column;
    outputBox.alignContent = juce::FlexBox::AlignContent::center;
    outputBox.justifyContent = juce::FlexBox::JustifyContent::center;
    outputBox.items.add(juce::FlexItem(depthBox).withFlex(1).withMargin({ 0, 0, 0, static_cast<float>(localBounds.getWidth() / 20) }));
    outputBox.items.add(juce::FlexItem(gainBox).withFlex(1).withMargin({ 0, 0, 0, static_cast<float>(localBounds.getWidth() / 20) }));

    // Effect Box
    juce::FlexBox effectBox;
    effectBox.flexDirection = juce::FlexBox::Direction::column;
    effectBox.items.add(juce::FlexItem(filtersBox).withFlex(1).withMargin({ 0, 0, 0, 20 }));
    effectBox.items.add(juce::FlexItem(lfoBox).withFlex(1).withMargin({ 0, 70, 0, 70 }));
    
    juce::FlexBox fb;
    fb.flexDirection = juce::FlexBox::Direction::row;
    fb.items.add(juce::FlexItem(effectBox).withFlex(3));
    fb.items.add(juce::FlexItem(outputBox).withFlex(1));

    fb.performLayout(getLocalBounds().reduced(20));

    filtersGroup.setBounds(localBounds.getX(), localBounds.getY(), (3 * localBounds.getWidth()) / 4, localBounds.getHeight() / 2);
    lfoGroup.setBounds(localBounds.getX(), localBounds.getY() + (localBounds.getHeight() / 2), (3 * localBounds.getWidth()) / 4, localBounds.getHeight() / 2);
    outputGroup.setBounds(localBounds.getX() + (3 * localBounds.getWidth() / 4), localBounds.getY(), localBounds.getWidth() / 4, localBounds.getHeight());
}
//==============================================================================

void PhaserAudioProcessorEditor::initGUI()
{
    initRotarySlider("Nr Of Stages", nrStagesLabel, nrStagesSlider, ParamRange::nrStagesStart, ParamRange::nrStagesEnd, ParamRange::nrStagesInterval, ParamRange::nrStagesDefault);
    nrStagesAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.parameters, ParamIDs::nrStages, nrStagesSlider));

    initRotarySlider("Spread", spreadLabel, spreadSlider, ParamRange::spreadStart, ParamRange::spreadEnd, ParamRange::spreadInterval, ParamRange::spreadDefault);
    spreadAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.parameters, ParamIDs::spread, spreadSlider));
    
    initRotarySlider("Center", centerLabel, centerSlider, ParamRange::centerStart, ParamRange::centerEnd, ParamRange::centerInterval, ParamRange::centerDefault);
    centerAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.parameters, ParamIDs::center, centerSlider));
    
    initRotarySlider("Feedback", feedbackLabel, feedbackSlider, ParamRange::feedbackStart, ParamRange::feedbackEnd, ParamRange::feedbackInterval, ParamRange::feedbackDefault);
    feedbackAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.parameters, ParamIDs::feedback, feedbackSlider));

    initToggleButton("Invert", invertPolarityLabel, invertPolarityButton, ParamIDs::invertPolarity);
    invertPolarityAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(audioProcessor.parameters, ParamIDs::invertPolarity, invertPolarityButton));

    initRotarySlider("Mix", depthLabel, depthSlider, ParamRange::depthStart, ParamRange::depthEnd, ParamRange::depthInterval, ParamRange::depthDefault);
    depthAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.parameters, ParamIDs::depth, depthSlider));
    
    initRotarySlider("Rate", lfoFreqLabel, lfoFreqSlider, ParamRange::lfoFreqStart, ParamRange::lfoFreqEnd, ParamRange::lfoFreqInterval, ParamRange::lfoFreqDefault);
    lfoFreqAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.parameters, ParamIDs::lfoFreq, lfoFreqSlider));
    
    initRotarySlider("Amount", lfoDepthLabel, lfoDepthSlider, ParamRange::lfoDepthStart, ParamRange::lfoDepthEnd, ParamRange::lfoDepthInterval, ParamRange::lfoDepthDefault);
    lfoDepthAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.parameters, ParamIDs::lfoDepth, lfoDepthSlider));
    
    initRotarySlider("Stereo", stereoLabel, stereoSlider, ParamRange::stereoStart, ParamRange::stereoEnd, ParamRange::stereoInterval, ParamRange::stereoDefault);
    stereoAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.parameters, ParamIDs::stereo, stereoSlider));

    initRotarySlider("Gain", gainLabel, gainSlider, ParamRange::gainStart, ParamRange::gainEnd, ParamRange::gainInterval, ParamRange::gainDefault);
    gainAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.parameters, ParamIDs::gain, gainSlider));

    initGroup("Filters", filtersGroup);
    initGroup("LFO", lfoGroup);
    initGroup("Output", outputGroup);
}

void PhaserAudioProcessorEditor::initRotarySlider(const std::string& name, juce::Label& label, juce::Slider& slider, float start, float end, float interval, float defaultValue)
{
    label.setText(name, juce::dontSendNotification);
    label.setSize(labelWidth, labelHeight);
    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);

    slider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    slider.setSize(sliderWidth, sliderHeight);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, textBoxWidth, textBoxHeight);
    slider.setLookAndFeel(&knobLookAndFeel);

    slider.setRange(start, end, interval);

    slider.setPopupDisplayEnabled(false, false, this);
    slider.setValue(defaultValue);
    slider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::blue);
    slider.setDoubleClickReturnValue(true, defaultValue);
    addAndMakeVisible(slider);
}

void PhaserAudioProcessorEditor::initToggleButton(const std::string& name, juce::Label& label, juce::ToggleButton& button, const std::string& id)
{
    button.setSize(5, 5);
    addAndMakeVisible(button);

    label.setText(name, juce::dontSendNotification);
    label.setSize(5, 5);
    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);
}

void PhaserAudioProcessorEditor::initGroup(const std::string& name, juce::GroupComponent& group)
{
    group.setText(name);
    // group.setColour(juce::GroupComponent::outlineColourId, juce::Colours::white);
    group.setColour(juce::GroupComponent::outlineColourId, juce::Colours::black); // juce::Colour::fromRGB(51, 191, 219)
    group.setColour(juce::GroupComponent::textColourId, juce::Colours::white);
    addAndMakeVisible(group);
}

juce::FlexBox PhaserAudioProcessorEditor::createFlexBox(juce::Label& label, juce::Slider& slider)
{
    juce::FlexBox fb;
    fb.alignContent = juce::FlexBox::AlignContent::center;
    fb.justifyContent = juce::FlexBox::JustifyContent::center;
    fb.flexDirection = juce::FlexBox::Direction::column;
    fb.items.add(juce::FlexItem(label).withWidth(labelWidth).withHeight(labelHeight));
    fb.items.add(juce::FlexItem(slider).withWidth(sliderWidth).withHeight(sliderHeight));
    
    return fb;
}