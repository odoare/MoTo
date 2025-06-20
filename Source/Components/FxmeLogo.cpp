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
FxmeLogo::FxmeLogo(juce::String title)
    : titleText(title)
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

    if (logo.isValid())
    {
        auto bounds = getLocalBounds().toFloat();
        auto size = juce::jmin(bounds.getWidth(), bounds.getHeight() * 0.7f);
        if (size < bounds.getWidth())
            bounds.setX(bounds.getX() + (bounds.getWidth() - size) / 2.0f);
        if (size < bounds.getHeight() * 0.7f)
            bounds.setY(bounds.getY() + (bounds.getHeight() * 0.7f - size) / 2.0f);
        auto logoBounds = juce::Rectangle<float>(bounds.getX(),
                                                 bounds.getY(),
                                                 size,
                                                 size);
        g.drawImage(logo, logoBounds);
    }

    g.setColour(juce::Colours::white);
    auto bounds = getLocalBounds().toFloat();
    auto logoBounds = juce::Rectangle<float>(bounds.getX(),
                                             bounds.getY() + bounds.getHeight() * 0.7,
                                             bounds.getWidth(),
                                             bounds.getHeight() * 0.3);
    g.drawText(titleText, logoBounds,
                juce::Justification::centred, true);   // draw some placeholder text
}

void FxmeLogo::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
