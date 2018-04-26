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
const char* L_ROOTFOLDERPROP = "RootFolder";
using namespace juce;
using namespace std;
using namespace SpatialAudio;
static int global_id = 0;
const float ONEOVERFIFTEEN = 1.0f / 15.0f;
SpatialAudio::LocalAudioSource::LocalAudioSource()
{
	isReady = false;

}
LocalAudioSource::LocalAudioSource(std::map<juce::String, juce::String> propertyDict, juce::String baseDir)
{
	// parse props from LocalAudioSource, then initialize using other constructor
	juce::String aFile = "";
	juce::String iFile = "";
	juce::String rootFolder = "";
	File rf;
	float x = -1.0;
	float y = -1.0;
	float r = -1.0;
	// find root folder to prepend with first
	if (propertyDict.find(L_ROOTFOLDERPROP) != propertyDict.end())
	{
		if (baseDir == "")
		{
			rf = File::getCurrentWorkingDirectory().getChildFile(propertyDict[L_ROOTFOLDERPROP]);
		}
		else
		{
			rf = File(baseDir).getChildFile(propertyDict[L_ROOTFOLDERPROP]);
		}
	}
	else
	{
		// no root folder to find assets? use the given folder
		if (baseDir != "")
		{
			rf = File(baseDir);
		}
		else
		{
			rf = File::getCurrentWorkingDirectory();
		}
	}

	if (propertyDict.find(L_AUDIOFILENAMEPROP) != propertyDict.end())
	{

		aFile = rf.getChildFile(propertyDict[L_AUDIOFILENAMEPROP]).getFullPathName();
	}

	if (propertyDict.find(L_IMAGEFILENAMEPROP) != propertyDict.end())
	{
		iFile = rf.getChildFile(propertyDict[L_IMAGEFILENAMEPROP]).getFullPathName();
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
		init(aFile, iFile, x, y, r, global_id++);
	}

}

LocalAudioSource::LocalAudioSource(
	juce::String audioFileName, 
	juce::String imageFileName, 
	float xPos, 
	float yPos, 
	float radius, 
	int id,
	juce::String baseDir) 
{
	init(audioFileName, imageFileName, xPos, yPos, radius, id);
}

LocalAudioSource::LocalAudioSource(const juce::var val, juce::String baseDir)
{
	juce::String aFile = "";
	juce::String iFile = "";
	File rf;
	float x = -1.0;
	float y = -1.0;
	float r = -1.0;
	var invalidVar;
	if (val[L_ROOTFOLDERPROP] != invalidVar)
	{
		if (baseDir == "")
		{
			// use execution directory
			rf = File::getCurrentWorkingDirectory().getChildFile(val[L_ROOTFOLDERPROP].toString());
		}
		else
		{
			rf = File(baseDir).getChildFile(val[L_ROOTFOLDERPROP].toString());
		}
	}
	else
	{
		if (baseDir != "")
		{
			rf = File(baseDir);
		}
		else
		{
			// current working directory
			rf = File::getCurrentWorkingDirectory();
		}
	}

	if (val[L_AUDIOFILENAMEPROP] != invalidVar)
	{

		aFile = rf.getChildFile(val[L_AUDIOFILENAMEPROP].toString()).getFullPathName();
	}

	if (val[L_IMAGEFILENAMEPROP] != invalidVar)
	{
		iFile = rf.getChildFile(val[L_IMAGEFILENAMEPROP].toString()).getFullPathName();
	}

	if (val[L_XPOSITIONPROP] != invalidVar)
	{
		x = val[L_XPOSITIONPROP];
	}

	if (val[L_YPOSITIONPROP] != invalidVar)
	{
		y = val[L_YPOSITIONPROP];
	}

	if (val[L_RADIUSPROP] != invalidVar)
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
		init(aFile, iFile, x, y, r, global_id++);
	}
}


bool LocalAudioSource::objectInRange(Point<float> avatarPosition, float avatarAngle)
{
	m_distance = m_position.getDistanceFrom(avatarPosition);
	// within bounds
	if (m_distance <= m_radius)
	{
		float distRatio = m_distance / m_radius; // ranges from 0 to 1.  the closer it is (lower distanceRatio, we want a higher gain)
		m_gain = distRatio == 0 ? 1 : 1 - pow(distRatio, 2);
		String m;
		m_transportSource.setGain(m_gain);

		float thetaRadians = avatarPosition.getAngleToPoint(m_position);
		
		float thetaDegrees = radiansToDegrees(thetaRadians) + 270;
		if (thetaDegrees < 0)
		{
			thetaDegrees = 360 + thetaDegrees;
		}

		// use quadrant of avatar relative to source
		// Quadrant I 
		if (avatarPosition.getX() >= m_position.getX() && avatarPosition.getY() < m_position.getY())
		{
			// do nothing
		}
		// quadrant II or IV
		else if ((avatarPosition.getX() <= m_position.getX() && avatarPosition.getY() < m_position.getY()) ||
			avatarPosition.getX() >= m_position.getX() && avatarPosition.getY() > m_position.getY())
		{
			thetaDegrees = thetaDegrees + 180;
		}
	

		int thetaTemp = ((int)(thetaDegrees + avatarAngle) % 360);
		// to handle rollover
		int thetaInd = (int)round(thetaTemp * ONEOVERFIFTEEN) % 24; 
		if (thetaInd != currentThetaInd)
		{
			if (thetaInd < 0)
			{
				// break here
				DBG("Somehow calculated a bad index");
				thetaInd = 0;
				if (currentThetaInd < 0) {}
			}
			currentThetaInd = thetaInd;
			/*m << "Theta in radians is PI * " << (thetaRadians + degreesToRadians(avatarAngle)) / 3.14159;
			Logger::getCurrentLogger()->writeToLog(m);
			String m1;
			m1 << "Current theta index is " << currentThetaInd << " , theta is " << thetaTemp; 
			Logger::getCurrentLogger()->writeToLog(m1);*/
			*(m_lFIR.coefficients) = dsp::FIR::Coefficients<float>(leftIR[currentThetaInd], (size_t) L_IRLEN);
			*(m_rFIR.coefficients) = dsp::FIR::Coefficients<float>(rightIR[currentThetaInd], (size_t)R_IRLEN);

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
	}
	else if (isReady)
	{
		discardNextAudioBlock(numSamples);
	}
	else
	{
		// do something i just want it to break here
		DBG("whaaaa");
	}
}


void LocalAudioSource::discardNextAudioBlock(int numSamples)
{
	// throw out a certain number of samples, instead of calculating the time skip (especially if we're in a loop section)
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

	this->init(rhs.audioFile(), rhs.imageFile(), rhs.position().getX(), rhs.position().getY(), rhs.radius(),rhs.id());
	return *this;
}

LocalAudioSource::LocalAudioSource(LocalAudioSource & other)
{
	init(other.audioFile(), other.imageFile(), other.position().getX(), other.position().getY(), other.radius(), other.id());
}

void SpatialAudio::LocalAudioSource::prepareFilters(double samplingRate, double samplesPerBlockExpected)
{
	dsp::ProcessSpec spec;
	spec.numChannels = 1; // the filters are mono
	spec.maximumBlockSize = (int)samplesPerBlockExpected;
	spec.sampleRate = samplingRate;
	m_transportSource.prepareToPlay((int)samplesPerBlockExpected, (int)samplingRate);

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
