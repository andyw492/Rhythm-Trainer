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
#include "NoteBlocks.cpp"
#include "Settings.h"
#include "Window.h"
#include "WindowDrawer.cpp"

using namespace std;

class WindowMaker
{
	TempoBall ball;
	NoteBlocks blocks;

public:

	WindowMaker(){}

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
		leftNoteButton.setFillColor(sf::Color::Blue);

		sf::RectangleShape rightNoteButton;
		rightNoteButton.setPosition(sf::Vector2f(500.f, 620.f));
		rightNoteButton.setSize(sf::Vector2f(100.f, 30.f));
		rightNoteButton.setFillColor(sf::Color::Blue);

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

			int moveCounter = 0; // debugging

			// for moving tempo ball and note blocks
			int tempoBallNextThreshold = 0;

			// for spawning left and right note blocks
			int leftSpawnNextThreshold = 0;
			int rightSpawnNextThreshold = 0;

			// store the left and right note blocks
			// (these vectors are passed by reference to the NoteBlocks class's functions so that
			// they don't have to be remade every time we draw to the window)
			vector<sf::RectangleShape> leftNoteBlocks;
			vector<sf::RectangleShape> rightNoteBlocks;

			while (!endAction)
			{
				// draw all of the objects to the window
				Window::window.clear();
				Window::window.draw(noteBorder);
				Window::window.draw(leftNoteButton);
				Window::window.draw(rightNoteButton);
				Window::window.draw(tempoBallShape);

				// draw the noteBlocks
				for (int i = 0; i < leftNoteBlocks.size(); i++)
				{
					Window::window.draw(leftNoteBlocks[i]);
				}
				for (int i = 0; i < rightNoteBlocks.size(); i++)
				{
					Window::window.draw(rightNoteBlocks[i]);
				}

				Window::window.draw(leftText);
				Window::window.display();

				// moving tempo ball and note blocks
				// if the time since the ball was last moved > 1/60 of a beat,
				// then tell the ball and notes to move the distance equivalent to 1/60 of a beat
				// using 1/60 of a beat seconds as a time interval makes the ball and notes move at 120 fps

				// we have to use the actionClock as a time reference because making a clock for this 
				// and restarting it whenever the functions are called will slow down the ball and notes too much

				// at bpm = 120, 1/60 of a beat is 0.833 seconds
				// thus, for bpm = 120 the functions should be called whenever the clock's elapsed time is:
				// 0.00833, 0.01666, 0.02500, etc.

				// 1/60 of a beat translated to seconds
				double tempoBallMoveInterval = (60.0 / Settings::bpm) / 60.0;

				// if the clock's elapsed time in seconds is 0.00833, 0.01666, 0.02500, etc.
				if (trunc(actionClock.getElapsedTime().asSeconds() / tempoBallMoveInterval) == tempoBallNextThreshold)
				{
					tempoBallShape = ball.move();
					blocks.moveLeftNotes(leftNoteBlocks);
					blocks.moveRightNotes(rightNoteBlocks);

					tempoBallNextThreshold++;

					// debugging
					moveCounter++;
					if (moveCounter == 60)
					{
						cout << "elapsed time: " << actionClock.getElapsedTime().asSeconds() << endl;
						moveCounter = 0;
					}
				}

				// spawn new notes whenever the action clock reaches a certain time
				// that time depends on the bpm and the [left/right]Interval

				// at bpm = 120 and leftInterval = 2, the time between notes is:
				// (60 / bpm) / leftInterval = (60 / 120) / 2 = 0.25 seconds
				// thus, a note should be spawned whenever the clock's elapsed time is: 0.25, 0.5, 0.75, etc.

				double leftNoteSpawnInterval = (60.0 / Settings::bpm) / Settings::leftInterval;
				double rightNoteSpawnInterval = (60.0 / Settings::bpm) / Settings::rightInterval;

				// check if the actionClock's elapsed time is on a leftInterval
				if (trunc(actionClock.getElapsedTime().asSeconds() / leftNoteSpawnInterval) == leftSpawnNextThreshold)
				{
					blocks.spawnLeftNote(leftNoteBlocks);
					cout << "called spawn left note, elapsed time: " << actionClock.getElapsedTime().asSeconds() << endl;
					leftSpawnNextThreshold++;
				}

				// check if the actionClock's elapsed time is on a rightInterval
				if (trunc(actionClock.getElapsedTime().asSeconds() / rightNoteSpawnInterval) == rightSpawnNextThreshold)
				{
					blocks.spawnRightNote(rightNoteBlocks);
					cout << "called spawn right note, elapsed time: " << actionClock.getElapsedTime().asSeconds() << endl;
					rightSpawnNextThreshold++;
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