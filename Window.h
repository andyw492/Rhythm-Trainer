#ifndef WINDOW_H
#define WINDOW_H

#include "SFML/Graphics.hpp"

// static class that contains the main window so that every class can draw on it
static class Window
{
public:

	static sf::RenderWindow window;

};


#endif // WINDOW_H