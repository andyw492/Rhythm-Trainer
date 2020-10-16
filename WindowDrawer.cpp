#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Network.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include <iostream>
#include <vector>
#include <cmath>

#include "Settings.h"
#include "Window.h"

class WindowDrawer
{

public:

	WindowDrawer(){}

	void drawToWindow(vector<sf::Drawable*> &drawables, bool textInput)
	{
		bool doCin = false;

		if (textInput)
		{
			doCin = true;
			cout << "in drawToWindow, drawables.size is " << drawables.size() << endl;
			string s; cin >> s;
		}


		while (drawables.size() > 0)
		{
			cout << "in drawToWindow's while loop, drawables.size is " << drawables.size() << endl;
			//string s; cin >> s;

			Window::window.clear();
			Window::window.draw(*drawables[0]);
			Window::window.display();



			delete drawables[0];
			drawables.erase(drawables.begin());
		}

		if (textInput)
		{
			cout << "drew to window " << endl;
			string s; cin >> s;
		}
	}

};