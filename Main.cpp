#include <SFML/Graphics.hpp>

#include <iostream>

#include "WindowMaker.cpp"

using namespace std;

#include "Settings.h"
// default values for settings
int Settings::bpm = 0;
sf::Keyboard::Key Settings::leftKey = sf::Keyboard::F;
int Settings::leftInterval = 0;
sf::Keyboard::Key Settings::rightKey = sf::Keyboard::J;
int Settings::rightInterval = 0;
int Settings::numBeats = 0;



#include "Window.h"
// default values for WindowMaker
sf::RenderWindow Window::window(sf::VideoMode(800, 800), "Rhythm Trainer");


int main()
{
	string asdf = "a7777777777777777777777777777777777777777777777777777777777777777777";

	//thread tempoBallMover(&TempoBall::move, TempoBall());

	//thread worker(&tempTempoBall::move, tempTempoBall(), window);
	//thread baller(&ballClass::DoBall, ballClass(), "ball", 10);

	// replace these with user inputs later
	Settings::bpm = 120;
	Settings::leftKey = sf::Keyboard::F;
	Settings::leftInterval = 1;
	Settings::rightKey = sf::Keyboard::J;
	Settings::rightInterval = 1;
	Settings::numBeats = 99;
	WindowMaker w;
	w.loadWindow();

	

	return 0;
}