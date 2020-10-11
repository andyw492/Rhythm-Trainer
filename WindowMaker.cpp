#include <iostream>
#include <vector>
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Network.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include "InputAnalyzer.cpp"

using namespace std;

class WindowMaker
{
public:

	WindowMaker() {}

	void drawWindow()
	{
		//-----------------------TEXT SETUP----------------------------
		sf::Font font;
		if (!font.loadFromFile("fontfile.ttf"))
		{
			cout << "DRAWWINDOW ERROR: could not load font from file" << endl;
			return;
		}

		sf::Text text("F", font, 24);

		//-----------------------WINDOW SETUP---------------------------
		sf::Clock actionClock;

		sf::RenderWindow window(sf::VideoMode(400, 400), "Key Press Reader");
		window.display();

		//-----------------------WINDOW EVENTS------------------------------
		while (window.isOpen())
		{
			sf::Event event;
			// while there are pending events...
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
						text.setString("F " +
							to_string(actionClock.getElapsedTime().asSeconds()));
						while (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
						{
							window.clear();
							text.setPosition(100.f, 100.f);
							window.draw(text);
							window.display();	
						}
						text.setString("F");
						if (!sf::Keyboard::isKeyPressed(sf::Keyboard::F))
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