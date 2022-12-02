/*
  ==============================================================================

    DeckGUI.h
    Created: 14 Jul 2022 5:45:39pm
    Author:  Janko Bauer

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

//==============================================================================
class DeckGUI  : public juce::Component,
                 public juce::Button::Listener,
                 public juce::Slider::Listener,
                 public juce::FileDragAndDropTarget, 
                 public juce::Timer
{
public:
    // Initialises the Deck. Takes a player to manage the GUI controls, takes the 
    //format manager and waveform display to obtain the wave form data from the 
    //audio file and display it.
    DeckGUI(DJAudioPlayer* player,
            juce::AudioFormatManager& formatManagerToUse,
            juce::AudioThumbnailCache & cachToUse);
    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    /** Implementing button listener to handle all button interactions */
    void buttonClicked(juce::Button*) override;

    /** Implementing Slider listener to handle all slider and knob interactions */
    void sliderValueChanged(juce::Slider* slider) override;

    // Used to start file drag and load operations
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    // If the drag operation was successful we load the file into the audio player
    void filesDropped(const juce::StringArray& files, int x, int y) override;

    // Timer to regularly update the GUI as the track progresses
    void timerCallback() override; 

    // A function used to load the audio file into the audio layer and wave form display for processing. 
    void loadAudioFile(juce::URL fileURL);

private:
    juce::TextButton playButton{ "Play" };
    juce::TextButton pauseButton{ "Pause" };
    juce::TextButton stopButton{ "Stop" };
    juce::TextButton loadButton{ "Load" };
    juce::TextButton reverbStateButton{ "On/Off" };

    juce::Slider volSlider;
    juce::Slider speedSlider;
    juce::Slider posSlider;

    juce::Slider roomSizSlider;
    juce::Slider dampingSlider;
    juce::Slider widthSlider;
    juce::Slider wetLvlSlider;
    juce::Slider dryLvlSlider;
    juce::Slider frezzeSlider;

    juce::String currTrackName = "No track loaded";

    juce::LookAndFeel_V4 deckColours;

    DJAudioPlayer* player;

    WaveformDisplay waveformDisplay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
