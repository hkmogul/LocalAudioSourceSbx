/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "LocalAudioSource.h"
#include "Avatar.h"
#include <map>
#include<vector>
#include <tuple>
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent, KeyListener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
	bool keyPressed(const juce::KeyPress &key, juce::Component * originatingComponent) override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
	// have a map of the ID for each audioSourceRegistry, and the corresponding image component for it
	//std::map<int, SpatialAudio::LocalAudioSource> audioSourceRegistry;
	//juce::HashMap<int, SpatialAudio::LocalAudioSource> audioSourceRegistry;
	std::vector<SpatialAudio::LocalAudioSource> audioSourceRegistry;
	
	SpatialAudio::Avatar player;
	ImageComponent arrow;
	//juce::Array<ImageComponent> images;
	//juce::HashMap<int, ImageComponent*> images;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
