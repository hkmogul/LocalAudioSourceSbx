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
	class SpatialAudioEngine
	{
	public:
		// constructor - takes in the parent component so it has somewhere to put GUI objects
		SpatialAudioEngine(Component *parentCpt);

		// deleter - mainly just deals with getting rid of the registry cleanly
		~SpatialAudioEngine();

		// maybe this one should be final...
		void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill);

		// this is important since every single transport source needs this
		void prepare(int samplingRate, int samplesPerBlockExpected);

		// do the GUI things
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


