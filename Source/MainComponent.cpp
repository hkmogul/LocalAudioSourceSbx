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
	

	player = Avatar(this,0.5f,0.5f,0.01f,45);

	addAndMakeVisible(arrow);
	// get position based on relative bounds
	arrow.setImage(player.img());
	


	//images = Array<ImageComponent>()
	
	//getTopLevelComponent()->addKeyListener(this);
	addKeyListener(&player);
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
		auto baseDir = file.getParentDirectory().getFullPathName();
		DBG(baseDir);
		
		auto root = JSON::parse(file);
		var invalidSource;
		// TODO: set background image and player image
		auto sources = root["Sources"];
		Array<var> *allSources = nullptr;
		if (sources != invalidSource)
		{
			allSources = sources.getArray();
			if (allSources != nullptr)
			{
				for (auto oneSource : *allSources)
				{
					// load the audio sources
					auto *temp = new LocalAudioSource(oneSource, baseDir);
					audioSourceRegistry.push_back(temp);
				}
			}
		}

	}
	else
	{
		// force creation of a default

		map<String, String> propMap;
		String baseDir = "C:\\JUCE\\sbx\\LocalAudioSourceSbx";
		String rootFolder = "Assets";
		propMap["AudioFile"] = "defaultSong.mp3";
		propMap["ImageFile"] = "defaultImage.jpg";
		propMap["XPosition"] = "0.1";
		propMap["YPosition"] = "0.7";
		propMap["Radius"] = "0.5";
		propMap["RootFolder"] = rootFolder;
		
		
		auto *temp = new LocalAudioSource(propMap, baseDir);
		audioSourceRegistry.push_back(temp);
		// TODO: can't use files that are already in play- can we preload and copy them into memory?
		//propMap["AudioFile"] = "C:\\Users\\hilar\\Desktop\\billyIdol.mp3";
		//propMap["ImageFile"] = "C:\\Users\\hilar\\Desktop\\coffee-pixels.png";
		//propMap["XPosition"] = "0.8";
		//auto *temp2 = new LocalAudioSource(propMap);
		//audioSourceRegistry.push_back(temp2);


		//propMap["AudioFile"] = "C:\\Users\\hilar\\Desktop\\nsync.mp3";
		//propMap["ImageFile"] = "C:\\Users\\hilar\\Desktop\\coffee-pixels.png";
		//propMap["XPosition"] = "0.5";
		//propMap["YPosition"] = "0.2";
		//auto *temp3 = new LocalAudioSource(propMap);
		//audioSourceRegistry.push_back(temp3);
	}
	
	this->leftSources = vector<AudioSampleBuffer>(audioSourceRegistry.size());
	this->rightSources = vector<AudioSampleBuffer>(audioSourceRegistry.size());

	// now that we have the registry, prepare them with the sample rate so the transport source doesn't fail
	for (auto iter = audioSourceRegistry.begin(); iter != audioSourceRegistry.end(); ++iter)
	{
		if (*iter != nullptr)
		{
			(*iter)->prepareFilters(sampleRate, samplesPerBlockExpected);
			addAndMakeVisible((*iter)->imageComponent());
		}

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
	//vector<future<void>> calcTasks(audioSourceRegistry.size());
	int sourceIndex = 0;
	
	for (auto iter = audioSourceRegistry.begin(); iter != audioSourceRegistry.end(); ++iter)
	{
		auto val = *iter;
		if (val->objectInRange(player.getPosition(), player.theta()))
		{
			// iterate through to get the right number
			leftSources[sourceIndex] = AudioSampleBuffer(1, bufferToFill.buffer->getNumSamples());
			rightSources[sourceIndex] = AudioSampleBuffer(1, bufferToFill.buffer->getNumSamples());
			val->populateNextAudioBlock(leftSources[sourceIndex], rightSources[sourceIndex], bufferToFill.buffer->getNumSamples());
			// mark this index as one to copy in
			relevantIndices.push_back(sourceIndex);
		}
		else
		{
			val->discardNextAudioBlock(bufferToFill.buffer->getNumSamples());
		}

		sourceIndex++;

	}

	// make sure all the tasks are completed
	//for (auto& it : calcTasks)
	//{
	//	it.get();
	//}
	

	if (sourceIndex > 0)
	{
		auto lPtr = bufferToFill.buffer->getWritePointer(0);
		auto rPtr = bufferToFill.buffer->getWritePointer(1);
		for (int i = 0; i < bufferToFill.buffer->getNumSamples(); i++)
		{
			// iterate through indices that would have values, normalize based on the size of the index array
			for (auto it = relevantIndices.begin(); it != relevantIndices.end(); it++)
			{
				lPtr[i] += leftSources[*it].getReadPointer(0)[i] / relevantIndices.size();
				rPtr[i] += rightSources[*it].getReadPointer(0)[i] / relevantIndices.size();
			}
		}
	}
	else
	{
		bufferToFill.clearActiveBufferRegion();
	}
}

void MainComponent::releaseResources()
{
	// the ptrs for the audio source registry were dynamically allocated to keep them in scope
	for (auto iter = audioSourceRegistry.begin(); iter != audioSourceRegistry.end(); ++iter)
	{
		if (*iter != nullptr)
		{
			delete *iter;
		}
	}
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
			// this would happen if it failed to allocate the memory
			continue;
		}
		if (val->imageComponent() != nullptr)
		{
			auto position = val->position();
			auto test = val->imageComponent();
			test->setBoundsRelative(position.getX(), position.getY(), 0.05f, 0.05f);
		}
		else
		{
			Logger::getCurrentLogger()->writeToLog("IT HAS NO IMAGE?!?!");
			// probably should paint a dot there
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
		if (val->imageComponent() != nullptr)
		{
			auto position = val->position();
			auto test = val->imageComponent();
			test->setBoundsRelative(position.getX(), position.getY(), 0.05f, 0.05f);
		}
		else
		{
		}

	}
	
}


// global functions to call once I make this work
void callPopulateFunction(LocalAudioSource * src, vector<AudioSampleBuffer>& lBuf, vector<AudioSampleBuffer>& rBuf,int index, int numSamples)
{
	src->populateNextAudioBlock(lBuf[index], rBuf[index], numSamples);
}

void callDiscardFunction(LocalAudioSource *src, int numSamples)
{
	src->discardNextAudioBlock(numSamples);
}