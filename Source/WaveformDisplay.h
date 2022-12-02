/*
  ==============================================================================

    WaveformDisplay.h
    Created: 22 Jul 2022 9:50:31am
    Author:  Janko Bauer

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveformDisplay  : public juce::Component, 
                         public juce:: ChangeListener
{
public:
    WaveformDisplay(juce::AudioFormatManager& firmatManagerToUse,
                    juce::AudioThumbnailCache& cacheToUse);

    ~WaveformDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    // Load a track to proccess the wave form
    void loadURL(juce::URL audioURL);

    /** set the relaitive position of the playhead */
    void setPositionRelative(double pos);

private:
    juce::AudioThumbnail audioThumb;
    bool fileLoaded; 
    double plyHeadPos;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
