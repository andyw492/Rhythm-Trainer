#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Network.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include <iostream>
#include <vector>
#include <cmath>

#include "Settings.h"

using namespace std;

// InputAnalyzer takes in a key press and elapsed time, and returns how accurate the key press was
// to the expected time that is derived from the bpm and noteInterval
class InputAnalyzer 
{
	vector<float> leftExpectedTimes;
	vector<float> leftRecordedTimes;

	vector<float> rightExpectedTimes;
	vector<float> rightRecordedTimes;

	int noteCount = 0;

public:

	InputAnalyzer()
	{
		// load expectedTimes for each side
		// note n is expected at (60 / (bpm * noteInterval)) * (n - 1) seconds
		// (e.g. the first note is expected at 0 seconds)
		// the total amount of notes for each hand is (numBeats * [left/right]Interval)
		// the * 2 at the end is just to prevent vector subscript out of range
		// (nothing bad happens if leftExpectedTimes is too big)
		for (int i = 0; i < Settings::numBeats * Settings::leftInterval * 2; i++)
		{
			leftExpectedTimes.push_back(float((60.0 / (Settings::bpm * Settings::leftInterval) * i)));
			cout << "leftExpectedTimes at " << i << " is " << leftExpectedTimes[i] << endl;
		}
		for (int i = 0; i < Settings::numBeats * Settings::rightInterval * 2; i++)
		{
			rightExpectedTimes.push_back(float((60.0 / (Settings::bpm * Settings::rightInterval) * i)));
		}
	}

	string getPressAccuracy(sf::Keyboard::Key key, float seconds)
	{
		leftRecordedTimes.push_back(seconds);

		// only for phase 1
		if (seconds < 0.1)
		{
			noteCount = 0;
		}
		cout << "seconds is " << seconds << endl;
		cout << "noteCount is " << noteCount << endl;

		// thresholds for "red", "yellow" and "green" scores:
		// red score if >= 0.2 beats off
		// yellow score if < 0.2 and > 0.1 beats off
		// green score if <= 0.1 beats off

		// 0.2 beats = 0.2 * (60 / bpm) seconds
		// 0.1 beats = 0.1 * (60 / bpm) seconds

		float redScoreDifferenceThreshold = (0.2 * 60.0 / Settings::bpm);
		float greenScoreDifferenceThreshold = (0.1 * 60.0 / Settings::bpm);
		cout << endl;
		cout << "red threshold: " << redScoreDifferenceThreshold << endl;
		cout << "green threshold: " << greenScoreDifferenceThreshold << endl;
		cout << "expected at " << leftExpectedTimes[noteCount] << endl;

		// need to store the return in a string because noteCount needs to be incremented before returning
		string returnString = "";

		if (key == Settings::leftKey)
		{
			cout << "difference is " << abs(seconds - leftExpectedTimes[noteCount]) << endl;
			if (abs(seconds - leftExpectedTimes[noteCount]) >= redScoreDifferenceThreshold)
			{
				returnString = "leftRed";
			}
			if (abs(seconds - leftExpectedTimes[noteCount]) < redScoreDifferenceThreshold &&
				abs(seconds - leftExpectedTimes[noteCount]) > greenScoreDifferenceThreshold)
			{
				returnString = "leftYellow";
			}
			if (abs(seconds - leftExpectedTimes[noteCount]) <= greenScoreDifferenceThreshold)
			{
				returnString = "leftGreen";
			}
		}

		else 
		if (key == Settings::rightKey)
		{
			if (abs(seconds - rightExpectedTimes[noteCount]) >= redScoreDifferenceThreshold)
			{
				returnString = "rightRed";
			}
			if (abs(seconds - rightExpectedTimes[noteCount]) < redScoreDifferenceThreshold &&
				abs(seconds - rightExpectedTimes[noteCount]) > greenScoreDifferenceThreshold)
			{
				returnString = "rightYellow";
			}
			if (abs(seconds - rightExpectedTimes[noteCount]) <= greenScoreDifferenceThreshold)
			{
				returnString = "rightGreen";
			}
		}

		noteCount++;
		// if returnString == "", then none of the above six return statements were executed
		return returnString;

	}

};