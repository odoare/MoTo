/*
  ==============================================================================

    buttonsComponent.cpp
    Created: 26 May 2025 6:33:40pm
    Author:  doare

  ==============================================================================
*/

#include <JuceHeader.h>
#include "buttonsComponent.h"

//==============================================================================
ButtonsComponent::ButtonsComponent(
  juce::AudioProcessorValueTreeState& ap,
  juce::StringArray choices,
  juce::String exclusiveParam,
  juce::LookAndFeel &lookAndFeel
  )
    : juce::Component("buttonsComponent"),
      buttons(),
      channelButtonAttachments()
{

    apvts = &ap;
    numChannels = choices.size();
    exclusiveParamName = exclusiveParam;
    for (size_t i = 0; i < numChannels; ++i)
    {
        buttons.push_back(std::make_unique<juce::ToggleButton>());
        buttons.back()->setClickingTogglesState(true);
        buttons.back()->setButtonText(choices[i]);
        buttons.back()->setColour(juce::ToggleButton::tickColourId,juce::Colours::cyan);
        buttons.back()->setLookAndFeel(&lookAndFeel);
        addAndMakeVisible(buttons.back().get());
        channelButtonAttachments.push_back(std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
            *apvts, choices[i], *buttons.back()));
    }

    exclusiveButton.setClickingTogglesState(true);
    exclusiveButton.setButtonText("Excl.");
    exclusiveButton.setColour(juce::ToggleButton::tickColourId, juce::Colours::cyan);
    // exclusiveButton.setLookAndFeel(&buttonLookAndFeel);
    addAndMakeVisible(exclusiveButton);
    exclusiveButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        *apvts, exclusiveParam, exclusiveButton);

}

ButtonsComponent::~ButtonsComponent()
{
}

void ButtonsComponent::paint (juce::Graphics& g)
{

}

void ButtonsComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

  juce::FlexBox fb;
  fb.flexDirection = juce::FlexBox::Direction::row;
  for (size_t i = 0; i < numChannels; ++i)
  {
      fb.items.add(juce::FlexItem(*buttons[i]).withFlex(1.0f));
  }
  fb.items.add(juce::FlexItem(exclusiveButton).withFlex(1.5f));
  fb.performLayout(getLocalBounds().toFloat());
}

void ButtonsComponent::prepare()
{
  auto selectFunc = [this, apvtsPtr = apvts](int i)
    {
    if (apvtsPtr->getRawParameterValue(exclusiveParamName)->load())
      {
        bool state = buttons[i]->getToggleState();
        for (int j = 0; j < numChannels; ++j)
        {
            if (j != i && state) buttons[j]->setToggleState(false, juce::sendNotification);
        }
      }
    };

  for (int i = 0; i < numChannels; ++i)
  {
    buttons[i]->onClick = [selectFunc, i]()
    {
        selectFunc(i);
    };
  }
}
