#ifndef PROCESS_H
#define PROCESS_H
/*******************************************************************************
 * File:        Process.h
 * Programmer:  Joseph McDade
 * zID:         1757419
 * Due Date:    10/12/2018
 *
 * Purpose:     Header file for Process object class
 ******************************************************************************/
#include <iostream>
#include <stdlib.h>
#include <queue>

using namespace std;

struct event
{
	char Operation;
	int Value;
};

class Process
{
  public:
    Process();

    //Accessor Methods
		//Get Methods
		inline string getName() const { return(ProcessName); };
		inline int getPriority() const { return(Priority); };
		inline int getProcessID() const { return(ProcessID); };
		inline int getArrivalTime() const { return(ArrivalTime); };
		inline int getTimestamp() const { return(Timestamp); };
		inline int getValue() const { return(History[Sub].Value); };
		inline int getCPUTotal() const { return(CPUTotal); };
		inline int getCPUCount() const { return(CPUCount); };
		inline int getITotal() const { return(ITotal); };
		inline int getICount() const { return(ICount); };
		inline int getOTotal() const { return(OTotal); };
		inline int getOCount() const { return(OCount); };
		char getNextEvent();
		inline int getQueuetime() const { return(TotalQueuetime); };

		//Set Methods
		void importQueue(queue<string>&, pid_t);
		void setTimestamp(int newTimestamp){ Timestamp = newTimestamp; };
		int  iterateCPUTimer();
		inline void clearCPUTimer() { CPUTimer = 0; };
		int  iterateIOTimer();
		inline void clearIOTimer() { IOTimer = 0; };
		inline void incrementSub(){ Sub++; };
		inline void incrementICount() { ICount++; };
		inline void incrementITotal() { ITotal++; };
		inline void incrementOCount() { OCount++; };
		inline void incrementOTotal() { OTotal++; };
		inline void incrementCPUTotal(){ CPUTotal++; };
		inline void incrementCPUCount() { CPUCount++; };
		void addQueuetime(int);

    //Utility Methods

		//Operator Overload
		bool operator>(const Process&) const;
		bool operator<(const Process&) const;
		Process& operator=(const Process&);

  private:
		string ProcessName;	// Process name
		int Priority;				// Priority of the process
		int ProcessID;			// ID of the process
		int ArrivalTime;		// Which cycle the process is allowed to enter ready
												// queue if not at AT_ONCE
		event History[10];	// Operations to be performed
		int Sub;						// Current operation needed to be performed
		int CPUTimer;				// Counts clock ticks of current CPU burst
		int IOTimer;				// Counts clock ticks of current I/O burst
		int CPUTotal;				// Counts total CPU clocks
		int ITotal;					// Counts total clocks as IActive
		int OTotal;					// Counts total clocks as OActive
		int CPUCount;				// Counts number of times process needed CPU burst
		int ICount;					// Counts number of times needed Input burst
		int OCount;					// Counts number of times needed Output burst
		int Timestamp;			// Timestamp of TIMER when entered queue
		int TotalQueuetime;	// Total time spent in a queue other than entry
};

#endif
