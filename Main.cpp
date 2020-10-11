#include <SFML/Graphics.hpp>

#include <iostream>

#include "WindowMaker.cpp"

using namespace std;

// default values for settings
int Settings::bpm = 0;
sf::Keyboard::Key Settings::leftKey = sf::Keyboard::F;
int Settings::leftInterval = 0;
sf::Keyboard::Key Settings::rightKey = sf::Keyboard::J;
int Settings::rightInterval = 0;
int Settings::numBeats = 0;

int main()
{
	// replace these with user inputs later
	Settings::bpm = 120;
	Settings::leftKey = sf::Keyboard::F;
	Settings::leftInterval = 2;
	Settings::rightKey = sf::Keyboard::J;
	Settings::rightInterval = 0;
	Settings::numBeats = 4;

	WindowMaker w;
	w.loadWindow();

	string asdf = "akshl78887777777777777777777777777mfh78df";

	return 0;
}