/*
  ==============================================================================

    TabInstances.cpp
    Created: 5 Aug 2022 11:51:56am
    Author:  Janko Bauer

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TabInstances.h"





struct ButtonsPage : public juce::Component
{
    ButtonsPage()
    {

    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ButtonsPage)
};









//==============================================================================
TabInstances::TabInstances() 
    : TabbedComponent(juce::TabbedButtonBar::TabsAtTop)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.


    auto colour = juce::Colours::lightblue;

    addTab("Buttons", colour, new ButtonsPage(), true);
    addTab("Buttons2", colour, new ButtonsPage(), true);


    //addTab("Tables", colour, new TableDemoComponent(), true);


}

TabInstances::~TabInstances()
{
}

