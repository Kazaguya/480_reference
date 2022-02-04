/*******************************************************************************
 * File:        Process.cc
 * Programmer:  Joseph McDade
 * zID:         1757419
 * Due Date:    10/12/2018
 *
 * Purpose:     Class definitions for the Process object class
 ******************************************************************************/

#include "Process.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////  Constructors  ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
 * Default Constructor
 * Arguments: none
 * Notes:     Builds the processes
 ******************************************************************************/
Process::Process()
{
	ProcessName = "NULL";
	Priority = 0;
	ProcessID = 0;
	ArrivalTime = 0;
	Sub = 0;
	CPUTimer = 0;
	IOTimer = 0;
	CPUTotal = 0;
	ITotal = 0;
	OTotal = 0;
	CPUCount = 0;
	ICount = 0;
	OCount = 0;
	Timestamp = 0;
	TotalQueuetime = 0;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// Get Methods /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
 * Method:		char Process::getNextEvent()
 * Arguments:	none
 * Returns:		char indicating which operation to perform next
 * Notes:			Called when an operation is complete to move to next item in
 *						History array and returns the operation found
 ******************************************************************************/
char Process::getNextEvent(){
	Sub++;
	return(History[Sub].Operation);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// Set Methods /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
 * Method:		void Process::importQueue(queue<string> &inputQueue, pid_t newPID)
 * Arguments: &inputQueue - A queue of strings read from a file that contains
 *													values for the initialization of the process
 *						newPID - The PID of the new process
 * Returns:   none
 * Notes:     Changes a process to have values passed as a queue
 ******************************************************************************/
void Process::importQueue(queue<string> &inputQueue, pid_t newPID){
	ProcessID = newPID;
	ProcessName = inputQueue.front();
	inputQueue.pop();
	Priority = stoi(inputQueue.front());
	inputQueue.pop();
	ArrivalTime = stoi(inputQueue.front());
	inputQueue.pop();
	for(int i = 0; i < 10; i++)
	{	// Place operations in History array
		History[i].Operation = inputQueue.front().c_str()[0];
		inputQueue.pop();
		History[i].Value = stoi(inputQueue.front());
		inputQueue.pop();
	}
	while(!inputQueue.empty())
	{	// Empty the inputQueue
		inputQueue.pop();
	}
}
/*******************************************************************************
 * Method:		int Process::iterateCPUTimer()
 * Arguments: none
 * Returns:   Current number of CPU cycles
 * Notes:			Used to iterate the time in CPU and compare to cycles needed
 ******************************************************************************/
int Process::iterateCPUTimer(){
	CPUTimer++;
	return(CPUTimer);
}
/*******************************************************************************
 * Method:		int Process::iterateIOTimer()
 * Arguments:	none
 * Returns:		Current number of IO cycles
 * Notes:			Used to iterate the time in IO and compare to cycles needed
 ******************************************************************************/
int Process::iterateIOTimer(){
	IOTimer++;
	return(IOTimer);
}
/*******************************************************************************
 * Method:		void Process::addQueuetime(int timer)
 * Arguments:	timer - Current cycle
 * Returns:		none
 * Notes:			Adds time spent in a queue to TotalQueuetime
 ******************************************************************************/
void Process::addQueuetime(int timer){
	TotalQueuetime += timer - Timestamp;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// Operator Overloads //////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
 * Method:		bool Process::operator>(const Process& rhs) const
 * Arguments:	&rhs - address of Process to compare against
 * Returns:		bool result of comparison
 * Notes:			Used by priority queue to insert items in proper order
 ******************************************************************************/
bool Process::operator>(const Process& rhs) const{
	if(this->Priority == rhs.Priority)
		return(this->Timestamp < rhs.Timestamp);
	return(this->Priority > rhs.Priority);
}
/*******************************************************************************
 * Method:		bool Process::operator<(const Process& rhs) const
 * Arguments:	&rhs - address of Process to compare against
 * Returns:		bool result of comparison
 * Notes:			Used by priority queue to insert items in proper order
 ******************************************************************************/
bool Process::operator<(const Process& rhs) const{
	if(this->Priority == rhs.Priority)
		return(this->Timestamp > rhs.Timestamp);
	return(this->Priority < rhs.Priority);
}
/*******************************************************************************
 * Method:		Process& Process::operator=(const Process& alpha)
 * Arguments:	&alpha - Process to copy
 * Returns:		Process with contents copied from alpha
 * Notes:			Used to set one Process to the same values as another
 ******************************************************************************/
Process& Process::operator=(const Process& alpha)
{
	this->ProcessName = alpha.ProcessName;
	this->Priority = alpha.Priority;
	this->ProcessID = alpha.ProcessID;
	this->ArrivalTime = alpha.ArrivalTime;
	this->Sub = alpha.Sub;
	this->CPUTimer = alpha.CPUTimer;
	this->IOTimer = alpha.IOTimer;
	this->CPUTotal = alpha.CPUTotal;
	this->ITotal = alpha.ITotal;
	this->OTotal = alpha.OTotal;
	this->CPUCount = CPUCount;
	this->ICount = alpha.ICount;
	this->OCount = alpha.OCount;
	this->Timestamp = alpha.Timestamp;
	// Copy all elements of History
	for(int i = 0; i < 10; i++)
	{	this->History[i] = alpha.History[i]; }
	return *this;
}
