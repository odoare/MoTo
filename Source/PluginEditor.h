/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Components/knobsComponent.h"
#include "Components/buttonsComponent.h"
#include "Components/FxmeLogo.h"

//==============================================================================
/**
*/
class MoToAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MoToAudioProcessorEditor (MoToAudioProcessor&);
    ~MoToAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MoToAudioProcessor& audioProcessor;

    void addController(juce::Slider&, juce::Slider::SliderStyle, juce::Colour, juce::Colour);
    void addAndConnectLabel(juce::Slider&, juce::Label&);

    fxme::FxmeKnob levelSlider{ audioProcessor.apvts, "Level", juce::Colours::cyan};
    fxme::FxmeButton muteButton{audioProcessor.apvts, "Mute", juce::Colours::red};
    fxme::FxmeButton dimButton{audioProcessor.apvts, "Dim", juce::Colours::yellow};
    fxme::FxmeButton monoButton{audioProcessor.apvts, "Mono", juce::Colours::green};


    fxme::FxmeLookAndFeel buttonLookAndFeel;
    fxme::FxmeLookAndFeel knobLookAndFeel;

    fxme::HorizontalVuMeter meterL{[&]() { return audioProcessor.getMaxLevel(0); },
                                [&]() { return audioProcessor.getSmoothedMaxLevel(0); }},
                         meterR{[&]() { return audioProcessor.getMaxLevel(1); },
                                [&]() { return audioProcessor.getSmoothedMaxLevel(1); }};

    int lastChangedChannelIndex;

    // juce::Image logo;
    FxmeLogo logo;

    juce::TabbedComponent tabs{juce::TabbedButtonBar::TabsAtTop};
    ButtonsComponent buttonsComponent{audioProcessor.apvts, audioProcessor.choices, "Exclusive", buttonLookAndFeel};
    KnobsComponent knobsComponent{audioProcessor.apvts, audioProcessor.choicesL, knobLookAndFeel};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MoToAudioProcessorEditor)
};
