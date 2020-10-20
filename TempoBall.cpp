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
	string direction;
	int leftmostPixel;
	int rightmostPixel;
	int currentPixel;

	int pixelsToMoveOffset;
	int changePixelOffsetThreshold;
	int pixelsMovedSinceOffset;

	sf::CircleShape tempoBallShape;

	// for debugging
	bool firstMove;
	sf::Clock tempoBallClock;
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

		// if moving the ball would move it past pixel 590, then
		// decrease pixelsToMove by however many pixels it takes to move to 590
		// "move" the ball to 590
		// "move" the ball from 590 in the negative x direction by the remaining pixelsToMove
		// move the actual tempo ball to the final ball position
		// set the direction to left


		// if moving the ball would move it past the rightmostPixel, then
		// move it to the rightmostPixel and set the direction to "left"
		if (currentPixel + pixelsToMove >= rightmostPixel)
		{
			/*
			int ballPosition = currentPixel;
			int tempPixelsToMove = pixelsToMove;
			tempPixelsToMove = rightmostPixel - ballPosition;
			ballPosition = rightmostPixel;
			ballPosition = rightmostPixel - tempPixelsToMove;

			// move the ball by an offset of [new position] - [old position]
			tempoBallShape.move(ballPosition - currentPixel, 0);
			*/

			tempoBallShape.move(rightmostPixel - currentPixel, 0);
			currentPixel = rightmostPixel;
			direction = "left";
		}

		// if moving the ball would move it past pixel 150, then
		// do the same but with 150 instead of 590 and in the opposite direction
		// set the direction to right

		// if moving the ball would move it past the rightmostPixel, then
		// move it to the rightmostPixel and set the direction to "left"
		else
			if (currentPixel + pixelsToMove < leftmostPixel)
			{
				/*
				int ballPosition = currentPixel;
				int tempPixelsToMove = pixelsToMove;
				tempPixelsToMove = ballPosition - leftmostPixel;
				ballPosition = leftmostPixel;
				ballPosition = leftmostPixel + tempPixelsToMove;

				// move the ball by an offset of [new position] - [old position]
				tempoBallShape.move(ballPosition - currentPixel, 0);
				*/
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

		/*
		Window::window.setActive(true);
		Window::window.clear();
		Window::window.draw(tempoBallShape);
		Window::window.display();
		Window::window.setActive(false);
		*/
	}
};
