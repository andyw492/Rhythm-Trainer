#include <iostream>
#include <string>
#include <thread>
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/Network.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

using namespace std;

class tempTempoBall
{
	static bool s_doWork;
	static bool s_Finished;

	static bool ballMove;
	static bool ballFinished;

public:

	tempTempoBall()
	{
		//s_Finished = false;
	}

	void setDoWork(bool f)
	{
		s_doWork = f;
	}

	void setFinished(bool f)
	{
		s_Finished = f;
	}

	void move(sf::RenderWindow window)
	{
		while (!s_Finished)
		{
			using namespace std::literals::chrono_literals;

			while (s_doWork)
			{
				string s = "qwer";
				int i = 5;
				//cout << s_Finished << endl;
				cout << s << i << endl;
				this_thread::sleep_for(0.5s);
			}
		}

	}
};