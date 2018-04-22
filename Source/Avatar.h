#pragma once
#include <JuceHeader.h>

namespace SpatialAudio
{
	class Avatar : public juce::KeyListener
	{
	public:
		Avatar();
		Avatar(Component * ptr,float xInit, float yInit, float velocity, float thetaInit);

		// getter methods
		float x() {return xPos;};
		float y() { return yPos; };
		float theta() { return thetaPos; };
		Point<float> getPosition() { return position; };
		// setter methods
		float x(float newX) { xPos = newX; };
		float y(float newY) { yPos = newY; };
		float theta(float newTheta) { thetaPos = newTheta; };


		// motion will be defined by LRUD/WASD, with rotation changing that
		void moveD();
		void moveU();

		// rotation (clockwise/counterclockwise)
		void rotateClockwise();
		void rotateCounterClockwise();
		bool keyPressed(const juce::KeyPress & key, juce::Component * originatingComponent)override;
		Image& img();
	private:
		Point<float> position;
		float xPos;
		float yPos;
		float thetaPos;
		float vel; // velocity for movement only, not rotation
		float thetaVel;
		Image m_img; // base image
		Image m_rotatedImage;
		Component * m_cpt;
	};
}