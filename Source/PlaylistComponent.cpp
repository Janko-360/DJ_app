/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 28 Jul 2022 8:30:24am
    Author:  Janko Bauer

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"


//==============================================================================
PlaylistComponent::PlaylistComponent(juce::AudioFormatManager& _formatManager, 
                                    DeckGUI* _DeckGUI1, DeckGUI* _DeckGUI2)
    : formatManager(_formatManager), 
      deck1(_DeckGUI1),
      deck2(_DeckGUI2)
{
    loadData();

    for (auto* columnXml : columnList->getChildIterator())
    {
        juce::String colHead = columnXml->getStringAttribute("name");
        if (colHead == "Show")
        {
            // do nothing, don't add this column
        }
        else 
        {
            tableComponent.getHeader().addColumn(colHead,
                columnXml->getIntAttribute("columnId"),
                columnXml->getIntAttribute("width"),
                50, 400,
                juce::TableHeaderComponent::defaultFlags);
        }
    }

    // Add track 
    addAndMakeVisible(loadButton);
    loadButton.addListener(this);

    // Remove track
    addAndMakeVisible(removeButton);
    removeButton.addListener(this);

    // Search field
    addAndMakeVisible(searchText);
    searchText.setJustification(juce::Justification::verticallyCentred);
    searchText.setJustification(juce::Justification::horizontallyCentred);
    searchText.setTextToShowWhenEmpty("Search term...", juce::Colours::orange);
    searchText.setColour(1, juce::Colours::lightgrey);

    // Search button
    addAndMakeVisible(searchButton);
    searchButton.addListener(this);

    // Clear search results
    addAndMakeVisible(clearSrchButton);
    clearSrchButton.addListener(this);

    // main table
    addAndMakeVisible(tableComponent);
    tableComponent.setModel(this);
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.fillAll(juce::Colours::darkgrey);

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("Playlist", getWidth() / 3, 0, getWidth()/3, getHeight() / 11,
                juce::Justification::centred, true);
}

void PlaylistComponent::resized()
{
    double rowHeight = getHeight() / 11;

    loadButton.setBounds(0, 0, getWidth()*2/9, rowHeight);
    removeButton.setBounds(getWidth()*2/9, 0, getWidth()/9, rowHeight);
    searchText.setBounds((getWidth() * 11) / 18, 0, getWidth()*2/9, rowHeight);
    searchButton.setBounds(getWidth() * 8 / 9, 0, getWidth()/9 , rowHeight);
    clearSrchButton.setBounds(getWidth() * 15 / 18, 0, getWidth() / 18, rowHeight);

    tableComponent.setBounds(0, rowHeight, getWidth(), getHeight());
}

int PlaylistComponent::getNumRows()
{
    return dataList->getNumChildElements();
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g,
                                            int rowNumber,
                                            int width,
                                            int height,
                                            bool rowIsSelected)
{
    if (dataList->getChildElement(rowNumber)->getStringAttribute("Show") == "false")
    {
        // do nothing
    }
    else
    {
        if (rowIsSelected) 
        {
            g.fillAll(juce::Colours::green);
        } 
        else {
            g.fillAll(juce::Colours::darkgrey);
        }
    }
}

void PlaylistComponent::paintCell(juce::Graphics& g,
                                    int rowNumber,
                                    int columnId,
                                    int width,
                                    int height,
                                    bool rowIsSelected)
{
    if (dataList->getChildElement(rowNumber)->getStringAttribute("Show") == "true")
    {
        g.setColour(getLookAndFeel().findColour(juce::ListBox::textColourId));

        if (auto* rowElement = dataList->getChildElement(rowNumber))
        {
            auto text = rowElement->getStringAttribute(getAttributeNameForColumnId(columnId));

            g.drawText(text, 2, 0, width - 4, height, juce::Justification::centredLeft, true);
        }

        g.setColour(getLookAndFeel().findColour(juce::ListBox::backgroundColourId));
        g.fillRect(width - 1, 0, 1, height);
    }
}

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
                                                            int columnId,
                                                            bool isRowSelected,
                                                            Component* existingComponentToUpdate)
{
    if (columnId == 7 )
    {
        if (existingComponentToUpdate == nullptr &&
            dataList->getChildElement(rowNumber)->getStringAttribute("Show") == "true")
        {
            juce::TextButton* btn = new juce::TextButton{ "Load" };
            juce::String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);
            btn->setName("Deck-1");
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
        else if(existingComponentToUpdate != nullptr &&
            dataList->getChildElement(rowNumber)->getStringAttribute("Show") == "false")
        {
            delete existingComponentToUpdate;
            existingComponentToUpdate = nullptr;
        }
    }

    if (columnId == 8 )
    {
        if (existingComponentToUpdate == nullptr && 
            dataList->getChildElement(rowNumber)->getStringAttribute("Show") == "true")
        {
            juce::TextButton* btn = new juce::TextButton{ "Load" };
            juce::String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);
            btn->setName("Deck-2");
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
        else if (dataList->getChildElement(rowNumber)->getStringAttribute("Show") == "false")
        {
            delete existingComponentToUpdate;;
            existingComponentToUpdate = nullptr;
        }
    }

    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button* button)
{
    if (button->getName() == "Deck-1")
    {
        int id = std::stoi(button->getComponentID().toStdString());
        juce::File srtPath = juce::File(dataList->getChildElement(id)->getStringAttribute("Path", "unknown"));
        deck1->loadAudioFile(juce::URL{ srtPath });
    }

    if (button->getName() == "Deck-2")
    {
        int id = std::stoi(button->getComponentID().toStdString());
        juce::File srtPath = juce::File(dataList->getChildElement(id)->getStringAttribute("Path", "unknown"));
        deck2->loadAudioFile(juce::URL{ srtPath });
    }

    if (button == &clearSrchButton)
    {
        resetSearch();
    }

     if (button == &searchButton)
    {
         searchData(searchText.getText());
    }

    if (button == &loadButton)
    {
        juce::FileChooser chooser{ "Select a file " };
        if (chooser.browseForFileToOpen())
        {
            addPlaylistData( juce::File{ chooser.getResult() });           
        }
    }

    if (button == &removeButton)
    {
        int rowNumber = tableComponent.getSelectedRow();
        dataList->removeChildElement(dataList->getChildElement(rowNumber), true );
        tableComponent.updateContent();
        wirteData();
    }
}

