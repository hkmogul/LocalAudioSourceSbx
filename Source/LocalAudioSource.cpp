#pragma once
#include "LocalAudioSource.h"
#include <math.h>
#include <functional>
#include <string>
#include "leftIR.h"
#include "rightIR.h"
const char* L_AUDIOFILENAMEPROP = "AudioFile";
const char* L_IMAGEFILENAMEPROP = "ImageFile";
const char* L_XPOSITIONPROP = "XPosition";
const char* L_YPOSITIONPROP = "YPosition";
const char* L_RADIUSPROP = "Radius";
using namespace juce;
using namespace std;
using namespace SpatialAudio;
static int global_id = 0;
const float ONEOVERFIFTEEN = 1.0f / 15.0f;
SpatialAudio::LocalAudioSource::LocalAudioSource()
{
	isReady = false;

}
LocalAudioSource::LocalAudioSource(std::map<juce::String, juce::String> propertyDict)
{
	// parse props from LocalAudioSource, then initialize using other constructor
	juce::String aFile = "";
	juce::String iFile = "";
	float x = -1.0;
	float y = -1.0;
	float r = -1.0;
	if (propertyDict.find(L_AUDIOFILENAMEPROP) != propertyDict.end())
	{
		aFile = propertyDict[L_AUDIOFILENAMEPROP];
	}

	if (propertyDict.find(L_IMAGEFILENAMEPROP) != propertyDict.end())
	{
		iFile = propertyDict[L_IMAGEFILENAMEPROP];
	}

	if (propertyDict.find(L_RADIUSPROP) != propertyDict.end())
	{
		r = propertyDict[L_RADIUSPROP].getFloatValue();
	}

	if (propertyDict.find(L_XPOSITIONPROP) != propertyDict.end())
	{
		x = propertyDict[L_XPOSITIONPROP].getFloatValue();
	}

	if (propertyDict.find(L_YPOSITIONPROP) != propertyDict.end())
	{
		y = propertyDict[L_YPOSITIONPROP].getFloatValue();
	}

	// necessary components are xposition yposition and audio file names.  check if these are nonnull before passing to explicit constructor
	if (aFile.isEmpty() || x < 0 || y < 0)
	{
		isReady = false;
		return;
	}
	else
	{
		// use explicit constructor
		// TODO: maybe UUIDs instead?
		init(aFile, iFile, x, y, r, global_id++);
	}

}

LocalAudioSource::LocalAudioSource(
	juce::String audioFileName, 
	juce::String imageFileName, 
	float xPos, 
	float yPos, 
	float radius, 
	int id) 
{
	init(audioFileName, imageFileName, xPos, yPos, radius, id);
}

LocalAudioSource::LocalAudioSource(const juce::var val)
{
	juce::String aFile = "";
	juce::String iFile = "";
	float x = -1.0;
	float y = -1.0;
	float r = -1.0;

	if (!val[L_AUDIOFILENAMEPROP] != NULL)
	{
		aFile = val[L_AUDIOFILENAMEPROP].toString();
	}

	if (!val[L_IMAGEFILENAMEPROP] != NULL)
	{
		iFile = val[L_IMAGEFILENAMEPROP].toString();
	}

	if (!val[L_XPOSITIONPROP] != NULL)
	{
		x = val[L_XPOSITIONPROP];
	}

	if (!val[L_YPOSITIONPROP] != NULL)
	{
		y = val[L_YPOSITIONPROP];
	}

	if (!val[L_RADIUSPROP] != NULL)
	{
		r = val[L_RADIUSPROP];
	}

	// necessary components are xposition yposition and audio file names.  check if these are nonnull before passing to explicit constructor
	if (aFile.isEmpty() || x < 0 || y < 0)
	{
		isReady = false;
		return;
	}
	else
	{
		// use explicit constructor
		// TODO: maybe UUIDs instead?
		LocalAudioSource(aFile, iFile, x, y, r, global_id++);
	}
}


bool LocalAudioSource::objectInRange(Point<float> avatarPosition, float theta)
{
	m_distance = m_position.getDistanceFrom(avatarPosition);
	if (m_distance <= m_radius)
	{
		float distRatio = m_distance / m_radius; // ranges from 0 to 1.  the closer it is (lower distanceRatio, we want a higher gain)
		m_gain = distRatio == 0 ? 1 : 1 - pow(distRatio, 2);
		String m;
		//m << "Gain is " << m_gain;
		m_transportSource.setGain(m_gain);

		float thetaRadians = m_position.getAngleToPoint(avatarPosition);
		float thetaDegrees = radiansToDegrees(thetaRadians);
		if (thetaDegrees < 0)
		{
			thetaDegrees = 360 + thetaDegrees;
		}
		float thetaTemp = ((int)(thetaDegrees + theta) % 360);
		// to handle rollover
		int thetaInd = (int)round(thetaTemp * ONEOVERFIFTEEN) % 24; 
		if (thetaInd != currentThetaInd)
		{
			if (thetaInd < 0)
			{
				// break here
				if (currentThetaInd < 0) {}
			}
			currentThetaInd = thetaInd;
			String m;
			m << "Current theta index is " << currentThetaInd << " , theta is " << thetaTemp; 
			Logger::getCurrentLogger()->writeToLog(m);
			*(m_lFIR.coefficients) = dsp::FIR::Coefficients<float>(leftIR[currentThetaInd], (size_t) L_IRLEN);
			*(m_rFIR.coefficients) = dsp::FIR::Coefficients<float>(rightIR[currentThetaInd], (size_t)R_IRLEN);

		}

		return true;
	}

	m_gain = 0;
	return false;
}

void addToExistingBuffer(AudioSourceChannelInfo& bufferToFill)
{
	// assume that a buffer is cleared-nonnoise already

}

