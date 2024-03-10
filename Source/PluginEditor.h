/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Components/FxmeLookAndFeel.h"

//==============================================================================
/**
*/
class MonitoringSectionAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MonitoringSectionAudioProcessorEditor (MonitoringSectionAudioProcessor&);
    ~MonitoringSectionAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MonitoringSectionAudioProcessor& audioProcessor;

    void addController(juce::Slider&, juce::Slider::SliderStyle, juce::Colour, juce::Colour);
    void addAndConnectLabel(juce::Slider&, juce::Label&);

    juce::Slider levelSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> levelSliderAttachment;
    juce::Label levelLabel{"levelLabel", "Level"};

    juce::ToggleButton muteButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> muteButtonAttachment;
    juce::Label muteLabel{"muteLabel", "Mute"};

    juce::ToggleButton dimButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> dimButtonAttachment;
    juce::Label dimLabel{"dimLabel", "Dim"};

    juce::ToggleButton monoButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> monoButtonAttachment;
    juce::Label monoLabel{"monoLabel", "Mono"};

    juce::ToggleButton channelButton[NUM_STEREO_OUT];
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> channelButtonAttachment[NUM_STEREO_OUT];
    juce::Label channelLabel[NUM_STEREO_OUT];

    FxmeButtonLookAndFeel buttonLookAndFeel;
    FxmeKnobLookAndFeel knobLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MonitoringSectionAudioProcessorEditor)
};
