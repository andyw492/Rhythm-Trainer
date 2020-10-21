#include <iostream>
#include <vector>
#include <cmath>
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Network.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include "Settings.h"
#include "Window.h"

using namespace std;


class TempoBall
{
	// TODO: explain variables

	string direction;
	int leftmostPixel;
	int rightmostPixel;
	int currentPixel;

	int pixelsToMoveOffset;
	int changePixelOffsetThreshold;
	int pixelsMovedSinceOffset;
	sf::Clock tempoBallClock;

	bool firstMove;
	sf::CircleShape tempoBallShape;
	
	int moveCounter;

public:

	TempoBall()
	{

		direction = "right";
		leftmostPixel = 100;
		rightmostPixel = 640;
		currentPixel = 150;

		pixelsToMoveOffset = 0;
		changePixelOffsetThreshold = 0;
		pixelsMovedSinceOffset = 0;

		// create the tempo ball that will be passed to the TempoBall class
		// radius = 30, pointCount = 120
		
		tempoBallShape.setRadius(30);
		tempoBallShape.setPointCount(120);
		tempoBallShape.setPosition(currentPixel, 100);

		firstMove = true;
		moveCounter = 0;
	
	}

	sf::CircleShape move()
	{
		if (firstMove)
		{
			firstMove = false;
			tempoBallClock.restart();
		}

		// debugging
		moveCounter++;
		if (moveCounter == 60)
		{
			cout << "tempoBallClock: " << tempoBallClock.getElapsedTime().asSeconds() << endl << endl;
			moveCounter = 0;
		}



		// each time move() is called, move the ball 1/60 of the way to the other side
		int pixelsToMove = (rightmostPixel - leftmostPixel) / 60;

		// pixelsToMoveOffset:
		// every 10 seconds, the tempo ball bpm slows down by 1 (tested at bpm = 120)
		// due to the time it takes to draw to the window
		// therefore, we will move the tempo ball by an extra pixel for every nth pixel moved
		// where n = round(120 / (elapsedSeconds / 10))

		pixelsMovedSinceOffset += pixelsToMove;
		changePixelOffsetThreshold = round(120.0 / (tempoBallClock.getElapsedTime().asSeconds() / 10.0));
		if (pixelsMovedSinceOffset >= changePixelOffsetThreshold)
		{
			pixelsToMove++;
			pixelsMovedSinceOffset = 0;
		}
		
		if (direction == "left")
		{
			// move in the negative x direction if direction == "left"
			pixelsToMove *= -1;
		}

		// if moving the ball would move it past the rightmostPixel, then
		// move it to the rightmostPixel and set the direction to "left"
		if (currentPixel + pixelsToMove >= rightmostPixel)
		{
			tempoBallShape.move(rightmostPixel - currentPixel, 0);
			currentPixel = rightmostPixel;
			direction = "left";
		}

		// if moving the ball would move it past the rightmostPixel, then
		// move it to the rightmostPixel and set the direction to "left"
		else
		if (currentPixel + pixelsToMove <= leftmostPixel)
		{
			tempoBallShape.move(leftmostPixel - currentPixel, 0);
			currentPixel = leftmostPixel;

			direction = "right";
		}

		// if moving the ball doesn't move it past the left or right pixel boundaries
		else
		{
			tempoBallShape.move(pixelsToMove, 0);
			currentPixel += pixelsToMove;
		}

		return tempoBallShape;
	}
};
