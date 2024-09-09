#include <string>
#include <vector>
#include <utility>
#include <iostream>

using namespace std;

class process {
    public: 
        process();
        
        string processName;
        int processID;
        int arrivalTime;
        int processWait;
        int CPUTimer;
        int ITimer;
        int OTimer;
        int CPUTotal;
        int ITotal;
        int OTotal;
        int CPUCount;
        int ICount;
        int OCount;
        int startTime = 0;
        int endTime = 0;
        int idleTime = 0;
        
        int history_sub;
        vector<pair<string, int>> history;
        string toString();
        string getProcessType();
        int getProcessTotal();
};