#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <map>

namespace SpatialAudio
{
	/// Class to represent an audio source in space

	class LocalAudioSource
	{

	public:

		// default constructor
		LocalAudioSource();
		// constructor using the serialized properties
		LocalAudioSource(std::map<juce::String, juce::String> propertyDict);

		// constructor using the raw properties
		LocalAudioSource(juce::String audioFileName, juce::String imageFileName, float xPos, float yPos, float radius, int id);

		// constructor using JsonCpp value
		LocalAudioSource(const juce::var val);

		
		// detects if the avatar is in range.  sets the internal flag for it
		// NB: this should ONLY be called of the player location/orientation changes
		bool objectInRange(Point<float> avatarPosition, float theta);
		// just get the property.  do this if position has not changed
		bool objectInRange() { return inRange; };

		// if position has not changed, but orientation has
		//void updateHRTF(float theta);

		void populateNextAudioBlock(AudioSampleBuffer& leftBuffer, AudioSampleBuffer& rightBuffer, int numSamples);

		void discardNextAudioBlock(int numSamples); // if out of range, don't want it to skip around

		int id() const {
			return m_id;
		};

		juce::Image& img() { return m_img; };
		juce::String& audioFile() { return m_audioFileName; };
		juce::String& imageFile() { return m_imageFileName; };
		Point<float>& position() { return m_position; };
		float& radius() { return m_radius; };
		LocalAudioSource& operator=(const LocalAudioSource& rhs);
		LocalAudioSource(LocalAudioSource& other);
		void prepareFilters(double samplingRate, double samplesPerBlockExpected);
		ScopedPointer<ImageComponent> m_imageComponent;

	private:
		juce::Image m_img;
		AudioFormatManager m_formatManager;
		ScopedPointer<AudioFormatReaderSource> m_readerSource;
		AudioTransportSource m_transportSource;
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

		void init(juce::String audioFileName, juce::String imageFileName, float xPos, float yPos, float radius, int id);
		juce::dsp::FIR::Filter<float> m_lFIR;
		juce::dsp::FIR::Filter<float> m_rFIR;
	};
}