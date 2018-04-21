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
#include <functional>
#include <future>
#include <asyncinfo.h>
using namespace SpatialAudio;
using namespace std;
//==============================================================================

void callPopulateFunction(LocalAudioSource * src, vector<AudioSampleBuffer>& lBuf, vector<AudioSampleBuffer>& rBuf, int index, int numSamples);
void callDiscardFunction(LocalAudioSource *src, int numSamples);
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

		map<String, String> propMap;
		propMap["AudioFile"] = "C:\\Users\\hilar\\OneDrive\\School\\ComputerAudition\\Hilary_Mogul_HW3\\pop.wav";
		propMap["ImageFile"] = "C:\\JUCE\\sbx\\LocalAudioSourceSbx\\Assets\\defaultImage.jpg";
		propMap["XPosition"] = "0.1";
		propMap["YPosition"] = "0.7";
		propMap["Radius"] = "0.5";

		//addAndMakeVisible(testSource);
		auto *temp = new LocalAudioSource(propMap);
		audioSourceRegistry.push_back(temp);

		propMap["AudioFile"] = "C:\\Users\\hilar\\Desktop\\billyIdol.mp3";
		propMap["ImageFile"] = "C:\\Users\\hilar\\Desktop\\coffee-pixels.png";
		propMap["XPosition"] = "0.8";
		auto *temp2 = new LocalAudioSource(propMap);
		audioSourceRegistry.push_back(temp2);


		propMap["AudioFile"] = "C:\\Users\\hilar\\Desktop\\nsync.mp3";
		propMap["ImageFile"] = "C:\\Users\\hilar\\Desktop\\coffee-pixels.png";
		propMap["XPosition"] = "0.5";
		propMap["YPosition"] = "0.2";
		auto *temp3 = new LocalAudioSource(propMap);
		audioSourceRegistry.push_back(temp3);
	}
	
	this->leftSources = vector<AudioSampleBuffer>(audioSourceRegistry.size());
	this->rightSources = vector<AudioSampleBuffer>(audioSourceRegistry.size());

	// now that we have the registry, prepare them with the sample rate so the transport source doesn't fail
	for (auto iter = audioSourceRegistry.begin(); iter != audioSourceRegistry.end(); ++iter)
	{
		(*iter)->prepareFilters(sampleRate, samplesPerBlockExpected);
	}
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
	// iterate through loaded LocalAudioSource map, find which are in range, add them to the buffers
	bufferToFill.clearActiveBufferRegion();

	leftSources = vector<AudioSampleBuffer>(audioSourceRegistry.size());
	rightSources = vector<AudioSampleBuffer>(audioSourceRegistry.size());
	vector<int> relevantIndices;
	// vector of async tasks to call?
	vector<future<void>> calcTasks(audioSourceRegistry.size());
	int sourceIndex = 0;
	for (auto iter = audioSourceRegistry.begin(); iter != audioSourceRegistry.end(); ++iter)
	{
		auto val = *iter;
		if (val->objectInRange(player.getPosition(), player.theta()))
		{

			leftSources[sourceIndex] = AudioSampleBuffer(1, bufferToFill.buffer->getNumSamples());
			rightSources[sourceIndex] = AudioSampleBuffer(1, bufferToFill.buffer->getNumSamples());
			//calcTasks[sourceIndex] = async(launch::async, callPopulateFunction,val, leftSources, rightSources,sourceIndex, bufferToFill.buffer->getNumSamples());
			//calcTasks[sourceIndex].valid();
			val->populateNextAudioBlock(leftSources[sourceIndex], rightSources[sourceIndex], bufferToFill.buffer->getNumSamples());
			// mark this index as one to copy in
			relevantIndices.push_back(sourceIndex);
		}
		else
		{
			//calcTasks[sourceIndex] = async(launch::async, callDiscardFunction,val, bufferToFill.buffer->getNumSamples());
			val->discardNextAudioBlock(bufferToFill.buffer->getNumSamples());
		}
		sourceIndex++;


	}

	// make sure all the tasks are completed
	//for (auto& it : calcTasks)
	//{
	//	it.get();
	//}

	// asynchronously add these values in?
	

	if (sourceIndex > 0)
	{
		auto lPtr = bufferToFill.buffer->getWritePointer(0);
		auto rPtr = bufferToFill.buffer->getWritePointer(1);
		for (int i = 0; i < bufferToFill.buffer->getNumSamples(); i++)
		{
			for (auto it = relevantIndices.begin(); it != relevantIndices.end(); it++)
			{
				lPtr[i] += leftSources[*it].getReadPointer(0)[i];
				rPtr[i] += rightSources[*it].getReadPointer(0)[i];
			}
		}
	}
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
	(void)originatingComponent; // block warnings
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
	// should only attempt this if a rotation was done
	arrow.setBoundsRelative(pos.getX(), pos.getY(), 0.05f, 0.05f);
	auto bounds = arrow.getBounds();
	arrow.setTransform(
		AffineTransform::identity.rotated(
			degreesToRadians(player.theta()), 
			(float)bounds.getCentreX(), 
			(float)bounds.getCentreY()));

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
	Logger::getCurrentLogger()->writeToLog(m);
	// should only attempt this if a rotation was done
	arrow.setBoundsRelative(pos.getX(), pos.getY(), 0.05f, 0.05f);
	auto bounds = arrow.getBounds();
	arrow.setTransform(AffineTransform::identity.rotated(degreesToRadians(player.theta()), (float)bounds.getCentreX(), (float)bounds.getCentreY()));

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

void callPopulateFunction(LocalAudioSource * src, vector<AudioSampleBuffer>& lBuf, vector<AudioSampleBuffer>& rBuf,int index, int numSamples)
{
	src->populateNextAudioBlock(lBuf[index], rBuf[index], numSamples);
}

void callDiscardFunction(LocalAudioSource *src, int numSamples)
{
	src->discardNextAudioBlock(numSamples);
}