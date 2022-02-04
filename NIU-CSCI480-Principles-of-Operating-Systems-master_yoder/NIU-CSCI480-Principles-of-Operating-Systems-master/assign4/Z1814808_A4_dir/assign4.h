#include<iostream>
#include<string>
#include<queue>
#include<fstream>
#include<vector>
using namespace std;

const int MAX_TIME = 500;
const int AT_ONCE = 5;
const int QUEUE_SIZE = 20;
const int ARRAY_SIZE = 10;
const int HOW_OFTEN = 25;

struct Process {
	string name;
	int priority;
	int arrivalTime;
	int processID;

	pair<char, int> history[ARRAY_SIZE];
	int historySub = 0;

	int CPUTimer = 0;
	int IOTimer = 0;

	int CPUTotal = 0;
	int ITotal = 0;
	int OTotal = 0;

	int CPUCount = 0;
	int ICount = 0;
	int OCount = 0;

	Process(string name, int priority, int arrivalTime, int processID, pair<char, int> history[]) {
		this->name = name;
		this->priority = priority;
		this->arrivalTime = arrivalTime;
		this->processID = processID;
		
		for(int i = 0; i < ARRAY_SIZE; i++){
		  this->history[i] = history[i];
		}
		CPUTimer = 0;
		IOTimer = 0;
		
		CPUTotal = 0;
		ITotal = 0;
		OTotal = 0;
		
		CPUCount = 0;
		ICount = 0;
		OCount = 0;
	};
};

/****************************
 *
 * Function that does the 
 * compar for the priority
 * queue. It will let the older
 * Process if the priority is
 * the same.
 *
 * ************************/
struct comp{
  bool operator()(const Process* lhs, const Process* rhs)
  {
    if(lhs->priority < rhs->priority)   //If the priorities are different
        return lhs->priority < rhs->priority;

    return false; //If the priroty is the same, leave the olders one at the top
  };
};



void readFile(const char* filename);
void state(int timer);
