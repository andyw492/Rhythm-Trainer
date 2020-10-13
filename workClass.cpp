#include <iostream>
#include <thread>
using namespace std;

class workClass
{
	static bool s_Finished;

	public:

	workClass()
	{
		//s_Finished = false;
	}

	void setFinished(bool f)
	{
		s_Finished = f;
	}

	void DoWork(string s, int i)
	{
		using namespace std::literals::chrono_literals;

		while(!s_Finished)
		{
			cout << s_Finished << endl;
			cout << s << i << endl;
			this_thread::sleep_for(0.5s);
		}
	}
};