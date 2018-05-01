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
MainComponent::MainComponent() :engine(this, true, true)
{
    // specify the number of input and output channels that we want to open
	setAudioChannels(0, 2);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
	engine.prepare(sampleRate, samplesPerBlockExpected);
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
	engine.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
}


//==============================================================================
void MainComponent::paint (Graphics& g)
{
	engine.paint(g);
}

void MainComponent::resized()
{
	repaint();
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