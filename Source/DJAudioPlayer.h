/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 14 Jul 2022 8:57:56am
    Author:  Janko Bauer

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class DJAudioPlayer : public juce::AudioSource {

public:
    // Initilizer for one of the two audio players, passing a referecne to the single 
    //instance of format manager
    DJAudioPlayer(juce::AudioFormatManager& _formatManager);
    ~DJAudioPlayer();

    // Gets the audio sources ready to work
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    // Sends the desired audio stream to the output hardware, this audio stream will 
    //first be modified to our desire
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    // At the end, deconstruct the resource streames
    void releaseResources() override;

    // Takes a file path and uses the format maneger to extract the audio data 
    //into the readerSource. This reader source will be used for further modifications
    void loadURL(juce::URL audioURL);
    // Changes the anplitude of the readerSource, aka Volume.
    void setGain(double gain);
    // readerSource is sped up or down.
    void setSpeed(double ratio);
    // set the location in the audio track to play
    void setPosition(double posInSecs);
    // calulates the position in seconds and calls setPosition with the result
    void setPositionRelative(double pos);
    // Starts a audio stream
    void start();
    // Pauses a audio stream
    void stop();

    /** Get the relative position of the palyhead */
    double getPositionRelative();

    // This variable is used to tobble the reverb effect. It is toggeled from DeckGUI
    juce::String reverbStatus = "off";
    // Sets the parameters for the reverb effect. There are default values but we can change then from the DeckGUI
    // Parameters are the new value form the knobs and the parameter to set. With this info the function can update the reverb. 
    void setParameter(float newValue, juce::String paramToSet);

private:
    juce::AudioFormatManager& formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{ &transportSource, false, 2 };
    juce::ReverbAudioSource reverbSource{ &resampleSource, false };    
};