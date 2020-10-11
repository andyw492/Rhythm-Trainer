#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Network.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include <iostream>
#include <vector>

#include "Settings.h"

using namespace std;

// InputAnalyzer takes in a key press and elapsed time, and returns how accurate the key press was
// to the expected time that is derived from the bpm and noteInterval
class InputAnalyzer 
{
	vector<float> expectedTimes;
	vector<float> recordedTimes;
	int noteCount = 0;

public:

	InputAnalyzer()
	{
		// load expectedTimes
		// note n is expected at (60 / bpm) * (n - 1) seconds
		// (e.g. the first note is expected at 0 seconds)
		for (int i = 0; i < Settings::numBeats; i++)
		{
			expectedTimes.push_back(float((60.0 / Settings::bpm) * i));
		}
	}

	string getPressAccuracy(sf::Keyboard::Key key, float seconds)
	{
		recordedTimes.push_back(seconds);

		// thresholds for "red", "yellow" and "green" scores:
		// red score if >= 0.2 beats off
		// yellow score if < 0.2 and > 0.1 beats off
		// green score if <= 0.1 beats off

		// 0.2 beats = 0.2 * (60 / bpm) seconds
		// 0.1 beats = 0.1 * (60 / bpm) seconds

		float redScoreDifferenceThreshold = (0.2 * 60 / Settings::bpm);
		float greenScoreDifferenceThreshold = (0.1 * 60 / Settings::bpm);
		cout << "red threshold: " << redScoreDifferenceThreshold << endl;
		cout << "green threshold: " << greenScoreDifferenceThreshold << endl;

		if (key == Settings::leftKey)
		{
			cout << "difference is " << seconds - expectedTimes[noteCount] << endl;
			if (seconds - expectedTimes[noteCount] >= redScoreDifferenceThreshold)
			{
				return "leftRed";
			}
			if (seconds - expectedTimes[noteCount] < redScoreDifferenceThreshold &&
				seconds - expectedTimes[noteCount] > greenScoreDifferenceThreshold)
			{
				return "leftYellow";
			}
			if (seconds - expectedTimes[noteCount] <= greenScoreDifferenceThreshold)
			{
				return "leftGreen";
			}
		}

		else 
		if (key == Settings::rightKey)
		{
			if (seconds - expectedTimes[noteCount] >= redScoreDifferenceThreshold)
			{
				return "rightRed";
			}
			if (seconds - expectedTimes[noteCount] < redScoreDifferenceThreshold &&
				seconds - expectedTimes[noteCount] > greenScoreDifferenceThreshold)
			{
				return "rightYellow";
			}
			if (seconds - expectedTimes[noteCount] <= greenScoreDifferenceThreshold)
			{
				return "rightGreen";
			}
		}

		// if for some reason none of the above six return statements were executed
		return "getPressAccuracyError";

	}

};