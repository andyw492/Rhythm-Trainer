#include <iostream>
#include <vector>
#include <thread>
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Network.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include "InputAnalyzer.cpp"
#include "TempoBall.cpp"
#include "Settings.h"
#include "Window.h"

using namespace std;

class WindowMaker
{
public:

	WindowMaker()
	{
	}


	void loadWindow()
	{


		//-----------------------WINDOW SETUP---------------------------
		

		



		InputAnalyzer inputAnlz;
		TempoBall ball;

		while (Window::window.isOpen())
		{
			readKeyPresses(inputAnlz, ball);
		}


		
		


	}
	/*
	void tempoBallRunner(TempoBall &ball)
	{
		sf::Clock timeSinceMoved;
	}
	*/

	void readKeyPresses(InputAnalyzer &inputAnlz, TempoBall &ball)
	{
		//-----------------------TEXT SETUP----------------------------
		sf::Font font;
		if (!font.loadFromFile("fontfile.ttf"))
		{
			cout << "DRAWWINDOW ERROR: could not load font from file" << endl;
			return;
		}

		sf::Text text("F", font, 24);

		//-----------------------THREAD SETUP----------------------------

		thread tempoBallMover(&TempoBall::move, TempoBall());
		

		//-----------------------WINDOW EVENTS------------------------------
		sf::Clock actionClock;

		bool firstPress = true; // temporary for phase 1

		while (Window::window.isOpen())
		{
			//--------------------------DEFAULT STATE-----------------------------

			sf::Text text("Default State", font, 24);
			Window::window.clear();
			text.setPosition(100.f, 100.f);
			Window::window.draw(text);
			Window::window.display();

			//--------------------------STARTUP STATE-------------------------------

			// start the tempo ball
			ball.setBallMove(true);

			//--------------------------ACTION STATE---------------------------------


			// endAction can be set to true to make the window stop polling for events
			bool endAction = false;

			sf::Event event;

			// while there are pending events
			while (Window::window.pollEvent(event) && !endAction)
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
						Window::window.close();
						break;

					// key pressed
					case sf::Event::KeyPressed:
					{
						// allow the user to close the window through Ctrl-W
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::W)
							|| sf::Keyboard::isKeyPressed(sf::Keyboard::RControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::W))
						{
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

							Window::window.clear();
							text.setPosition(100.f, 100.f);
							Window::window.draw(text);
							Window::window.display();
						}
						text.setString("F");
						if (!sf::Keyboard::isKeyPressed(Settings::leftKey))
						{
							Window::window.clear();
							Window::window.display();
						}
						break;
					}

				// we don't process other types of events
				default:
					break;
				}
			} // end while (window.pollEvent(event) && !endAction)

			// stop the tempo ball
			ball.setBallMove(false);

			// go back to default state

		} // end while (window.isOpen())

		// join the threads
		ball.setBallFinished(true);
		//tempoBallMover.join();

	}
};