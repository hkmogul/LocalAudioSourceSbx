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
const char* L_BYPASSHRTFPROP = "BypassHRTF";
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
	bool bypass = false;
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

	if (propertyDict.find(L_BYPASSHRTFPROP) != propertyDict.end())
	{
		bypass = propertyDict[L_BYPASSHRTFPROP].getIntValue() > 0 ? true : false;
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
		init(aFile, iFile, x, y, r, global_id++, bypass);
	}

}

LocalAudioSource::LocalAudioSource(
	juce::String audioFileName, 
	juce::String imageFileName, 
	float xPos, 
	float yPos, 
	float radius, 
	int id,
	juce::String baseDir,
	bool shouldBypass) 
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
	bool bypass = false;
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

	if (val[L_BYPASSHRTFPROP] != invalidVar)
	{
		bypass = (int)val[L_BYPASSHRTFPROP] > 0 ? true : false;
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
		init(aFile, iFile, x, y, r, global_id++, bypass);
	}
}


bool LocalAudioSource::objectInRange(Point<float> avatarPosition, float avatarAngle)
{
	if ((avatarPosition == lastReadPosition && avatarAngle == lastReadAngle) ||
		(avatarPosition == lastReadPosition && !inRange))
	{
		// nothings changed, or don't bother doing filter loads when not in range
		return inRange;
	}
	else if (avatarPosition == lastReadPosition && inRange && avatarAngle != lastReadAngle)
	{
		// only angle has changed
		angleFilterHandling(avatarPosition, avatarAngle);

	}
	else
	{
		m_distance = m_position.getDistanceFrom(avatarPosition);

		// within bounds
		if (m_distance <= m_radius)
		{
			inRange = true;
			// ranges from 0 to 1.  the closer it is (lower distanceRatio, we want a higher gain)
			float distRatio = m_distance / m_radius;
			m_gain = distRatio == 0 ? 1 : 1 - pow(distRatio, 2);
			m_transportSource.setGain(m_gain);
			angleFilterHandling(avatarPosition, avatarAngle);
		}
		else
		{
			inRange = false;
			// reset ramp flag
			firstBlockFlag = true;
			m_gain = 0;
		}
	}

	lastReadAngle = avatarAngle;
	lastReadPosition = avatarPosition;
	return inRange;
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
		if (firstBlockFlag)
		{
			float stepSize = 1.0f / numSamples;
			auto *ptr = leftBuffer.getWritePointer(0);
			float scale = 0;
			for (auto i = 0; i < numSamples; i++)
			{
				ptr[i] *= scale;
				scale += stepSize;
			}

			firstBlockFlag = false;
		}

		// copy right buffer data before applying HRTF
		rightBuffer.copyFrom(0, 0, leftBuffer.getReadPointer(0), numSamples);
		if (!shouldBypassHRTF)
		{
			// turn these into blocks
			dsp::AudioBlock<float> lBufBlock(leftBuffer);
			dsp::AudioBlock<float> rBufBlock(rightBuffer);

			// turn the blocks into destructive/in place processing contexts
			dsp::ProcessContextReplacing<float> lContext(lBufBlock);
			dsp::ProcessContextReplacing<float> rContext(rBufBlock);

			m_lFIR.process(lContext);
			m_rFIR.process(rContext);
		}
		else
		{
			DBG("BYPASS WORKED");
		}

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

float SpatialAudio::LocalAudioSource::calculateAngle(const juce::Point<float> other, float playerAngle) const
{
	// create a point translated about the other
	auto translatedSource = m_position.translated(-1 * other.getX(),-1 * other.getY());

	// rotate translated source about origin (clockwise, so multiply our offset by -1)
	translatedSource = translatedSource.rotatedAboutOrigin(degreesToRadians(-1 * playerAngle));

	// juces angle calculation is clockwise. use counter clockwise to fix
	auto thetaRadians =-1* translatedSource.getAngleToPoint(origin);

	// +360 to make sure that we are in positive territory, +180 to create a phase shift to get the correct orientation
	auto degrees = (360 + 180+(int)radiansToDegrees(thetaRadians)) %360;
	/*String m;
	m << "Degrees using rotation/translation is : " << degrees;
	DBG(m);*/
	return degrees;
}

void SpatialAudio::LocalAudioSource::angleFilterHandling(Point<float> avatarPosition, float avatarAngle)
{
	float thetaDegrees = calculateAngle(avatarPosition, avatarAngle);

	// to handle rollover
	int thetaInd = (int)round(thetaDegrees * ONEOVERFIFTEEN) % 24;
	// dont bother updating the filter if its not going to be used
	if (thetaInd != currentThetaInd && !shouldBypassHRTF)
	{
		if (thetaInd < 0)
		{
			// break here
			DBG("Somehow calculated a bad index");
			DBG(thetaInd);
			thetaInd = 0;
			if (currentThetaInd < 0) {}
		}
		currentThetaInd = thetaInd;
		// copy in coefficients from a predefined header
		*(m_lFIR.coefficients) = dsp::FIR::Coefficients<float>(leftIR[currentThetaInd], (size_t)L_IRLEN);
		*(m_rFIR.coefficients) = dsp::FIR::Coefficients<float>(rightIR[currentThetaInd], (size_t)R_IRLEN);
	}
}

void SpatialAudio::LocalAudioSource::init(
	juce::String audioFileName, 
	juce::String imageFileName, 
	float xPos, 
	float yPos, 
	float radius, 
	int id,
	bool shouldBypass)
{
	shouldBypassHRTF = shouldBypass;
	firstBlockFlag = true;
	lastReadAngle = -1;
	lastReadPosition = Point<float>(0, 0);
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
