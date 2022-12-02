/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 14 Jul 2022 8:57:56am
    Author:  Janko Bauer

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager) 
    : formatManager(_formatManager)
{
}
DJAudioPlayer::~DJAudioPlayer()
{
}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    reverbSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (reverbStatus == "on")
    {
        reverbSource.getNextAudioBlock(bufferToFill);
    }
    else
    {
        resampleSource.getNextAudioBlock(bufferToFill);
    }
}

void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
    reverbSource.releaseResources();
}

void DJAudioPlayer::loadURL(juce::URL audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) // good file retreval 
    {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
    }
}

void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1.0)
    {
    }
    else {
        transportSource.setGain(gain);
    }
}

void DJAudioPlayer::setSpeed(double ratio)
{
    if (ratio < 0 || ratio > 100.0)
    {
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }
}

void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
    if (pos < 0 || pos > 1.0)
    {
    }
    else {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

void DJAudioPlayer::start()
{
    transportSource.start();
    reverbSource.getParameters();
}

void DJAudioPlayer::stop()
{
    transportSource.stop();
}

double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

void DJAudioPlayer::setParameter(float newValue, juce::String paramToSet)
{
    juce::Reverb::Parameters reverbParams = reverbSource.getParameters();

    if (paramToSet == "size")
    {
        reverbParams.roomSize = newValue;
    }
    else if (paramToSet == "damp")
    {
        reverbParams.damping = newValue;
    }
    else if (paramToSet == "wet")
    {
        reverbParams.wetLevel = newValue;
    }
    else if (paramToSet == "dry")
    {
        reverbParams.dryLevel = newValue;
    }
    else if (paramToSet == "width")
    {
        reverbParams.width = newValue;
    }
    else if (paramToSet == "freeze")
    {
        reverbParams.freezeMode = newValue;
    }
    else
    {
        // Do nothing
    }
    reverbSource.setParameters(reverbParams);
}

