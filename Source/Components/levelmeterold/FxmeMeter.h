/*
  ==============================================================================

    VerticalMeter.h
    Created: 29 Nov 2023 4:38:30pm
    Author:  doare

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <iostream>

class VerticalMeter : public juce::Component, public juce::Timer
{
public:
  VerticalMeter(std::function<float()>&& valueFunction) : valueSupplier(std::move(valueFunction))
  {
    startTimerHz(24);
    meterColour=juce::Colours::white;
  }

  void paint(juce::Graphics& g) override
  {
    const auto level = valueSupplier();

    auto bounds = getLocalBounds().reduced(10).toFloat();
    auto h = bounds.getHeight();
    const auto newHeight = juce::jmap(level,-60.0f, 6.0f, 0.0f, h);
    auto dh = -h+newHeight;
    auto y = bounds.getTopLeft().getY();
    auto bounds2 = bounds;
    bounds2.setTop(y-dh);
  
    // g.setColour(juce::Colours::black);
    // g.fillRect(bounds);

    g.setColour(meterColour);
    //const auto scaledY = juce::jmap(level,-60.0f, 6.0f, 0.0f, static_cast<float>(bounds.getHeight()));
    g.fillRect(bounds2);
  }

  void timerCallback() override
  {
    repaint();
  }

  void setColour(juce::Colour newColour)
  {
    meterColour = newColour;
  }

private:
  std::function<float()> valueSupplier;
  juce::Colour meterColour;
};

/*
  ==============================================================================

    VerticalMeter.h
    Created: 29 Nov 2023 4:38:30pm
    Author:  doare

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <iostream>

class HorizontalMultiMeter : public juce::Component, public juce::Timer
{
public:
  HorizontalMultiMeter(std::function<float()>&& maxValueFunction,
                        std::function<float()>&& smoothedMaxValueFunction)
                        : maxValueSupplier(std::move(maxValueFunction)),
                          smoothedMaxValueSupplier(std::move(smoothedMaxValueFunction))
  {
    startTimerHz(30);
  }

  void paint(juce::Graphics& g) override
  {
    const auto maxLevel = maxValueSupplier();
    const auto smoothedMaxLevel = smoothedMaxValueSupplier();

    auto bounds = getLocalBounds().reduced(2.f).toFloat();
    auto w = bounds.getWidth();

    const auto maxWidth = juce::jmap(maxLevel,minValue, maxValue, 0.0f, w);
    const auto smoothedMaxWidth = juce::jmap(smoothedMaxLevel, minValue, maxValue, 0.0f, w);
    const auto overBackgroundWidth = juce::jmap(0.f, maxValue, minValue, 0.0f, w);

    // auto dw = -w+newWidth;
    auto x = bounds.getTopLeft().getX();
    auto maxBounds = bounds;
    auto smoothedMaxBounds = bounds;
    auto overBackgroundBounds = bounds;

    g.setColour(backgroundColour);
    g.setColour(juce::Colours::black.brighter(0.2f));
    g.fillRect(bounds);

    g.setColour(overColour.darker(1.5f));
    overBackgroundBounds.setLeft(x+w-overBackgroundWidth);
    g.fillRect(overBackgroundBounds);
    
    if (smoothedMaxLevel>=1.f)
      g.setColour(overColour);
    else
      g.setColour(smoothedMaxColour);
    
    smoothedMaxBounds.setRight(x+smoothedMaxWidth);
    g.fillRect(smoothedMaxBounds);

    g.setColour(maxColour);
    maxBounds.setRight(x+maxWidth);
    g.fillRect(maxBounds);

    g.setColour(juce::Colours::darkgrey);
    g.drawRect(bounds);
  }

  void timerCallback() override
  {
    repaint();
  }

  void setSmoothedMaxColour(juce::Colour newColour)
  {
    smoothedMaxColour = newColour;
  }

  void setMaxColour(juce::Colour newColour)
  {
    maxColour = newColour;
  }

  void setOverColour(juce::Colour newColour)
  {
    overColour = newColour;
  }

  void setBackgroundColour(juce::Colour newColour)
  {
    backgroundColour = newColour;
  }

  bool plotTicks{false};
  float minValue{-60.f}, maxValue{6.f};

private:
  std::function<float()> maxValueSupplier, smoothedMaxValueSupplier;
  juce::Colour backgroundColour{juce::Colours::black},
                smoothedMaxColour{juce::Colours::yellow},
                maxColour{juce::Colours::green},
                overColour{juce::Colours::red};
};
