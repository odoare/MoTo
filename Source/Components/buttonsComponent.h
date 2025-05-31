/*
  ------------------------------------------------------------------------------

    buttonsComponent.h
    Author:  Olivier Doaré
    github.com/odoare

    (c) 2023-2025 Olivier Doaré

    Licenced under the GNU Lesser General Public License (LGPL) Version 3.0
    SPDX-License-Identifier: LGPL-3.0-or-later

  ------------------------------------------------------------------------------
    This file defines a component that contains a set of toggle buttons
    for selecting channels, with an additional exclusive toggle button.
    The buttons are linked to parameters in an AudioProcessorValueTreeState.
  ------------------------------------------------------------------------------
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