void PlaylistComponent::addPlaylistData(juce::File filePath)
{
    //get file name from Path
    juce::String fileName = filePath.getFileName().substring(0, filePath.getFileName().lastIndexOfChar('.'));

    // Get the last access date 
    juce::Time creationTime = filePath.getLastAccessTime();

    // "parse" the audio file for data extraction
    auto* reader = formatManager.createReaderFor(juce::URL(filePath).createInputStream(false));
    double fileSampleRate = reader->sampleRate;
    short int trackLength = reader->lengthInSamples / fileSampleRate;
    // Formatted string for length in minutes or hours. 
    juce::String strTrackLength = "";
    if (trackLength >= 3600) // trach is longer than an hour
    {
        short int seconds = trackLength % 60;
        short int minutes = ((trackLength - seconds)/60) % 60;
        short int hours = ((trackLength - minutes*60 - seconds)/60)/60;
        strTrackLength = std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds);
    }
    else {
        short int seconds = trackLength % 60;
        short int minutes = (trackLength - seconds) / 60;
        strTrackLength = std::to_string(minutes) + ":" + std::to_string(seconds);
    }

    // file type 
    juce::String fileType = filePath.getFileExtension();

    // file path 
    juce::String pathString = (filePath.getFullPathName());

    // file size 
    juce::String fileSize = juce::File::descriptionOfSizeInBytes(filePath.getSize());

    // assign the metadata to a new XmlElement
    auto newElement = new juce::XmlElement("ITEM");

    newElement->setAttribute("Song", fileName);
    newElement->setAttribute("Length", strTrackLength);
    newElement->setAttribute("kHz", fileSampleRate / 1000.0);
    newElement->setAttribute("Type", fileType);
    newElement->setAttribute("Size", fileSize);
    newElement->setAttribute("Date", 
                    creationTime.toString(true, false,
                    false, false));
    newElement->setAttribute("Deck-1", "Load");
    newElement->setAttribute("Deck-2", "Load");
    newElement->setAttribute("Path", pathString);
    newElement->setAttribute("Show", "true");

    //update the table data
    dataList->addChildElement(newElement);

    tableComponent.updateContent();

    wirteData();
}

void PlaylistComponent::loadData()
{
    std::unique_ptr<juce::InputStream> input(juce::File("data.xml").createInputStream());

    demoData = juce::parseXML(input->readString());

    dataList = demoData->getChildByName("DATA");
    columnList = demoData->getChildByName("COLUMNS");
}

juce::String PlaylistComponent::getAttributeNameForColumnId(const int columnId) const
{
    for (auto* columnXml : columnList->getChildIterator())
    {
        if (columnXml->getIntAttribute("columnId") == columnId)
            return columnXml->getStringAttribute("name");
    }

    return {};
}

void PlaylistComponent::wirteData()
{
    // write the updated table data to the xml data file
    demoData->writeTo(juce::File("data.xml"), {});
}

void PlaylistComponent::searchData(juce::String searchTerm)
{
    int numRows = getNumRows();
    for (int index = 0; index < numRows; ++index)
    {
        juce::XmlElement* currentRow = dataList->getChildElement(index);

        if (currentRow->getStringAttribute("Song").contains(searchTerm) ||
            currentRow->getStringAttribute("Date").contains(searchTerm) ||
            currentRow->getStringAttribute("Length").contains(searchTerm) ||
            currentRow->getStringAttribute("kHz").contains(searchTerm) ||
            currentRow->getStringAttribute("Path").contains(searchTerm) ||
            currentRow->getStringAttribute("Size").contains(searchTerm) ||
            currentRow->getStringAttribute("Type").contains(searchTerm))
        {
            // do nothing and keep item visible 
        }
        else
        {
            currentRow->setAttribute("Show", "false");
        }
    }
    TableSorter sorter{};

    dataList->sortChildElements(sorter);

    tableComponent.updateContent();
    repaint();
     
}

void PlaylistComponent::resetSearch()
{
    searchText.clear();

    int numRows = getNumRows();
    for (int index = 0; index < numRows; ++index)
    {
        juce::XmlElement* currentRow = dataList->getChildElement(index);

        if (currentRow->getStringAttribute("Show") == "false")
        {
            currentRow->setAttribute("Show", "true");
        }
    }
    tableComponent.updateContent();
    repaint();
}