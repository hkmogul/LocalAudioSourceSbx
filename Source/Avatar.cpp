/*
  ==============================================================================

    Avatar.cpp
    Created: 17 Apr 2018 11:27:50am
    Author:  hilar

  ==============================================================================
*/

#include "Avatar.h"
using namespace SpatialAudio;
using namespace std;
Avatar::Avatar()
{
}

Avatar::Avatar(float xInit, float yInit, float velocity, float thetaInit) 
	: xPos(xInit),yPos(yInit),vel(velocity), thetaPos(thetaInit)
{
	thetaVel = 10; // degrees
}

void SpatialAudio::Avatar::moveD()
{
	// backwards along vector

}

void SpatialAudio::Avatar::moveU()
{
}

void SpatialAudio::Avatar::moveL()
{
}

void SpatialAudio::Avatar::moveR()
{
	// find dx and dy components based on current angle
}

void SpatialAudio::Avatar::rotateClockwise()
{
	thetaPos += thetaVel;
	thetaPos = (int)thetaPos % 360;
}

void SpatialAudio::Avatar::rotateCounterClockwise()
{
	thetaPos -= thetaVel;
	thetaPos = (int)thetaPos % 360;
}
