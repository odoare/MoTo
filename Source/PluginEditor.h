/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Components/FxmeLookAndFeel.h"
#include "Components/FxmeMeter.h"

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
    int channelIndex[NUM_STEREO_OUT];

    juce::ToggleButton exclusiveButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> exclusiveButtonAttachment;
    juce::Label exclusiveLabel{"exclusiveLabel","Excl"};
    
    FxmeButtonLookAndFeel buttonLookAndFeel;
    FxmeKnobLookAndFeel knobLookAndFeel;

    // VerticalMeter   verticalMeterLmax{[&]() { return audioProcessor.getMaxLevel(0); }},
    //                 verticalMeterRmax{[&]() { return audioProcessor.getMaxLevel(1); }},
    //                 verticalMeterL{[&]() { return audioProcessor.getRmsLevel(0); }},
    //                 verticalMeterR{[&]() { return audioProcessor.getRmsLevel(1); }};

    HorizontalMultiMeter meterL{[&]() { return audioProcessor.getMaxLevel(0); },
                                [&]() { return audioProcessor.getSmoothedMaxLevel(0); }},
                         meterR{[&]() { return audioProcessor.getMaxLevel(1); },
                                [&]() { return audioProcessor.getSmoothedMaxLevel(1); }};

    int lastChangedChannelIndex;

    juce::Image logo;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MonitoringSectionAudioProcessorEditor)
};
