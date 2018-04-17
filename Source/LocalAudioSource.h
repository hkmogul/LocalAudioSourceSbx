#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <map>


namespace SpatialAudio
{
	/// Class to represent an audio source in space

	class LocalAudioSource
	{

	public:
		// constructor using the serialized properties
		LocalAudioSource(std::map<juce::String, juce::String> propertyDict);

		// constructor using the raw properties
		LocalAudioSource(juce::String audioFileName, juce::String imageFileName, float xPos, float yPos, float radius, int id);

		// detects if the avatar is in range.  sets the internal flag for it
		// NB: this should ONLY be called of the player location/orientation changes
		bool objectInRange(Point<float> avatarPosition, float theta);
		// just get the property.  do this if position has not changed
		bool objectInRange() { return inRange; };

		// if position has not changed, but orientation has
		//void updateHRTF(float theta);

		void populateNextAudioBlock(AudioSampleBuffer& leftBuffer, AudioSampleBuffer& rightBuffer, int numSamples);

		void discardNextAudioBlock(int numSamples); // if out of range, don't want it to skip around

	private:
		//juce::ImageCache m_img;
		juce::Image m_img;
		AudioFormatManager m_formatManager;
		ScopedPointer<AudioFormatReaderSource> m_readerSource;
		AudioTransportSource m_transportSource;
		juce::dsp::FIR::Filter<float> m_spatialImpulseResponseLeft;
		juce::dsp::FIR::Filter<float> m_spatialImpulseResponseRight;
		juce::dsp::FIR::Filter<float> m_filterImpulseResponse;
		juce::String m_imageFileName;
		juce::String m_audioFileName;
		Point<float> m_position;
		int m_id;
		float m_gain;
		float m_radius;
		float m_distance;
		float m_theta;
		int currentThetaInd;
		bool isReady; // denotes if the audio files and images are properly loaded
		bool inRange; // denotes the avatar is in range


		juce::dsp::FIR::Filter<float> m_lFIR;
		juce::dsp::FIR::Filter<float> m_rFIR;
	};
}