/*
  ------------------------------------------------------------------------------

    knobsComponent.cpp
    Author:  Olivier Doaré    
    github.com/odoare

    (c) 2023-2025 Olivier Doaré

    Licenced under the GNU Lesser General Public License (LGPL) Version 3.0
    SPDX-License-Identifier: LGPL-3.0-or-later

  ------------------------------------------------------------------------------
    This file implements a component that contains a set of knobs
    for controlling parameters in an AudioProcessorValueTreeState.
    The knobs are linked to parameters and can be resized dynamically.
  ------------------------------------------------------------------------------
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
