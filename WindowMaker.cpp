#include <iostream>
#include <vector>
#include <thread>
#include <string>
#include <cmath>
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Network.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include "InputAnalyzer.cpp"
#include "TempoBall.cpp"
#include "Settings.h"
#include "Window.h"
#include "WindowDrawer.cpp"

using namespace std;

class WindowMaker
{
	TempoBall ball;
	// note block

public:

	WindowMaker()
	{
	}


	void loadWindow()
	{


		//-----------------------WINDOW SETUP---------------------------
		
		InputAnalyzer inputAnlz;

		while (Window::window.isOpen())
		{
			runWindowProcesses(inputAnlz);
		}


		
		


	}
	

	/*
		Function that runs the default state, startup state, and action state.
		- calls the first two functions to get the tempo ball and note block positions into the vector
		- stops the action when the actionClock reaches a certain point
		- polls events for closing the window or inputting key presses (puts the key press info returned by InputAnalyzer into the vector)
		- calls WindowDrawer and gives it the vector so it can draw everything
		- repeats above steps while !endAction
	*/
	void runWindowProcesses(InputAnalyzer &inputAnlz)
	{
		//-----------------------TEXT SETUP----------------------------
		sf::Font font;
		if (!font.loadFromFile("fontfile.ttf"))
		{
			cout << "DRAWWINDOW ERROR: could not load font from file" << endl;
			return;
		}

		sf::Text leftText("F", font, 24);
		sf::Text rightText("J", font, 24);

		//-----------------------DISPLAY SETUP------------------------------

		sf::Text text("Press any key to begin", font, 24);
		text.setPosition(100.f, 100.f);

		sf::RectangleShape noteBorder;
		noteBorder.setPosition(sf::Vector2f(100.f, 200.f));
		noteBorder.setSize(sf::Vector2f(600.f, 500.f));
		noteBorder.setFillColor(sf::Color::Black);
		noteBorder.setOutlineColor(sf::Color::White);
		noteBorder.setOutlineThickness(10);

		sf::RectangleShape leftNoteButton;
		leftNoteButton.setPosition(sf::Vector2f(200.f, 620.f));
		leftNoteButton.setSize(sf::Vector2f(100.f, 30.f));
		leftNoteButton.setFillColor(sf::Color::White);

		sf::RectangleShape rightNoteButton;
		rightNoteButton.setPosition(sf::Vector2f(500.f, 620.f));
		rightNoteButton.setSize(sf::Vector2f(100.f, 30.f));
		rightNoteButton.setFillColor(sf::Color::White);

		// the tempo ball
		// the properties of tempoBallShape are given by a copy constructor later on using the TempoBall class
		sf::CircleShape tempoBallShape;

		//-----------------------WINDOW EVENTS------------------------------
		sf::Clock actionClock;
		sf::Clock timeSinceMoved; // for tempo ball
		bool textInput = false;

		bool firstPress = true; // temporary for phase 1

		while (Window::window.isOpen())
		{
			//--------------------------DEFAULT STATE-----------------------------

			Window::window.clear();
			Window::window.draw(text);
			Window::window.display();

			// wait for the user to press any key (that isn't ctrl-W)
			bool anyKeyPressed = false;
			while (!anyKeyPressed)
			{
				sf::Event event;
				while (Window::window.pollEvent(event))
				{
					switch (event.type)
					{
						// window closed
						case sf::Event::Closed:
							Window::window.close();
							break;

						// key pressed
						case sf::Event::KeyPressed:
						{
							// allow the user to close the window through Ctrl-W
							if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && 
								sf::Keyboard::isKeyPressed(sf::Keyboard::W)
								||
								sf::Keyboard::isKeyPressed(sf::Keyboard::RControl) && 
								sf::Keyboard::isKeyPressed(sf::Keyboard::W))
							{
								Window::window.close();
								break;
							}

							anyKeyPressed = true;
						}

						default:
							break;
						}
				} // end while (window.pollEvent(event))
			} // end while (!anyKeyPressed)


			//--------------------------STARTUP STATE-------------------------------

			sf::Clock startupClock;

			sf::Text startupText; // temporary
			startupText.setFont(font);
			startupText.setCharacterSize(24);

			// run this loop for 4 beats of time
			// 4 beats = 4 * (60 / bpm) seconds
			while (startupClock.getElapsedTime().asSeconds() < 4 * (60.0 / Settings::bpm))
			{
				// temporary
				startupText.setString(to_string(actionClock.getElapsedTime().asSeconds()));

				// draw all of the objects to the window
				// for copy-paste: Window::window.draw

				Window::window.clear();
				Window::window.draw(noteBorder);
				Window::window.draw(leftNoteButton);
				Window::window.draw(rightNoteButton);
				Window::window.draw(tempoBallShape);
				Window::window.display();


				// moving tempo ball and note blocks
				// if the time since the ball was last moved > 0.0083 seconds,
				// then tell the ball and notes to move the distance equivalent to 0.0083 seconds
				// using 0.0083 seconds as a time interval makes the ball and notes move at 120 fps
				if (timeSinceMoved.getElapsedTime().asSeconds() > 0.0083)
				{
					tempoBallShape = ball.move();
					timeSinceMoved.restart();
				}
			}

			//--------------------------ACTION STATE---------------------------------
			actionClock.restart();

			// endAction can be set to true to make the window stop polling for events
			bool endAction = false;

			int moveCounter = 0;
			int moveThreshold = 1;
			while (!endAction)
			{
				// draw all of the objects to the window
				// for copy-paste: Window::window.draw

				Window::window.clear();
				Window::window.draw(noteBorder);
				Window::window.draw(leftNoteButton);
				Window::window.draw(rightNoteButton);
				Window::window.draw(tempoBallShape);
				Window::window.draw(leftText);
				Window::window.display();

				//while(true){}

				// moving tempo ball and note blocks
				// if the time since the ball was last moved > 1/60 of a beat,
				// then tell the ball and notes to move the distance equivalent to 1/60 of a beat
				// using 1/60 of a beat seconds as a time interval makes the ball and notes move at 120 fps

				// if the time since the ball was last moved > 1/60 of a beat,
				double tempoBallMoveInterval = (60.0 / Settings::bpm) / 60.0;

				
				/*
				while (actionClock.getElapsedTime().asSeconds() < 2)
				{
					if (trunc(actionClock.getElapsedTime().asSeconds() / tempoBallMoveInterval) == moveThreshold)
					{
						cout << trunc(actionClock.getElapsedTime().asSeconds() / tempoBallMoveInterval) << endl;
						cout << "elapsed time: " << actionClock.getElapsedTime().asSeconds() << endl << endl;
						moveThreshold++;
					}
					
				}

				while(true){}
				*/
				if (trunc(actionClock.getElapsedTime().asSeconds() / tempoBallMoveInterval) == moveThreshold)
				{
					//cout << "moving ball, elapsed time is " << actionClock.getElapsedTime().asSeconds() << endl;
					
					//timeSinceMoved.restart();
					
					//cout << "trunc: " << trunc(actionClock.getElapsedTime().asSeconds() / tempoBallMoveInterval) << endl;
					//cout << "moveThreshold: " << moveThreshold << endl;
					//cout << "equal? " << (trunc(actionClock.getElapsedTime().asSeconds() / tempoBallMoveInterval) == moveThreshold) << endl;
					moveThreshold++;

					tempoBallShape = ball.move();

					moveCounter++;
					if (moveCounter == 60)
					{
						cout << "elapsed time: " << actionClock.getElapsedTime().asSeconds() << endl;
						moveCounter = 0;
					}
				}

				// stop the Action on the nth beat, where n = numBeats
				// the nth beat in seconds = (60 / bpm) * (numBeats)

				// it doesn't stop on n = numBeats + 1 because the first beat is at 0 seconds
				// example: if numBeats = 4 and bpm = 120, then the notes are expected at:
				// 0.0 seconds, 0.5 seconds, 1.0 seconds, 1.5 seconds
				// and it stops looking for notes at (60 / 120) * 4 = 2.0 seconds

				if (actionClock.getElapsedTime().asSeconds() > float((60.0 / Settings::bpm) * Settings::numBeats))
				{
					endAction = true;
				}



				sf::Event event;
				// while there are pending events
				while (Window::window.pollEvent(event))
				{

					// check the type of the event...
					switch (event.type)
					{
						// window closed
						case sf::Event::Closed:
							Window::window.close();
							break;

						// key pressed
						case sf::Event::KeyPressed:
						{
							// allow the user to close the window through Ctrl-W
							if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::W)
								|| sf::Keyboard::isKeyPressed(sf::Keyboard::RControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::W))
							{
								endAction = true;
								Window::window.close();
								break;
							}

							// stop the Action when escape is pressed
							if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
							{
								endAction = true;
							}


							if (firstPress) // temporary for phase 1
							{
								actionClock.restart();
								firstPress = false;
							}

							// temporary
							leftText.setString("F " +
								to_string(actionClock.getElapsedTime().asSeconds()));
							
							bool sentKeyPress = false;
							if (sf::Keyboard::isKeyPressed(Settings::leftKey))
							{
								
								// only send the key press to InputAnalyzer once
								if (!sentKeyPress)
								{
									leftText.setString(inputAnlz.getPressAccuracy
									(Settings::leftKey, actionClock.getElapsedTime().asSeconds()));

									cout << "Elapsed time: " << actionClock.getElapsedTime().asSeconds() << endl;

									sentKeyPress = true;
								}
								

								leftText.setPosition(100.f, 100.f);
								string textString = leftText.getString();
								//drawables.push_back(new sf::Text(leftText));
								//Window::window.draw(leftText);
								textInput = true;
								//string s; cin >> s;

								/*
								Window::window.setActive(true);
								Window::window.clear();
								text.setPosition(100.f, 100.f);
								Window::window.draw(text);
								Window::window.display();
								Window::window.setActive(false);
								*/
							}

							// if the right key is pressed
							if (!sf::Keyboard::isKeyPressed(Settings::leftKey))
							{
								//Window::window.clear();
								//Window::window.display();
							}
							break;
						}

					// we don't process other types of events
					default:
						break;
					}
				} // end while (window.pollEvent(event))


				//drawer.drawToWindow(drawables, textInput);
				textInput = false;

					
			}
			


			// go back to default state

		} // end while (window.isOpen())

		// join the threads
		//tempoBallMover.join();

	}
};