/*******************************************************************************
   PROGRAM:			CSCI 480 Assignment 4
   PROGRAMMER:	Joseph McDade
   LOGON ID:		z1757419
   DUE DATE:		10/12/2018
	 Note:				Uses hard tabs set to display as 2 spaces

   FUNCTION:		Simulate a single processor and dealing with multiprocessing.
*******************************************************************************/

#include "Process.h"
#include "Assign4.h"

int main() {
	// Initialize Ready Queue
	if(!entry.empty())
	{	// If the entry queue isn't empty, check if need to add to ready queue
		while(entry.front().getArrivalTime() <= Timer && process_amount < AT_ONCE)
		{	// Add procs to ready if arrived and less than AT_ONCE
			process_amount++;
			Process x = entry.front();
			x.setTimestamp(Timer);
			cout << "Process " << x.getName()
				<< " is moving to ready queue at timer: " << Timer << endl;
			ready.push(x);
			entry.pop();
			if(!entry.empty()) // If the entry queue isn't empty, get next item
				x = entry.front();
		}
	}
	while((Timer < MAX_TIME) && process_amount > 0)
	{	// While we haven't reached MAX_TIME and still processes to process
		if(Timer % HOW_OFTEN == 0)
			printStats(); // Print status of simulation if at multiple of HOW_OFTEN
		if(!entry.empty() && process_amount < AT_ONCE)
		{	// If the entry queue isn't empty and we can process more, check next proc
			if(entry.front().getArrivalTime() <= Timer)
			{	// If process has arrived, move to ready queue
				process_amount++;
				Process x = entry.front();
				x.setTimestamp(Timer); // Record when entered queue for priority
				cout << endl << "Process " << x.getName()
					<< " is moving to ready queue at timer: " << Timer << endl;
				ready.push(x);
				entry.pop();
			}
		}
		// Activate processes from queues //
		if(Active == nullptr && !ready.empty())
		{	// Get process from ready queue if no active process and ready not empty
			Process x = ready.top();
			ready.pop();
			Active = new Process;
			*Active = x;
			Active->incrementCPUCount();
		} else if(Active == nullptr && ready.empty())
		{	// No active and no ready processes, CPU is idle
			cpu_idle++;
		}
		if(IActive == nullptr && !input.empty())
		{	// Get process from input queue if no active input process
			Process x = input.top();
			input.pop();
			IActive = new Process;
			*IActive = x;
			IActive->incrementICount();
		}
		if(OActive == nullptr && !output.empty())
		{	// Get process from output queue if no active output process
			Process x = output.top();
			output.pop();
			OActive = new Process;
			*OActive = x;
			OActive->incrementOCount();
		}

		// Process the Active processes //
		if(Active)
		{	// If a process is active, iterate it a cycle
			Active->incrementCPUTotal();
			if(Active->iterateCPUTimer() == Active->getValue())
			{	// If at number of cycles to complete, move to next operation
				Active->addQueuetime(Timer);
				Active->clearCPUTimer();
				moveQueue(Active);
				delete Active;
				Active = nullptr;
			}
		}
		if(IActive)
		{	// If a process is iactive, iterate it a cycle
			IActive->incrementITotal();
			if(IActive->iterateIOTimer() == IActive->getValue())
			{	// If at number of cycles to complete, move to next operation
				IActive->addQueuetime(Timer);
				IActive->clearIOTimer();
				moveQueue(IActive);
				delete IActive;
				IActive = nullptr;
			}
		}
		if(OActive)
		{	// If a process is oactive, iterate it a cycle
			OActive->incrementOTotal();
			if(OActive->iterateIOTimer() == OActive->getValue())
			{	// If at number of cycles to complete, move to next operation
				OActive->addQueuetime(Timer);
				OActive->clearIOTimer();
				moveQueue(OActive);
				delete OActive;
				OActive = nullptr;
			}
		}

		// Prepare next cycle
		Timer++;
	}
	// Summarize end state of simulation
	cout << "********************************************************************"
		<< endl << "  Simulation Terminated" << endl << endl
		<< "  Total cycles:" << Timer << endl
		<< "  Total terminated processes:" << terminated_total << endl
		<< "  Total CPU idle time:" << cpu_idle << endl << endl;
	// Print left overs in queues
	if(!entry.empty())
	{	// Print leftovers from entry queue
		cout << "Processes in the entry queue:" << endl;
		while(!entry.empty()){
			Process X = entry.front();
			entry.pop();
			cout << " " << X.getProcessID();
		}
		cout << endl;
	}
	if(!ready.empty())
	{	// Print leftovers from ready queue
		cout << "Processes in the ready queue:" << endl;
		while(!ready.empty()){
			Process X = ready.top();
			ready.pop();
			cout << " " << X.getProcessID();
		}
		cout << endl;
	}
	if(!input.empty())
	{	// Print leftovers from input queue
		cout << "Processes in the input queue:" << endl;
		while(!input.empty()){
			Process X = input.top();
			input.pop();
			cout << " " << X.getProcessID();
		}
		cout << endl;
	}
	if(!output.empty())
	{	// Print leftovers from output queue
		cout << "Processes in the output queue:" << endl;
		while(!output.empty()){
			Process X = output.top();
			output.pop();
			cout << " " << X.getProcessID();
		}
		cout << endl;
	}
	if(!terminated.empty())
	{	// Print leftovers from terminated queue
		cout << "Processes that terminated:" << endl;
		while(!terminated.empty()){
			Process X = terminated.front();
			terminated.pop();
			cout << " " << X.getProcessID();
		}
		cout << endl;
	}
  return 0;
}
/////////////////////////////////  Functions  //////////////////////////////////
/*******************************************************************************
Function:		queue<Process> buildEntryQueue()
Use:				Builds a queue from an input file called data4.txt
Arguments:	none
Returns:		A queue of processes
*******************************************************************************/
queue<Process> buildEntryQueue(){
	queue<Process> entry;
	ifstream inFile;
	inFile.open("data4.txt");
	if(inFile.fail())
	{	// If the file failed to open, alert user and quit
		cerr << "Input file data4.txt failed to open" << endl;
		exit(-1);
	}
	string line;
	char* tokens;
	Process p;
	pid_t PID = 100;
	queue<string> inputQueue;
	while(getline(inFile, line))
	{	// Get first line of parameters for Process
		tokens = strtok((char*)line.c_str(), " ");
		if(strcmp(tokens, "STOPHERE") == 0)
			break;
		while(tokens)
		{	// While there are more items on first line
			inputQueue.push(string(tokens));
			tokens = strtok(NULL, " ");
		}
		// Get second line of parameters for Process
		getline(inFile, line);
		tokens = strtok((char*)line.c_str(), " ");
		while(tokens)
		{	// While there are more items on second line
			inputQueue.push(string(tokens));
			tokens = strtok(NULL, " ");
		}
		p.importQueue(inputQueue, PID++);
		entry.push(p);
	}
	return entry;
}
/*******************************************************************************
Function:		void terminate_proc()
Use:				Prints statistics of active process, adds it to the terminated queue
						then marks the process as terminated by decrementing process_amount
Arguments:	none
Returns:		none
*******************************************************************************/
void terminate_proc(){
	cout << endl
		<< "** Terminating " << Active->getName() << " that had priority: "
		<< Active->getPriority() << " **" << endl
		<< setw(32) << left << "  Number of CPU bursts:"
			<< setw(4) << right << Active->getCPUCount() << endl
		<< setw(32) << left << "  Number of Input bursts:"
			<< setw(4) << right << Active->getICount() << endl
		<< setw(32) << left << "  Number of Output bursts:"
			<< setw(4) << right << Active->getOCount() << endl
		<< setw(32) << left << "  Time spent in CPU:"
			<< setw(4) << right << Active->getCPUTotal() << endl
		<< setw(32) << left << "  Time spent in Input:"
			<< setw(4) << right << Active->getITotal() << endl
		<< setw(32) << left << "  Time spent in Output:"
			<< setw(4) << right << Active->getOTotal() << endl
		<< setw(32) << left << "  Time spent waiting in queues:"
			<< setw(4) << right << Active->getQueuetime() << endl
		<< setw(32) << left << "  Current cycle: "
			<< setw(4) << right << Timer << endl;;
	terminated.push(*Active);
	process_amount--;
	terminated_total++;
}
/*******************************************************************************
Function:		void moveQueue(Process *X)
Use:				Move a process from I/O/Active process to appropriate queue or
						terminates if at end of needed bursts
Arguments:	*X - pointer to a process that needs to be moved
Returns:		none
*******************************************************************************/
void moveQueue(Process *X){
	char nextEvent = X->getNextEvent();
	if(nextEvent == 'I'){ // Move to input queue
		X->setTimestamp(Timer);
		input.push(*X);
	} else if(nextEvent == 'O'){ // Move to output queue
		X->setTimestamp(Timer);
		output.push(*X);
	} else if(nextEvent == 'C'){ // Move to ready queue
		X->setTimestamp(Timer);
		ready.push(*X);
	} else { // Process is finished, terminate
		terminate_proc();
	}
}
/*******************************************************************************
Function:		void printStats()
Use:				Prints the current statistics of the simulation
Arguments:	none
Returns:		none
*******************************************************************************/
void printStats(){
	queue<Process> temp_entry, temp_ready, temp_input, temp_output;
	cout << endl
		<< "***** Current Simlation Status as of Timer: "
		<< setw(4) << right << Timer << " *****" << endl;
	if(Active) // If there is an active process print
			cout << Active->getName() << " is Active process with PID: "
			<< Active->getProcessID() << endl;
	if(IActive) // If there is an iactive process print
		cout << IActive->getName() << " is IActive process with PID: "
			<< IActive->getProcessID() << endl;
	if(OActive) // If there is an oactive process print
		cout << OActive->getName() << " is OActive process with PID: "
			<< OActive->getProcessID() << endl;
	if(!entry.empty())
	{	// If the entry queue is not empty, print its contents
		cout << "Current processes in entry queue:";
		while(!entry.empty())
		{	// Iterate through each item in entry
			Process X = entry.front();
			cout << " " << X.getProcessID();
			temp_entry.push(X);
			entry.pop();
		}
		cout << endl;
		while(!temp_entry.empty())
		{	// Rebuild entry queue
			entry.push(temp_entry.front());
			temp_entry.pop();
		}
	}
	if(!ready.empty())
	{	// If the ready queue is not empty, print its contents
		cout << "Current processes in ready queue:";
		while(!ready.empty())
		{	// Iterate through each item in ready
			Process X = ready.top();
			cout << " " << X.getProcessID();
			temp_ready.push(X);
			ready.pop();
		}
		cout << endl;
		while(!temp_ready.empty())
		{	// Rebuild ready queue
			ready.push(temp_ready.front());
			temp_ready.pop();
		}
	}
	if(!input.empty())
	{	// If the input queue is not empty, print its contents
		cout << "Current processes in input queue:";
		while(!input.empty())
		{	// Iterate through each item in input
			Process X = input.top();
			cout << " " << X.getProcessID();
			temp_input.push(X);
			input.pop();
		}
		cout << endl;
		while(!temp_input.empty())
		{	// Rebuild input queue
			input.push(temp_input.front());
			temp_input.pop();
		}
	}
	if(!output.empty())
	{	// If the output queue is not empty, print its contents
		cout << "Current processes in output queue:";
		while(!output.empty())
		{	// Iterate through each item in output
			Process X = output.top();
			cout << " " << X.getProcessID();
			temp_output.push(X);
			output.pop();
		}
		cout << endl;
		while(!temp_output.empty())
		{	// Rebuild output queue
			output.push(temp_output.front());
			temp_output.pop();
		}
	}
}
