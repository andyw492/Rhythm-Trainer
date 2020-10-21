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

	// the amount of pixels that a note should move for every beat,
	// equal to 1/4 the distance between the starting pixel and the target pixel
	int pixelsPerBeat;

	bool firstMove;
	sf::Clock noteBlocksClock;

	sf::RectangleShape noteTemplate;
	vector<sf::RectangleShape> leftNotes;
	vector<sf::RectangleShape> rightNotes;

public:

	NoteBlocks()
	{
		startingPixel = 200;
		targetPixel = 620;
		endingPixel = 700;
		pixelsPerBeat = (targetPixel - startingPixel) / 4;

		firstMove = false;

		noteTemplate.setSize(sf::Vector2f(100.f, 30.f));
		noteTemplate.setFillColor(sf::Color::Cyan);

	}

	void spawnLeftNote(vector<sf::RectangleShape> &leftNotes)
	{
		sf::RectangleShape leftNote(noteTemplate);
		leftNote.setPosition(sf::Vector2f(200.f, 200.f));
		leftNotes.push_back(leftNote);
	}

	void spawnRightNote(vector<sf::RectangleShape> &rightNotes)
	{
		sf::RectangleShape rightNote(noteTemplate);
		rightNote.setPosition(sf::Vector2f(500.f, 200.f));
		rightNotes.push_back(rightNote);
	}

	void moveLeftNotes(vector<sf::RectangleShape> &leftNotes)
	{
		if (firstMove)
		{
			firstMove = false;
			noteBlocksClock.restart();
		}

		// move the notes 1/60 of the distance of one beat of time
		// TODO: figure out why the 2 * is needed to make it the right speed
		int pixelsToMove = 2 * (pixelsPerBeat) / 60;

		// move each note individually
		for (int i = 0; i < leftNotes.size(); i++)
		{
			float yPosition = leftNotes[i].getPosition().y;

			// if moving the note would move it past the ending pixel, then
			// remove it from the vector
			if (yPosition + pixelsToMove >= endingPixel)
			{
				leftNotes.erase(leftNotes.begin() + i);
			}
			else
			{
				leftNotes[i].move(0, pixelsToMove);
			}
		}

	}

	void moveRightNotes(vector<sf::RectangleShape> &rightNotes)
	{
		if (firstMove)
		{
			firstMove = false;
			noteBlocksClock.restart();
		}

		// move the notes 1/60 of the distance of one beat of time
		int pixelsToMove = 2 * (pixelsPerBeat) / 60;

		// move each note individually
		for (int i = 0; i < rightNotes.size(); i++)
		{
			float yPosition = rightNotes[i].getPosition().y;

			// if moving the note would move it past the ending pixel, then
			// remove it from the vector
			if (yPosition + pixelsToMove >= endingPixel)
			{
				rightNotes.erase(rightNotes.begin() + i);
			}
			else
			{
				rightNotes[i].move(0, pixelsToMove);
			}
		}

	}
};