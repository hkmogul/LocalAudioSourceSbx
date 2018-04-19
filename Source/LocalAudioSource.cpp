
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
		r = stof(propertyDict[L_RADIUSPROP].toRawUTF8());
	}

	if (propertyDict.find(L_XPOSITIONPROP) != propertyDict.end())
	{
		x = stof(propertyDict[L_XPOSITIONPROP].toRawUTF8());
	}

	if (propertyDict.find(L_YPOSITIONPROP) != propertyDict.end())
	{
		y = stof(propertyDict[L_YPOSITIONPROP].toRawUTF8());
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

LocalAudioSource::LocalAudioSource(
	juce::String audioFileName, 
	juce::String imageFileName, 
	float xPos, 
	float yPos, 
	float radius, 
	int id) : m_id(id), m_audioFileName(audioFileName), m_imageFileName(imageFileName), m_radius(radius), m_position(xPos,yPos)
{
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

		m_img = ImageFileFormat::loadFrom(m_imageFileName);
		m_imageComponent->setImage(m_img);
		// prepare the filter orders
		dsp::ProcessSpec spec;
		spec.numChannels = 1;
		// TODO: sample rate and block size in constructor
		m_lFIR.prepare(spec);
		
		m_rFIR.prepare(spec);

		// CREATE UNITY IMPULSE RESPONSE

	}
	catch (exception& e)
	{
		Logger::getCurrentLogger()->writeToLog(e.what());
		isReady = false;
	}

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

		float thetaRadians = m_position.getAngleToPoint(avatarPosition);
		float thetaDegrees = radiansToDegrees(thetaRadians);
		float thetaTemp = ((int)(thetaDegrees + theta) % 360);
		int thetaInd = round(thetaTemp * ONEOVERFIFTEEN); 
		if (thetaInd != currentThetaInd)
		{
			currentThetaInd = thetaInd;
			size_t size = 30;
			*(m_lFIR.coefficients) = dsp::FIR::Coefficients<float>(leftIR[currentThetaInd], (size_t) 30);
			*(m_rFIR.coefficients) = dsp::FIR::Coefficients<float>(rightIR[currentThetaInd], (size_t)30);

		}

		return true;
	}

	m_gain = 0;
	return false;
}

void LocalAudioSource::populateNextAudioBlock(AudioSampleBuffer& leftBuffer, AudioSampleBuffer& rightBuffer, int numSamples)
{
	if (isReady && inRange)
	{
		// fill in one buffer from the transport source
		AudioSourceChannelInfo info = AudioSourceChannelInfo();
		m_transportSource.getNextAudioBlock(info);
		leftBuffer.copyFrom(0, 0, info.buffer->getReadPointer(0), numSamples);
		leftBuffer.applyGain(m_gain); // use commutative prop - only have to apply gain once

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
}
