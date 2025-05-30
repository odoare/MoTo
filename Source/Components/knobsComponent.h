/*
  ==============================================================================

    knobsComponent.h
    Created: 26 May 2025 6:34:14pm
    Author:  doare

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class KnobsComponent  : public juce::Component
{
public:
    KnobsComponent(juce::AudioProcessorValueTreeState& ap,
      juce::StringArray choices,
      juce::LookAndFeel &lookAndFeel=juce::LookAndFeel::getDefaultLookAndFeel()
      );
    ~KnobsComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    int numChannels;
    juce::AudioProcessorValueTreeState *apvts;

    std::vector<std::unique_ptr<fxme::FxmeKnob>> knobs;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KnobsComponent)
};
