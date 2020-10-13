#include <iostream>
#include <vector>
#include <thread>
using namespace std;

#include "workClass.cpp"
bool workClass::s_Finished = false;

int main() 
{ 
	vector<thread> workerThreads;
	int workerThreadsCount = 0;
	while(true)
	{
		cout << "Default State" << endl;
		string t; cin >> t;

		workClass wc;

		thread worker(&workClass::DoWork, workClass(), "qwer", 5);
		workerThreads.push_back(worker);
		workerThreadsCount++;

		string s = "";
		bool endAction = false;
		while(!endAction)
		{
			cout << "check" << endl;
			cin >> s;
			if(s == "q")
			{
				endAction = true;
			}
		}		
		
		wc.setFinished(true);
	}


	for(int i = 0; i < workerThreads.size(); i++)
	{
		workerThreads[i].join();
	}


    return 0; 
} 