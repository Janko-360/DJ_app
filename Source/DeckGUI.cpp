/*
  ==============================================================================

    DeckGUI.cpp
    Created: 14 Jul 2022 5:45:39pm
    Author:  Janko Bauer

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, 
                juce::AudioFormatManager& _formatManagerToUse,
                juce::AudioThumbnailCache& _cacheToUse) 
    : player(_player),
      waveformDisplay(_formatManagerToUse, _cacheToUse)
{
    // Set all custom colours to one pallet and assign the pallet as needed
    deckColours.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
    deckColours.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    deckColours.setColour(juce::TextButton::textColourOnId, juce::Colours::red);
    deckColours.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
    deckColours.setColour(juce::Slider::thumbColourId, juce::Colours::green);
    deckColours.setColour(juce::Slider::trackColourId, juce::Colours::darkgreen);

    // Play button
    addAndMakeVisible(playButton);
    playButton.setLookAndFeel(&deckColours);
    playButton.setToggleable(true);
    playButton.addListener(this);

    // Stop button 
    addAndMakeVisible(stopButton); 
    stopButton.setLookAndFeel(&deckColours);
    playButton.setToggleable(true);
    stopButton.addListener(this);

    // Pause button
    addAndMakeVisible(pauseButton);
    pauseButton.setLookAndFeel(&deckColours);
    pauseButton.setToggleable(true);
    pauseButton.addListener(this);

    // Load button 
    addAndMakeVisible(loadButton);
    loadButton.addListener(this);
    loadButton.setLookAndFeel(&deckColours);

    // Volume slider
    addAndMakeVisible(volSlider);
    volSlider.setSliderStyle(juce::Slider::LinearVertical);
    volSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,
        false,
        120, 20);
    volSlider.setRange(0.0, 1.0);
    volSlider.setValue(0.25);  // Default volume value 
    volSlider.setLookAndFeel(&deckColours);
    volSlider.addListener(this);

    // Speed slider
    addAndMakeVisible(speedSlider);
    speedSlider.setSliderStyle(juce::Slider::LinearVertical);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,
        false,
        120, 20);
    speedSlider.setValue(1.0);  // Default speed of 1
    speedSlider.setLookAndFeel(&deckColours);
    speedSlider.addListener(this);

    // ----- Reverb controls -----
    // Reverb On/Off button
    addAndMakeVisible(reverbStateButton);
    reverbStateButton.addListener(this);
    reverbStateButton.setLookAndFeel(&deckColours);
    reverbStateButton.setToggleable(true);
    reverbStateButton.setToggleState(false, false);

    // Size
    addAndMakeVisible(roomSizSlider);
    roomSizSlider.addListener(this);
    roomSizSlider.setRange(0.0, 1.0);
    roomSizSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    roomSizSlider.setSliderStyle(juce::Slider::Rotary);
    roomSizSlider.setValue(0.5f);
    roomSizSlider.setLookAndFeel(&deckColours);

     // Damp 
    addAndMakeVisible(dampingSlider);
    dampingSlider.addListener(this);
    dampingSlider.setRange(0.0, 1.0);
    dampingSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    dampingSlider.setSliderStyle(juce::Slider::Rotary);
    dampingSlider.setValue(0.5f);
    dampingSlider.setLookAndFeel(&deckColours);

    // Width
    addAndMakeVisible(widthSlider);
    widthSlider.addListener(this);
    widthSlider.setRange(0.0, 1.0);
    widthSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    widthSlider.setSliderStyle(juce::Slider::Rotary);
    widthSlider.setValue(1.0f);
    widthSlider.setLookAndFeel(&deckColours);

    // Wet 
    addAndMakeVisible(wetLvlSlider);
    wetLvlSlider.addListener(this);
    wetLvlSlider.setRange(0.0, 1.0);
    wetLvlSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    wetLvlSlider.setSliderStyle(juce::Slider::Rotary);
    wetLvlSlider.setValue(0.33f);
    wetLvlSlider.setLookAndFeel(&deckColours);

    // Dry
    addAndMakeVisible(dryLvlSlider);
    dryLvlSlider.addListener(this);
    dryLvlSlider.setRange(0.0, 1.0);
    dryLvlSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    dryLvlSlider.setSliderStyle(juce::Slider::Rotary);
    dryLvlSlider.setValue(0.4f);
    dryLvlSlider.setLookAndFeel(&deckColours);

    // Frezze 
    addAndMakeVisible(frezzeSlider);
    frezzeSlider.addListener(this);
    frezzeSlider.setRange(0.0, 1.0);
    frezzeSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    frezzeSlider.setSliderStyle(juce::Slider::Rotary);
    frezzeSlider.setValue(0.0f);
    frezzeSlider.setLookAndFeel(&deckColours);

    // Track Possition slider
    addAndMakeVisible(posSlider);
    posSlider.setLookAndFeel(&deckColours);
    posSlider.setSliderStyle(juce::Slider::Rotary);
    posSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    posSlider.setRange(0.0, 1.0);
    posSlider.addListener(this);
    
    // Waveform Display
    addAndMakeVisible(waveformDisplay);

    // Start the timer for GUI refresh 
    Timer::startTimer(500);
}

DeckGUI::~DeckGUI()
{
    Timer::stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    double rowHeight = getHeight() / 16;

    // Labels 
    g.setFont(16.0f);
    g.drawText("Volume Slider", getWidth()/14, rowHeight*8, getWidth()*2/14, rowHeight,
        juce::Justification::left, true);

    g.drawText("Speed Slider", getWidth() * 3 / 14, rowHeight * 8, getWidth()*2 / 14, rowHeight,
        juce::Justification::left, true);

    g.drawText("Reverb", getWidth() * 7.6 / 14, rowHeight * 8.3, getWidth()*2 / 14, rowHeight,
        juce::Justification::left, true);

    // Data screen
    double colWidth = getWidth() / 8;

    g.setColour(juce::Colours::blue);
    g.fillRoundedRectangle(colWidth * 1.5, rowHeight * 0.5, colWidth * 5, rowHeight * 3.5, 10);
    g.setColour(juce::Colours::darkblue);
    g.drawRoundedRectangle(colWidth * 1.5, rowHeight * 0.5, colWidth * 5, rowHeight * 3.5, 10, 4);

    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawText("Track name: " + currTrackName, colWidth * 1.5, rowHeight * -0.5, colWidth * 5, rowHeight * 3.5,
        juce::Justification::centred, true);

    // Reverb control lables
     colWidth = getWidth() / 14;
     g.setColour(juce::Colours::black);
     g.setFont(16.0f);
    g.drawText("Size", colWidth * 6, rowHeight * 4.5, colWidth * 2, rowHeight * 2,
        juce::Justification::left, true);
    g.drawText( "Damp", colWidth * 7.8, rowHeight * 4.5, colWidth * 2, rowHeight * 2,
        juce::Justification::left, true);
    g.drawText( "Width", colWidth * 9.8, rowHeight * 4.5, colWidth * 2, rowHeight * 2,
        juce::Justification::left, true);
    g.drawText("Wet", colWidth * 6, rowHeight * 6.5, colWidth * 2, rowHeight * 2,
        juce::Justification::left, true);
    g.drawText("Dry", colWidth * 8, rowHeight * 6.5, colWidth * 2, rowHeight * 2,
        juce::Justification::left, true);
    g.drawText("Freeze", colWidth * 9.7, rowHeight * 6.5, colWidth * 2, rowHeight * 2,
        juce::Justification::left, true);

    g.drawText("Track Position", getWidth()/2.3, rowHeight*14.5, getWidth()*2/14, rowHeight,
        juce::Justification::left, true);
}

void DeckGUI::resized()
{
    double rowHeight = getHeight()/16;
    double colWidth = getWidth() / 8;

    // Screen row
    waveformDisplay.setBounds(colWidth*1.7, rowHeight*1.8, colWidth*4.6, rowHeight*2);

    // Controls row
    colWidth = getWidth() / 14; // overwride for the current needs
    volSlider.setBounds(colWidth, rowHeight * 4.5, colWidth, rowHeight*3);
    speedSlider.setBounds(colWidth*3, rowHeight * 4.5, colWidth, rowHeight * 3);

    // Reverb controls
    reverbStateButton.setBounds(colWidth*8.5, rowHeight * 8.3, colWidth, rowHeight);
    // Forst row 
    roomSizSlider.setBounds(colWidth*6, rowHeight * 4.5, colWidth * 2, rowHeight * 2);
    dampingSlider.setBounds(colWidth * 8, rowHeight * 4.5, colWidth * 2, rowHeight * 2);
    widthSlider.setBounds(colWidth * 10, rowHeight * 4.5, colWidth * 2, rowHeight * 2);
    // Second row
    wetLvlSlider.setBounds(colWidth * 6, rowHeight * 6.5, colWidth * 2, rowHeight * 2);
    dryLvlSlider.setBounds(colWidth * 8, rowHeight * 6.5, colWidth * 2, rowHeight * 2);
    frezzeSlider.setBounds(colWidth * 10, rowHeight * 6.5, colWidth * 2, rowHeight * 2);

    // Button row
    colWidth = getWidth() / 17; // overwride for the current needs
    playButton.setBounds(colWidth, rowHeight * 9.8, colWidth*3, rowHeight*1.5);
    pauseButton.setBounds(colWidth * 5, rowHeight * 9.8, colWidth * 3, rowHeight * 1.5);
    stopButton.setBounds(colWidth*9, rowHeight * 9.8, colWidth * 3, rowHeight * 1.5);
    loadButton.setBounds(colWidth * 13, rowHeight * 9.8, colWidth * 3, rowHeight * 1.5);

    // Position row 
    colWidth = getWidth() / 3; // overwride for the current needs
    posSlider.setBounds(colWidth, rowHeight * 12, colWidth, rowHeight*3);
}

void DeckGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton)
    {
        player->start();
        playButton.setToggleState(true, false);
        stopButton.setToggleState(false, false);
        pauseButton.setToggleState(false, false);
    }
    if (button == &stopButton)
    {
        player->setPositionRelative(0.0f);
        player->stop();
        playButton.setToggleState(false, false);
        stopButton.setToggleState(true, false);
        pauseButton.setToggleState(false, false);
    }
    if (button == &pauseButton)
    {
        player->stop();
        playButton.setToggleState(false, false);
        pauseButton.setToggleState(true, false);
        stopButton.setToggleState(false, false);
    }
    if (button == &loadButton)
    {
        juce::FileChooser chooser{ "Select a file " };
        if (chooser.browseForFileToOpen())
        {
            loadAudioFile(juce::URL{ chooser.getResult() });
        }
    }
    if (button == &reverbStateButton)
    {
        if (player->reverbStatus == "on")
        {
            reverbStateButton.setToggleState(false, false);
            player->reverbStatus = "off";
        }
        else
        {
            reverbStateButton.setToggleState(true, false);
            player->reverbStatus = "on";
        }
    }
}

void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }

    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }

    if (slider == &roomSizSlider)
    {
        player->setParameter(slider->getValue(), "size");
    }
    if (slider == &dampingSlider)
    {
        player->setParameter(slider->getValue(), "damp");
    }
    if (slider == &widthSlider)
    {
        player->setParameter(slider->getValue(), "width");
    }
    if (slider == &wetLvlSlider)
    {
        player->setParameter(slider->getValue(), "wet");
    }
    if (slider == &dryLvlSlider)
    {
        player->setParameter(slider->getValue(), "dry");
    }
    if (slider == &frezzeSlider)
    {
        player->setParameter(slider->getValue(), "freeze");
    }
}

bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files)
{
    return true;
}

void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y)
{
    if (files.size() == 1)
    {
        player->loadURL(juce::URL{ juce::File{files[0]} });
    }
}

void DeckGUI::timerCallback()
{
    double newValue = player->getPositionRelative();
    waveformDisplay.setPositionRelative(newValue);

    if (newValue <= 1.0 && newValue >= 0.0)
    {
        posSlider.setValue(player->getPositionRelative());
    }
}

void DeckGUI::loadAudioFile(juce::URL fileURL)
{
    player->loadURL(fileURL);
    waveformDisplay.loadURL(fileURL);

    playButton.setToggleState(false, false);
    stopButton.setToggleState(true, false);
    pauseButton.setToggleState(false, false);

    currTrackName = fileURL.toString(false);
    currTrackName = currTrackName.substring(currTrackName.lastIndexOfChar('/') + 1, currTrackName.lastIndexOfChar('.'));

    repaint();
}