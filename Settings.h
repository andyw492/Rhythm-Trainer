#ifndef SETTINGS_H
#define SETTINGS_H

#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Network.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

using namespace std;

// static class that contains info like bpm, noteInverval, numMeasures, etc.
static class Settings
{
public:

	static int bpm;
	static sf::Keyboard::Key leftKey;
	static int leftInterval;
	static sf::Keyboard::Key rightKey;
	static int rightInterval;
	static int numBeats;

};

#endif // SETTINGS_H