/*
  ==============================================================================

    TabInstances.h
    Created: 5 Aug 2022 11:51:56am
    Author:  Janko Bauer

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class TabInstances  : public juce::TabbedComponent
{
public:
    TabInstances();
    ~TabInstances() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TabInstances)
};
