#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <map>
namespace SpatialAudio
{
	// have a constant reference for the origin
	static const juce::Point<float> origin(0, 0);
	/// Class to represent an audio source in space

	class LocalAudioSource
	{

	public:

		// default constructor
		LocalAudioSource();
		// constructor using the serialized properties
		LocalAudioSource(std::map<juce::String, juce::String> propertyDict, juce::String baseDir = "");

		// constructor using the raw properties
		LocalAudioSource(juce::String audioFileName, juce::String imageFileName, float xPos, float yPos, float radius, int id, juce::String baseDir = "", bool shouldBypass=false, bool shouldRestart = false);

		// constructor using JsonCpp value
		LocalAudioSource(const juce::var val, juce::String baseDir = "");

		
		// detects if the avatar is in range.  sets the internal flag for it
		// NB: this should ONLY be called of the player location/orientation changes
		bool objectInRange(Point<float> avatarPosition, float theta);
		// just get the property.  do this if position has not changed
		bool objectInRange() const { return inRange; };

		// if position has not changed, but orientation has
		//void updateHRTF(float theta);

		void populateNextAudioBlock(AudioSampleBuffer& leftBuffer, AudioSampleBuffer& rightBuffer, int numSamples);
		void discardNextAudioBlock(int numSamples); // if out of range, don't want it to skip around

		int id() const {
			return m_id;
		};

		juce::Image img() const { return m_img; };
		juce::String audioFile() const { return m_audioFileName; };
		juce::String imageFile() const { return m_imageFileName; };
		ImageComponent * imageComponent() const { return m_imageComponent.get(); };
		Point<float> position() const { return m_position; };
		float radius() const { return m_radius; };
		LocalAudioSource& operator=(const LocalAudioSource& rhs);
		LocalAudioSource(LocalAudioSource& other);
		void prepareFilters(double samplingRate, double samplesPerBlockExpected);
		float getRadius() const { return m_radius; };
		float calculateAngle(const juce::Point<float> other, float playerAngle) const;

	private:
		juce::Image m_img;
		AudioFormatManager m_formatManager;
		ScopedPointer<AudioFormatReaderSource> m_readerSource;
		ScopedPointer<ImageComponent> m_imageComponent;

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
		bool firstBlockFlag; // if this is the first block since coming into range (or the first block in general- used to denote if gain needs to be ramped in)
		bool shouldRestartAudio; // if the source should restart playback when in range
		Point<float> lastReadPosition; // optimization to remove extra buffer loads if nothing changes
		float lastReadAngle;
		void angleFilterHandling(Point<float> position, float angle);

		void init(juce::String audioFileName, juce::String imageFileName, float xPos, float yPos, float radius, int id, bool shouldBypass = false, bool shouldRestart = false);
		juce::dsp::FIR::Filter<float> m_lFIR;
		juce::dsp::FIR::Filter<float> m_rFIR;
		bool shouldBypassHRTF;
	};
}