#include <iostream>
#include <vector>
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Network.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include "Settings.h"
#include "Window.h"

using namespace std;

class NoteBlocks
{
	int startingPixel; // the pixel where the top border is
	int targetPixel; // the pixel where the buttons are
	int endingPixel; // the pixel where the bottom border is

	vector<sf::RectangleShape> notes;

	NoteBlocks()
	{
		startingPixel = 200;
		targetPixel = 620;
		endingPixel = 700;

	}

	vector<sf::RectangleShape> moveLeftNotes()
	{
		// move the note 0.0083 seconds' worth of pixels
		// the amount of seconds it takes to move the note block from the start to the button (i.e. seconds per 4 beats)
		// is equal to 4(60 / bpm)
		// therefore, it takes 4(60 / bpm) seconds to move 620 - 200 = 420 pixels
		// it takes 0.0083 seconds to move x pixels; x = 420 / (4(60 / bpm) / 0.0083) pixels

		// use the same offset (120/104) as the tempo ball
		int pixelsToMove = (120.0 / 104.0) * int(double(targetPixel - startingPixel) / (4 * (60.0 / Settings::bpm) / 0.0083));
		

		return notes;
	
	}

	vector<sf::RectangleShape> moveRightNotes()
	{
		return notes;
	}
};