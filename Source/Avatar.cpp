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
	Avatar(0.5f, 0.5f, 0.01f, 0.0f);
}
Avatar::Avatar(float xInit = 0.5f, float yInit = 0.5f, float velocity = 0.01f, float thetaInit = 0.0f) 
	: xPos(xInit),yPos(yInit),vel(velocity), thetaPos(thetaInit), position(xInit, yInit)
{
	thetaVel = 15; // degrees

	m_img = ImageFileFormat::loadFrom(BinaryData::arrow_png, (size_t)BinaryData::arrow_pngSize);
	
	m_rotatedImage = ImageFileFormat::loadFrom(BinaryData::arrow_png, (size_t)BinaryData::arrow_pngSize);
}

void SpatialAudio::Avatar::moveD()
{
	position.setX(position.getX() - vel * sin(degreesToRadians(thetaPos)));
	position.setY(position.getY() + vel * cos(degreesToRadians(thetaPos)));
}

void SpatialAudio::Avatar::moveU()
{
	// find dx and dy components based on current angle
	position.setX(position.getX() + vel * sin(degreesToRadians(thetaPos)));
	position.setY(position.getY() - vel * cos(degreesToRadians(thetaPos)));
}

void SpatialAudio::Avatar::rotateClockwise()
{
	thetaPos += thetaVel;
	thetaPos = (int)thetaPos % 360;
	float thetaRad = degreesToRadians(thetaPos);
	
}

void SpatialAudio::Avatar::rotateCounterClockwise()
{
	thetaPos -= thetaVel;
	thetaPos = (int)thetaPos % 360;
	float thetaRad = degreesToRadians(thetaPos);
}


Image& Avatar::img()
{
	return m_img;
}