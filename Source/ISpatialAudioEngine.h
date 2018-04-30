#pragma once
#include "JuceHeader.h"
#include "LocalAudioSource.h"
#include "Avatar.h"
#include <vector>
// interface for Spatial Audio Engine - its defaults are for the desktop scenario
// inherited classes could/should read from CLLocation and other location API's parse GPS coordinates/map homography
namespace SpatialAudio
{
	// Base class for location based binaural system creation. Uses normalized locations
	class ISpatialAudioEngine
	{
	public:
		ISpatialAudioEngine(Component *parentCpt);
		~ISpatialAudioEngine();
		void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill);
		void prepare(int samplingRate, int samplesPerBlockExpected);
		void paint(Graphics &g);
	protected:
		Component * parentCpt;
		SpatialAudio::Avatar player;
		ImageComponent arrow;
		std::vector<SpatialAudio::LocalAudioSource*> audioSourceRegistry;
		std::vector<AudioSampleBuffer> leftSources;
		std::vector<AudioSampleBuffer> rightSources;
		void prepareAndAddComponents(int samplingRate, int samplesPerBlockExpected);
	};
}


