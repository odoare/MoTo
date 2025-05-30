/*
  ==============================================================================

    buttonsComponent.h
    Created: 26 May 2025 6:33:40pm
    Author:  doare

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ButtonsComponent  : public juce::Component
{
public:
    ButtonsComponent(juce::AudioProcessorValueTreeState& ap,
      juce::StringArray choices,
      juce::String exclusiveParam = "Exclusive",
      juce::LookAndFeel &lookAndFeel=juce::LookAndFeel::getDefaultLookAndFeel()
      );
    ~ButtonsComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void prepare();

private:
    int numChannels;

    // Toggle buttons for channel selection
    std::vector<std::unique_ptr<juce::ToggleButton>> buttons;
    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>> channelButtonAttachments;
    juce::AudioProcessorValueTreeState *apvts;
    juce::ToggleButton exclusiveButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> exclusiveButtonAttachment;
    juce::String exclusiveParamName;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ButtonsComponent)
};
