#include <iostream>
#include <vector>
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Network.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include "InputAnalyzer.cpp"
#include "Settings.h"

using namespace std;

class WindowMaker
{
public:

	WindowMaker() {}

	void loadWindow()
	{


		//-----------------------WINDOW SETUP---------------------------
		

		sf::RenderWindow window(sf::VideoMode(400, 400), "Key Press Reader");
		window.display();

		InputAnalyzer inputAnlz;


		while (window.isOpen())
		{
			readKeyPresses(window, inputAnlz);
		}
		



	}

	void readKeyPresses(sf::RenderWindow &window, InputAnalyzer &inputAnlz)
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

		bool firstPress = true; // temporary for phase 1

		while (window.isOpen())
		{
			sf::Event event;
			// while there are pending events
			// stop on the nth beat, where n = numBeats
			// the nth beat in seconds = (60 / bpm) * (numBeats)

			// it doesn't stop on n = numBeats + 1 because the first beat is at 0 seconds
			// example: if numBeats = 4 and bpm = 120, then the notes are expected at:
			// 0.0 seconds, 0.5 seconds, 1.0 seconds, 1.5 seconds
			// and it stops looking for notes at (60 / 120) * 4 = 2.0 seconds

			if (actionClock.getElapsedTime().asSeconds() > float((60.0 / Settings::bpm) * Settings::numBeats))
			{
				break;
			}
			while (window.pollEvent(event))
			{
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


					if (firstPress) // temporary for phase 1
					{
						actionClock.restart();
						firstPress = false;
					}

					text.setString("F " +
						to_string(actionClock.getElapsedTime().asSeconds()));
					
					bool sentKeyPress = false;
					while (sf::Keyboard::isKeyPressed(Settings::leftKey))
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