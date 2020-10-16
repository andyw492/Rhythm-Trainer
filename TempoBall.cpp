#include <iostream>
#include <vector>
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

	sf::CircleShape tempoBallShape;

public:

	TempoBall()
	{

		direction = "right";
		leftmostPixel = 150;
		rightmostPixel = 590;
		currentPixel = 150;

		// create the tempo ball that will be passed to the TempoBall class
		// radius = 30, pointCount = 120
		
		tempoBallShape.setRadius(30);
		tempoBallShape.setPointCount(120);
		tempoBallShape.setPosition(currentPixel, 150);
		Window::window.clear();
		Window::window.draw(tempoBallShape);
		Window::window.display();
	
	}

	sf::CircleShape move()
	{
		bool reset = false;

		if (reset)
		{
			// move ball back to original position
		}

		// move the ball 0.0083 seconds' worth of pixels
		// the amount of seconds it takes to move the ball from one end to another (i.e. seconds per beat)
		// is equal to 60 / bpm
		// therefore, it takes (60 / bpm) seconds to move 590 - 150 = 440 pixels
		// it takes 0.0083 seconds to move x pixels; x = 440 / ((60 / bpm) / 0.0083) pixels

		int pixelsToMove = int(440.0 / ((60.0 / Settings::bpm) / 0.0083));
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
		if (currentPixel + pixelsToMove > rightmostPixel)
		{
			int ballPosition = currentPixel;
			int tempPixelsToMove = pixelsToMove;
			tempPixelsToMove = 590 - ballPosition;
			ballPosition = 590;
			ballPosition = 590 - tempPixelsToMove;

			// move the ball by an offset of [new position] - [old position]
			tempoBallShape.move(ballPosition - currentPixel, 0);

			direction = "left";
		}

		// if moving the ball would move it past pixel 150, then
		// do the same but with 150 instead of 590 and in the opposite direction
		// set the direction to right
		else
			if (currentPixel + pixelsToMove < leftmostPixel)
			{
				int ballPosition = currentPixel;
				int tempPixelsToMove = pixelsToMove;
				tempPixelsToMove = ballPosition - 150;
				ballPosition = 150;
				ballPosition = 150 + tempPixelsToMove;

				// move the ball by an offset of [new position] - [old position]
				tempoBallShape.move(ballPosition - currentPixel, 0);

				direction = "right";
			}

		// if moving the ball doesn't move it past the left or right pixel boundaries
			else
			{
				tempoBallShape.move(pixelsToMove, 0);
			}

		currentPixel += pixelsToMove;


		if (currentPixel > 550)
		{
			//string s; cin >> s;
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
