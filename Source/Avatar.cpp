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
	auto newX = position.getX() - vel * sin(degreesToRadians(thetaPos));
	auto newY = position.getY() + vel * cos(degreesToRadians(thetaPos));
	position.setX(newX > 0.96f ? 0.96f : newX < 0.0f ? 0.0f : newX);
	position.setY(newY > 0.96f ? 0.96f : newY < 0.0f ? 0.0f : newY);
}

void SpatialAudio::Avatar::moveU()
{
	// find dx and dy components based on current angle
	auto newX = position.getX() + vel * sin(degreesToRadians(thetaPos));
	auto newY = position.getY() - vel * cos(degreesToRadians(thetaPos));
	// why .96? well it seems like the right and bottom bounds of the window have some bezel to them
	position.setX(newX > 0.96f ? 0.96f : newX < 0.0f ? 0.0f : newX);
	position.setY(newY > 0.96f ? 0.96f : newY < 0.0f ? 0.0f : newY);
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
	if (thetaPos < 0)
	{
		thetaPos = 360 + thetaPos;
	}
}


Image& Avatar::img()
{
	return m_img;
}