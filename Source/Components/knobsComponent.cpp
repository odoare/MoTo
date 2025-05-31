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


#include <JuceHeader.h>
#include "knobsComponent.h"

//==============================================================================
KnobsComponent::KnobsComponent(juce::AudioProcessorValueTreeState& ap,
      juce::StringArray choices,
      juce::LookAndFeel &lookAndFeel)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    apvts = &ap;
    numChannels = choices.size();
    for (int i = 0; i < numChannels; ++i)
    {
      knobs.push_back(std::make_unique<fxme::FxmeKnob>(ap,choices[i],juce::Colours::cyan));
      knobs.back()->slider.setLookAndFeel(&lookAndFeel);
      addAndMakeVisible(knobs.back()->slider);
    }


}

KnobsComponent::~KnobsComponent()
{
}

void KnobsComponent::paint (juce::Graphics& g)
{

}

void KnobsComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
  juce::FlexBox fb;
  fb.flexDirection = juce::FlexBox::Direction::row;
  for (int i = 0; i < numChannels; ++i)
  {
      fb.items.add(juce::FlexItem(knobs[i]->slider).withFlex(1.0f));
  }
  fb.performLayout(getLocalBounds().toFloat());

}
