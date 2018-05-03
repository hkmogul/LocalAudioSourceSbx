#include "ISpatialAudioEngine.h"

using namespace SpatialAudio;
using namespace std;

SpatialAudioEngine::SpatialAudioEngine(Component * parentCpt, bool avatarVisible, bool componentVisible) : parentCpt(parentCpt), componentsVisible(componentVisible), avatarVisible(avatarVisible)
{
	player = Avatar(parentCpt, 0.5f, 0.5f, 0.01f, 45);
	if (avatarVisible)
	{
		parentCpt->addAndMakeVisible(arrow);
		arrow.setImage(player.img());

	}
	parentCpt->addKeyListener(&player);
	// get position based on relative bounds
	
	parentCpt->setWantsKeyboardFocus(true);
	parentCpt->setSize(600, 600);

	FileChooser chooser("Select a JSON file to play...", File::nonexistent, "*.json");
	if (chooser.browseForFileToOpen())
	{
		File file(chooser.getResult());
		// get filename
		auto base = file.getParentDirectory();
		auto baseDir = file.getParentDirectory().getFullPathName();

		auto root = JSON::parse(file);
		// JUCE's JSON handling requires a null object to compare it against
		var invalidSource;
		// get the background image
		if (root["bgImage"] != invalidSource)
		{
			String path = root["bgImage"].toString();
			bgImage = ImageFileFormat::loadFrom(base.getChildFile(path));
		}



		auto sources = root["Sources"];
		Array<var> *allSources = nullptr;
		if (sources != invalidSource)
		{
			allSources = sources.getArray();
			if (allSources != nullptr)
			{
				// iterate through sources, attempt to parse
				for (auto oneSource : *allSources)
				{
					// load the audio sources
					auto *temp = new LocalAudioSource(oneSource, baseDir);
					audioSourceRegistry.push_back(temp);
				}
			}
		}
	}
	else
	{
		// force creation of a default

		map<String, String> propMap;
		String baseDir = "C:\\JUCE\\sbx\\LocalAudioSourceSbx\\Assets\\SingleSourceDemo";
		String rootFolder = "Assets";
		propMap["AudioFile"] = "defaultSong.mp3";
		propMap["ImageFile"] = "defaultImage.jpg";
		propMap["XPosition"] = "0.1";
		propMap["YPosition"] = "0.7";
		propMap["Radius"] = "0.5";
		propMap["RootFolder"] = rootFolder;


		auto *temp = new LocalAudioSource(propMap, baseDir);
		audioSourceRegistry.push_back(temp);
	}

	this->leftSources = vector<AudioSampleBuffer>(audioSourceRegistry.size());
	this->rightSources = vector<AudioSampleBuffer>(audioSourceRegistry.size());
}

SpatialAudioEngine::~SpatialAudioEngine()
{
	// everything has deleters except for the registry
	for (auto iter = audioSourceRegistry.begin(); iter != audioSourceRegistry.end(); ++iter)
	{
		if (*iter != nullptr)
		{
			delete *iter;
		}
	}
}

void SpatialAudio::SpatialAudioEngine::getNextAudioBlock(const AudioSourceChannelInfo & bufferToFill)
{
	bufferToFill.clearActiveBufferRegion();

	leftSources = vector<AudioSampleBuffer>(audioSourceRegistry.size());
	rightSources = vector<AudioSampleBuffer>(audioSourceRegistry.size());
	vector<int> relevantIndices;
	int sourceIndex = 0;

	for (auto iter = audioSourceRegistry.begin(); iter != audioSourceRegistry.end(); ++iter)
	{
		auto val = *iter;
		if (val->objectInRange(player.getPosition(), player.theta()))
		{
			// iterate through to get the right number
			leftSources[sourceIndex] = AudioSampleBuffer(1, bufferToFill.buffer->getNumSamples());
			rightSources[sourceIndex] = AudioSampleBuffer(1, bufferToFill.buffer->getNumSamples());
			val->populateNextAudioBlock(leftSources[sourceIndex], rightSources[sourceIndex], bufferToFill.buffer->getNumSamples());
			// mark this index as one to copy in
			relevantIndices.push_back(sourceIndex);
		}
		else
		{
			val->discardNextAudioBlock(bufferToFill.buffer->getNumSamples());
		}

		sourceIndex++;
	}

	if (sourceIndex > 0)
	{
		auto lPtr = bufferToFill.buffer->getWritePointer(0);
		auto rPtr = bufferToFill.buffer->getWritePointer(1);
		for (int i = 0; i < bufferToFill.buffer->getNumSamples(); i++)
		{
			// iterate through indices that would have values, normalize based on the size of the index array
			for (auto it = relevantIndices.begin(); it != relevantIndices.end(); it++)
			{
				lPtr[i] += leftSources[*it].getReadPointer(0)[i] / relevantIndices.size();
				rPtr[i] += rightSources[*it].getReadPointer(0)[i] / relevantIndices.size();
			}
		}
	}
	else
	{
		bufferToFill.clearActiveBufferRegion();
	}
}

void SpatialAudio::SpatialAudioEngine::prepare(int samplingRate, int samplesPerBlockExpected)
{

	prepareAndAddComponents(samplingRate, samplesPerBlockExpected);

}

void SpatialAudio::SpatialAudioEngine::paint(Graphics & g)
{
	if (bgImage.isValid())
	{
		g.drawImageAt(bgImage, 0, 0);
	}
	else
	{
		g.fillAll(parentCpt->getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
	}

	if (componentsVisible)
	{
		// iterate through locations
		for (auto iter = audioSourceRegistry.begin(); iter != audioSourceRegistry.end(); ++iter)
		{
			auto val = *iter;
			if (val == nullptr)
			{
				// this would happen if it failed to allocate the memory
				continue;
			}
			if (val->imageComponent() != nullptr)
			{
				// shift location based on the radius
				auto basePosition = val->position();
				float newX = basePosition.getX() -val->getRadius() / 2;
				float newY = basePosition.getY() -val->getRadius() / 2;
				auto test = val->imageComponent();
				test->setAlpha(1 - val->getRadius());
				test->setBoundsRelative(newX, newY, val->getRadius(), val->getRadius());
				//test->setBoundsRelative(newX, newY, 0.05f, 0.05f);
				

			}
			else
			{
				Logger::getCurrentLogger()->writeToLog("IT HAS NO IMAGE?!?!");
			}
		}
	}

	if (avatarVisible)
	{
		arrow.toFront(false);
		auto pos = player.getPosition();
		// should only attempt this if a rotation was done
		arrow.setBoundsRelative(pos.getX() - 0.025f, pos.getY() - 0.025f, 0.05f, 0.05f);
		auto bounds = arrow.getBounds();
		arrow.setTransform(
			AffineTransform::identity.rotated(
				degreesToRadians(player.theta()),
				(float)bounds.getCentreX(),
				(float)bounds.getCentreY()));
	}
}

void SpatialAudio::SpatialAudioEngine::prepareAndAddComponents(int samplingRate, int samplesPerBlockExpected)
{
	// add the GUI components, and prepare them
	for (auto iter = audioSourceRegistry.begin(); iter != audioSourceRegistry.end(); ++iter)
	{
		if (*iter != nullptr)
		{
			(*iter)->prepareFilters(samplingRate, samplesPerBlockExpected);
			if (componentsVisible)
			{
				parentCpt->addAndMakeVisible((*iter)->imageComponent());
			}
		}
	}
}