void LocalAudioSource::populateNextAudioBlock(float *lBuf, float* rBuf, int numSamples)
{

}

void LocalAudioSource::populateNextAudioBlock(AudioSampleBuffer& leftBuffer, AudioSampleBuffer& rightBuffer, int numSamples)
{
	if (isReady && inRange)
	{
		if (!m_transportSource.isPlaying())
		{
			
			m_transportSource.start();
		}
		// fill in one buffer from the transport source
		AudioSourceChannelInfo info = AudioSourceChannelInfo(leftBuffer);
		info.buffer->setSize(1, numSamples, false, false, true);
		m_transportSource.getNextAudioBlock(info);

		// copy right buffer data before applying HRTF
		rightBuffer.copyFrom(0, 0, leftBuffer.getReadPointer(0), numSamples);
		// turn these into blocks
		dsp::AudioBlock<float> lBufBlock(leftBuffer);
		dsp::AudioBlock<float> rBufBlock(rightBuffer);

		// turn the blocks into destructive/in place processing contexts
		dsp::ProcessContextReplacing<float> lContext(lBufBlock);
		dsp::ProcessContextReplacing<float> rContext(rBufBlock);

		m_lFIR.process(lContext);
		m_rFIR.process(rContext);

		// then process using whatever the distance based filter will be on the same contexts

	}
	else if (isReady)
	{
		discardNextAudioBlock(numSamples);
	}
}

void SpatialAudio::LocalAudioSource::populateNextAudioBlock(const AudioSourceChannelInfo & info)
{
	if (isReady && inRange)
	{
		if (!m_transportSource.isPlaying())
		{

			m_transportSource.start();
		}
		// fill in one buffer from the transport source
		m_transportSource.getNextAudioBlock(info);
		// turn these into blocks
		float *lp = info.buffer->getWritePointer(0);
		float *rp = info.buffer->getWritePointer(1);

		dsp::AudioBlock<float> lBufBlock = dsp::AudioBlock<float>(&lp, 1, 0, info.buffer->getNumSamples());
		dsp::AudioBlock<float> rBufBlock = dsp::AudioBlock<float>(&rp, 1, 0, info.buffer->getNumSamples());

		// turn the blocks into destructive/in place processing contexts
		dsp::ProcessContextReplacing<float> lContext(lBufBlock);
		dsp::ProcessContextReplacing<float> rContext(rBufBlock);

		m_lFIR.process(lContext);
		m_rFIR.process(rContext);

		// then process using whatever the distance based filter will be on the same contexts

	}
	else if (isReady)
	{
		discardNextAudioBlock(info.buffer->getNumSamples());
	}
}

void LocalAudioSource::discardNextAudioBlock(int numSamples)
{
	AudioSampleBuffer buffer(1, numSamples);
	AudioSourceChannelInfo buff(buffer);
	m_transportSource.getNextAudioBlock(buff);
}

LocalAudioSource & SpatialAudio::LocalAudioSource::operator=(const LocalAudioSource & rhs)
{
	if (this == &rhs)
	{
		return *this;
	}

	return LocalAudioSource(this->audioFile(), this->imageFile(), this->position().getX(), this->position().getY(), this->radius(),this->id());
}

LocalAudioSource::LocalAudioSource(LocalAudioSource & other)
{
	LocalAudioSource(this->audioFile(), this->imageFile(), this->position().getX(), this->position().getY(), this->radius(), this->id());
}

void SpatialAudio::LocalAudioSource::prepareFilters(double samplingRate, double samplesPerBlockExpected)
{
	dsp::ProcessSpec spec;
	spec.numChannels = 1;
	spec.maximumBlockSize = samplesPerBlockExpected;
	spec.sampleRate = samplingRate;
	m_transportSource.prepareToPlay(samplesPerBlockExpected, samplingRate);

	m_transportSource.start();
}

void SpatialAudio::LocalAudioSource::init(
	juce::String audioFileName, 
	juce::String imageFileName, 
	float xPos, 
	float yPos, 
	float radius, 
	int id)
{
	m_id = (id);
	m_audioFileName = (audioFileName); 
	m_imageFileName = (imageFileName);
	m_radius = (radius); 
	m_position = Point<float>(xPos, yPos);

	m_formatManager.registerBasicFormats();

	// do all the audio opening parts
	File file(m_audioFileName);

	try
	{
		AudioFormatReader* reader = m_formatManager.createReaderFor(file);

		// this will happen if the format manager can't handle this file
		if (reader != nullptr)
		{
			ScopedPointer<AudioFormatReaderSource> newSource = new AudioFormatReaderSource(reader, true);
			m_transportSource.setSource(newSource, 0, nullptr, reader->sampleRate);
			m_readerSource = newSource.release();

			// ABL - Always! Be! Looping!
			m_readerSource->setLooping(true);
		}
		else
		{
			isReady = false;
			// critical audio failure
			return;
		}

		isReady = true; // everything else should be ok now- if image loading fails, will load default bitmap
		File imgFile = File(m_imageFileName);
		// FOR NOW
		if (imgFile.existsAsFile())
		{
			m_img = ImageFileFormat::loadFrom(imgFile);
		}
		else
		{
			m_img = ImageFileFormat::loadFrom(BinaryData::arrow_png, (size_t)BinaryData::arrow_pngSize);
		}
		
		m_imageComponent = ScopedPointer<ImageComponent>(new ImageComponent);
		m_imageComponent->setImage(m_img);


		// CREATE UNITY IMPULSE RESPONSE

	}
	catch (exception& e)
	{
		Logger::getCurrentLogger()->writeToLog(e.what());
		isReady = false;
	}
}
