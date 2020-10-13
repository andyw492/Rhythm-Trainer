#include <iostream>
#include <vector>
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Network.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include "InputAnalyzer.cpp"
#include "TempoBall.cpp"
#include "Settings.h"

using namespace std;

class WindowMaker
{
public:

	WindowMaker() {}

	void loadWindow()
	{


		//-----------------------WINDOW SETUP---------------------------
		

		sf::RenderWindow window(sf::VideoMode(800, 800), "Key Press Reader");



		InputAnalyzer inputAnlz;
		TempoBall ball(window);

		while (window.isOpen())
		{
			readKeyPresses(window, inputAnlz, ball);
		}


		
		


	}

	void tempoBallRunner(TempoBall &ball)
	{
		sf::Clock timeSinceMoved;
	}

	void readKeyPresses(sf::RenderWindow &window, InputAnalyzer &inputAnlz, TempoBall &ball)
	{
		//-----------------------TEXT SETUP----------------------------
		sf::Font font;
		if (!font.loadFromFile("fontfile.ttf"))
		{
			cout << "DRAWWINDOW ERROR: could not load font from file" << endl;
			return;
		}

		sf::Text text("F", font, 24);

		//-----------------------WINDOW EVENTS------------------------------
		sf::Clock actionClock;
		sf::Clock timeSinceMoved; // for the tempo ball

		bool firstPress = true; // temporary for phase 1

		while (window.isOpen())
		{
			//--------------------------DEFAULT STATE-----------------------------

			// moving tempo ball
			// if the time since the ball was last moved > 0.0083 seconds,
			// then tell the ball to move the distance equivalent to 0.0083 seconds
			// using 0.0083 seconds as a time interval makes the ball move at 120 fps
			if (timeSinceMoved.getElapsedTime().asSeconds() > 0.0083)
			{
				timeSinceMoved.restart();
				ball.move(window);
				cout << "timeSinceMoved elapsed time: " << timeSinceMoved.getElapsedTime().asSeconds() << endl;
			}

			//--------------------------STARTUP STATE-------------------------------


			//--------------------------ACTION STATE---------------------------------

			// endAction can be set to true to make the window stop polling for events
			bool endAction = false;

			sf::Event event;

			// while there are pending events
			while (window.pollEvent(event) && !endAction)
			{
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

				// check the type of the event...
				switch (event.type)
				{
					// window closed
					case sf::Event::Closed:
						window.close();
						break;

					// key pressed
					case sf::Event::KeyPressed:
					{
						// allow the user to close the window through Ctrl-W
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::W)
							|| sf::Keyboard::isKeyPressed(sf::Keyboard::RControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::W))
						{
							window.close();
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

						text.setString("F " +
							to_string(actionClock.getElapsedTime().asSeconds()));
					
						bool sentKeyPress = false;
						if (sf::Keyboard::isKeyPressed(Settings::leftKey))
						{
							// only send the key press to InputAnalyzer once
							if (!sentKeyPress)
							{
								cout << "Press accuracy: " << inputAnlz.getPressAccuracy
								(Settings::leftKey, actionClock.getElapsedTime().asSeconds());

								cout << "Elapsed time: " << actionClock.getElapsedTime().asSeconds() << endl;

								sentKeyPress = true;
							}

							window.clear();
							text.setPosition(100.f, 100.f);
							window.draw(text);
							window.display();
						}
						text.setString("F");
						if (!sf::Keyboard::isKeyPressed(Settings::leftKey))
						{
							window.clear();
							window.display();
						}
						break;
					}

				// we don't process other types of events
				default:
					break;
				}
			}
		}

	}
};