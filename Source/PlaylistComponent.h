/*
  ==============================================================================

    PlaylistComponent.h
    Created: 28 Jul 2022 8:30:24am
    Author:  Janko Bauer

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include "DeckGUI.h"

//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
    public juce::TableListBoxModel, 
    public juce::Button::Listener
    //public juce::TextEditor
    //public juce::AudioFormatReader
{
public:
    PlaylistComponent(juce::AudioFormatManager& _formatManager, DeckGUI* DeckGUI1, DeckGUI* DeckGUI2);
    ~PlaylistComponent() override;

    // Paint all cosmetic components and labels
    void paint (juce::Graphics&) override;

    // Resizes all components in the window when a change in size is detected.
    void resized() override;

    // Returns the number of rows in the XML data set 
    int getNumRows() override;

    // Paints the individual row backgrounds and when selected and not selected
    void paintRowBackground(juce::Graphics &,
                            int rowNumber, 
                            int width, 
                            int height, 
                            bool rowIsSelected) override;

    // Paints content of individual cells 
    void paintCell(juce::Graphics &, 
                   int rowNumber, 
                    int columnId, 
                    int width, 
                    int height, 
                    bool rowIsSelected) override; 

    /* Refreshes the buttons in the table according to the conditions : 
    if the field is emptyand and shows, draw the button 
    else, delete the button and set it to a nullptr*/
    Component* refreshComponentForCell(int rowNumber,
        int columnId,
        bool isRowSelected,
        Component* existingComponentToUpdate) override; 

    // Event listener for all buttons. Handles all function calls or small executions as well. 
    void buttonClicked(juce::Button* button) override;

private:

    DeckGUI* deck1;
    DeckGUI* deck2;

    void addPlaylistData(juce::File filePath);

    juce::TableListBox tableComponent;

    std::vector<std::string> trackTitles;

    juce::TextButton loadButton{ "Add Track" };
    juce::TextButton removeButton{ "Remove Track" };
    juce::TextButton clearSrchButton{ "Clear" };
    juce::TextButton searchButton{ "Search" };
    juce::TextEditor searchText{};

    juce::AudioFormatManager& formatManager;


    std::unique_ptr<juce::XmlElement> demoData;
    juce::XmlElement* columnList = nullptr; 
    juce::XmlElement* dataList = nullptr;
    int numRows;

    void loadData();

    void wirteData();

    void searchData(juce::String searchTerm);

    void resetSearch();

    juce::String getAttributeNameForColumnId(const int columnId) const;

    // Small object used to sort the rows according to the visibility
    // Used in searching
    struct TableSorter
    {
        TableSorter()
        {
        }

        int compareElements(juce::XmlElement* first, juce::XmlElement* second) const
        {
            auto result = first->getStringAttribute("Show")
                .compareNatural(second->getStringAttribute("Show"));

            return result * -1;
        }
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
