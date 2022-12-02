/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 22 Jul 2022 9:50:31am
    Author:  Janko Bauer

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
                                 juce::AudioThumbnailCache& cacheToUse) :
                                   audioThumb(1000, formatManagerToUse, cacheToUse), 
                                   fileLoaded(false), 
                                   plyHeadPos(0)
{
    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(juce::Colours::lightgrey);
    g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), 2);
    g.setColour(juce::Colours::darkblue);
    g.drawRoundedRectangle(0, 0, getWidth(), getHeight(), 5, 2);


    if (fileLoaded) {
        g.setColour(juce::Colours::darkgreen);

        audioThumb.drawChannel(g, getLocalBounds(), 0,
            audioThumb.getTotalLength(), 1, 1.0f);

        g.setColour(juce::Colours::darkorange);
        g.drawRect(plyHeadPos * getWidth(), 0, getWidth() / 10, getHeight(), 2);
    }
    else {
        g.setColour(juce::Colours::orange);
        g.setFont(20.0f);
        g.drawText("File not Loaded...", getLocalBounds(),
            juce::Justification::centred, true);
    }
}

void WaveformDisplay::resized()
{

}

void WaveformDisplay::loadURL(juce::URL audioURL)
{
    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));
}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source)
{
   repaint();
}

void WaveformDisplay::setPositionRelative(double pos)
{
    if (plyHeadPos != pos)
    {
        plyHeadPos = pos;
        repaint();
    }
}