/*
  ==============================================================================

    TabsManager.h
    Created: 5 Aug 2022 10:15:51am
    Author:  Janko Bauer

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"

//==============================================================================
class TabsManager  : public juce::TabbedComponent
{
    public:
        // Initiate the tabs and pass pointers to what will the the content of the tabs
        TabsManager(DeckGUI* DeckGUI1, DeckGUI* DeckGUI2, PlaylistComponent* playlistComponet);

        // Tabs destructor, destroys the tab component once the app closes
        ~TabsManager() override;

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TabsManager)
};
