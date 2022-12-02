/*
  ==============================================================================

    TabsManager.cpp
    Created: 5 Aug 2022 10:15:51am
    Author:  Janko Bauer

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TabsManager.h"

//==============================================================================
TabsManager::TabsManager(DeckGUI* _DeckGUI1, DeckGUI* _DeckGUI2, PlaylistComponent* _playlistComponent)
    : juce::TabbedComponent(juce::TabbedButtonBar::TabsAtTop)
{
    addTab("Deck 1", juce::Colours::darkgrey, _DeckGUI1, false);  
    addTab("Deck 2", juce::Colours::grey, _DeckGUI2, false); 
    addTab("Library", juce::Colours::white, _playlistComponent, false);
}

TabsManager::~TabsManager()
{
}