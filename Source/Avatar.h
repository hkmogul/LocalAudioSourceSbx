#pragma once
#include <JuceHeader.h>

namespace SpatialAudio
{
	class Avatar
	{
	public:
		Avatar();

		// TODO: also imagecache stuff
		Avatar(float xInit, float yInit, float velocity, float thetaInit = 0.0);

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
		void moveL();
		void moveR();

		// rotation (clockwise/counterclockwise)
		void rotateClockwise();
		void rotateCounterClockwise();

	private:
		Point<float> position;
		float xPos;
		float yPos;
		float thetaPos;
		float vel; // velocity for movement only, not rotation
		float thetaVel;
	};
}