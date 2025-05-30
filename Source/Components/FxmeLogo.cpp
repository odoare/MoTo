/*
  ==============================================================================

    FxmeLogo.cpp
    Created: 30 May 2025 8:32:38pm
    Author:  doare

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FxmeLogo.h"

//==============================================================================
FxmeLogo::FxmeLogo()
{
  logo = juce::ImageCache::getFromMemory(BinaryData::logo686_png, BinaryData::logo686_pngSize);
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

FxmeLogo::~FxmeLogo()
{
}

void FxmeLogo::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    if (logo.isValid())
    {
        auto bounds = getLocalBounds().toFloat();
        auto logoBounds = juce::Rectangle<float>(bounds.getX(),
                                                 bounds.getY(),
                                                 bounds.getWidth(),
                                                 bounds.getHeight() * 0.7);
        g.drawImage(logo, logoBounds);
    }
    g.setColour(juce::Colours::white);
    auto bounds = getLocalBounds().toFloat();
    auto logoBounds = juce::Rectangle<float>(bounds.getX(),
                                             bounds.getY() + bounds.getHeight() * 0.7,
                                             bounds.getWidth(),
                                             bounds.getHeight() * 0.3);
    g.drawText("MoTo", logoBounds,
                juce::Justification::centred, true);   // draw some placeholder text
}

void FxmeLogo::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
