/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/
#pragma once
#include "MainComponent.h"
#include <map>
#include <vector>
#include "constants.h"
using namespace SpatialAudio;
using namespace std;
//==============================================================================
MainComponent::MainComponent()
{
	audioSourceRegistry.clear();
    // Make sure you set the size of the component after
    // you add any child components.

    // specify the number of input and output channels that we want to open
    setAudioChannels (0, 2);
	

	player = Avatar(0.5f,0.5f,0.01f,45);

	addAndMakeVisible(arrow);
	// get position based on relative bounds
	arrow.setImage(player.img());
	

	// read from the JSON and register the different LocalAudioSource objects
	// lets see if it will let us use filechooser in the constructor
	FileChooser chooser("Select a JSON file to play...", File::nonexistent, "*.json");
	if (chooser.browseForFileToOpen())
	{
		File file(chooser.getResult());
		auto root = JSON::parse(file);
		auto allSources = root.getProperty(SOURCES_KEY, var()).getArray();
		// initialize the imagecomponents array
		int count = 0;
		for (auto oneSource : *allSources)
		{
			// load the audio sources
			auto *temp = new LocalAudioSource(oneSource);
			audioSourceRegistry.push_back(temp);
			addAndMakeVisible((audioSourceRegistry.back()->m_imageComponent));
			count++;
		}
	}
	else
	{
		// force creation of a default

		//testSource.setImage(temp.img());
	}
	map<String, String> propMap;
	propMap["AudioFile"] = "C:\\JUCE\\sbx\\LocalAudioSourceSbx\\Assets\\defaultSong.mp3";
	propMap["ImageFile"] = "C:\\JUCE\\sbx\\LocalAudioSourceSbx\\Assets\\defaultImage.jpg";
	propMap["XPosition"] = "0.1";
	propMap["YPosition"] = "0.7";
	propMap["Radius"] = "0.2";

	//addAndMakeVisible(testSource);
	auto *temp = new LocalAudioSource(propMap);
	audioSourceRegistry.push_back(temp);
	addAndMakeVisible((audioSourceRegistry.back()->m_imageComponent.get()));
	//images = Array<ImageComponent>()
	
	//getTopLevelComponent()->addKeyListener(this);
	addKeyListener(this);
	setWantsKeyboardFocus(true);
	setSize(800, 600);

}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
	for (auto iter = audioSourceRegistry.begin(); iter != audioSourceRegistry.end(); ++iter)
	{
		(*iter)->prepareFilters(sampleRate, samplesPerBlockExpected);
	}
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
	// iterate through loaded LocalAudioSource map, find which are in range, add them to the buffers
	vector<int> sourcesInRange;
	for (auto iter = audioSourceRegistry.begin(); iter != audioSourceRegistry.end(); ++iter)
	{

	}
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
	for (auto iter = audioSourceRegistry.begin(); iter != audioSourceRegistry.end(); ++iter)
	{
		if (*iter != nullptr)
		{
			delete *iter;
		}
	}
}

bool MainComponent::keyPressed(const juce::KeyPress & key, juce::Component * originatingComponent)
{
	if (key.getKeyCode() == key.leftKey)
	{
		player.rotateCounterClockwise();
	}
	else if (key.getKeyCode() == key.rightKey)
	{
		player.rotateClockwise();
	}
	else if (key.getKeyCode() == key.upKey)
	{
		player.moveU();
	}
	else if (key.getKeyCode() == key.downKey)
	{
		player.moveD();
	}

	String m;
	m << "Current angle is " << player.theta() << " degrees.";
	repaint();
	Logger::getCurrentLogger()->writeToLog(m);
	return true;
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
	auto pos = player.getPosition();
	String m;
	m << "X is " << pos.getX() << " and Y is " << pos.getY();
	Logger::getCurrentLogger()->writeToLog(m);
	// should only attempt this if a rotation was done
	arrow.setBoundsRelative(pos.getX(), pos.getY(), 0.05f, 0.05f);
	auto bounds = arrow.getBounds();
	arrow.setTransform(AffineTransform::identity.rotated(degreesToRadians(player.theta()), bounds.getCentreX(), bounds.getCentreY()));

	for (auto iter = audioSourceRegistry.begin(); iter != audioSourceRegistry.end(); ++iter)
	{
		auto val = *iter;
		if (val == nullptr)
		{
			continue;
		}
		if (val->m_imageComponent != nullptr)
		{
			auto position = val->position();
			auto test = val->m_imageComponent.get();
			test->setBoundsRelative(position.getX(), position.getY(), 0.05f, 0.05f);
		}
		else
		{
			Logger::getCurrentLogger()->writeToLog("Thing didnt have shit");
		}

	}
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
	auto pos = player.getPosition();
	String m;
	m << "X is " << pos.getX() << " and Y is " << pos.getY();
	Logger::getCurrentLogger()->writeToLog(m);
	// should only attempt this if a rotation was done
	arrow.setBoundsRelative(pos.getX(), pos.getY(), 0.05f, 0.05f);
	auto bounds = arrow.getBounds();
	arrow.setTransform(AffineTransform::identity.rotated(degreesToRadians(player.theta()), bounds.getCentreX(), bounds.getCentreY()));

	for (auto iter = audioSourceRegistry.begin(); iter != audioSourceRegistry.end(); ++iter)
	{
		auto val = *iter;
		if (val->m_imageComponent != nullptr)
		{
			auto position = val->position();
			auto test = val->m_imageComponent.get();
			test->setBoundsRelative(position.getX(), position.getY(), 0.05f, 0.05f);
		}
		else
		{
			Logger::getCurrentLogger()->writeToLog("Thing didnt have shit");
		}

	}
	
}
