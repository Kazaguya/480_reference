/****************************************************************
PROGRAM:   Assign4
AUTHOR:    Spencer Yoder
LOGON ID:  Z1814808
DUE DATE:  Friday, October 12

FUNCTION: Process Timeing Simulation
******************************************************************/
#include"assign4.h"

queue<Process*> entry_queue;
priority_queue<Process*, vector<Process*>, comp> ready_queue;
priority_queue<Process*, vector<Process*>, comp> input_queue;
priority_queue<Process*, vector<Process*>, comp> output_queue;

Process *Active;
Process *IActive;
Process *OActive;

int main(int argc, char** argv)
{
	int timer = 0;
	int goodEnd = 0;	//Counter for each clock tick
	int cpuIdle = 0;	//Time spent when CPU Idle
	int iIdle = 0;		//Time spent when Input was idle
	int oIdle = 0;		//Time spent when output was idle

	if (argc < 2) {	//Check for file as arguemnt
		cerr << "args: input-file-name\n";
		return 1;
	}

	readFile(argv[1]);	//call function to read file

	while (timer <= MAX_TIME){	//While we are under the MAX_Time
		while (ready_queue.size() < AT_ONCE && !entry_queue.empty()) {	//Loop through the entry queue
		  if (entry_queue.front()->arrivalTime <= timer) {	//If the top element is ready to go into the ready queue
				ready_queue.push(entry_queue.front());	//Push it, Pull it
				cout << "Process " << entry_queue.front()->processID << " Moved from entry queue to ready queue at time " << timer <<endl;
				entry_queue.pop();	//Bop it,
			}

			else
				break;	//First element is not ready no move
		}

		if(timer % HOW_OFTEN == 0)//Print out status reports
                        state(timer);
		
		//If all the queue and pointers are empty, quit
		if(entry_queue.empty() && ready_queue.empty() && input_queue.empty() && output_queue.empty() && Active == NULL && IActive == NULL && OActive == NULL)
			break;

		//CPU Burst//
		if(Active != NULL){	//If there is an active process
			if(Active->CPUTimer == Active->history[Active->historySub].second){	//If process is done running
				Active->CPUTotal += Active->CPUTimer; //Add total run time to the CPUTotal
				Active->CPUCount++; //Add 1 to the number of CPU Bursts

				Active->CPUTimer = 0; //Clear the Timer
				Active->historySub++; //Add 1 to the array index
				
				if(Active->history[Active->historySub].first == 'I'){	//Check to see what burst is next
				  input_queue.push(Active);	//add to the input queue
				  cout << "Process " << Active->processID << " Moved from Active to input queue at time" << timer <<endl;
				}
				else if(Active->history[Active->historySub].first == 'O'){	//check for output
				    output_queue.push(Active);	//Push to output
				    cout << "Process " << Active->processID << " Moved from Active to output queue at time" << timer <<endl;
				}
				else{	//There is nothing else for the process
					cout << "-------------------------" <<endl;	//Output it
					cout << ">>A Process has Terminated!" <<endl;
					cout << "    Name: " << Active->name <<endl;
					cout << "    Process ID: " << Active->processID <<endl;
					cout << "    Priority: " << Active->priority <<endl;
					cout << "    CPU Burst: " << Active->CPUCount <<endl;
					cout << "    Input Burst: " << Active->ICount <<endl;
					cout << "    Output Burst: " << Active->OCount <<endl;
					cout << "    Cycle Time: " << timer <<endl;
					cout << "------------------------" <<endl;
					goodEnd++;	//Count how many process ended on their own
				}

				Active = NULL;	//Clear the Active Pointer
			}

			else	//If the process needs to keep running
				Active->CPUTimer++;	//add to the counter

		}
		else if (Active == NULL && ready_queue.top()->history[ready_queue.top()->historySub].first == 'C'){	//If Active is NULL and the next is CPU
			Active = ready_queue.top();	//Save pointer
			cout << "Process " << Active->processID << " Moved from ready queue to Active at time" << timer <<endl;
			ready_queue.pop();	//Pop from the ready_Queue
		}
		else	//CPU Idle time
			cpuIdle++;



		//Input Burst//
		if(IActive != NULL){ //If there is Input process
			if(IActive->IOTimer == IActive->history[IActive->historySub].second){ //Check the run time on the process
				IActive->ITotal += IActive->IOTimer;	//save the rur time
				IActive->ICount++;	//add to the coutner

				IActive->IOTimer = 0; //Reset the run time timers
				IActive->historySub++; //Move the array index

				cout << "Process " << IActive->processID << " Moved from Input to Ready queue at time" << timer <<endl;

				ready_queue.push(IActive); //Put it on the ready queue
				IActive = NULL; //clear the IActive pointer
			}

			else{ //Move the array index
				IActive->IOTimer++;
			}
		}
		else if (IActive == NULL && !input_queue.empty()){	//If the pointer is empty and there is something in the input queue
			IActive = input_queue.top();	//save the pointer
			input_queue.pop();	//pop from the queue
		}
		else	//Keep track of idle time
			iIdle++;



		//Output Burst//
		if(OActive != NULL){	//If the poitner is not NULL
			if(OActive->IOTimer == OActive->history[OActive->historySub].second){	//Check to see if the process is done
				OActive->OTotal += OActive->IOTimer;	//Save the time
				OActive->OCount++;	//add to the count

				OActive->IOTimer = 0;	//reset timeer
				OActive->historySub++;	//Mode array index

				cout << "Process " << OActive->processID << " Moved from output to Ready queue at time" << timer <<endl;

				ready_queue.push(OActive);	//put back on ready queue
				OActive = NULL;	//clear pointer
			}

			else{	//Output running
				OActive->IOTimer++;
			}
		}
		else if (OActive == NULL && !output_queue.empty()){	//If the pointer is NULL
			OActive = output_queue.top();//Save the top element
			output_queue.pop();	//Pop from queue
		}
		else	//Idle time
			oIdle++;

		timer++;	//CPU cycle timer
	}

	//Report to print at the end of the program
	cout << "*********************************" <<endl;
	cout << "Simulation Complete" <<endl;
	cout << "Cycles: " << timer <<endl;
	cout << "Entry Size: " << entry_queue.size() <<endl;
	cout << "Ready Size: " << ready_queue.size() <<endl;
	cout << "Input Size: " << input_queue.size() <<endl;
	cout << "Output Size: " << output_queue.size() <<endl;
	cout << "Processes Terminated Normally: " << goodEnd <<endl;
	cout << "CPU Idle Time: " << cpuIdle <<endl;
	cout << "Input Idle Time: " << iIdle <<endl;
	cout << "Output Idle Time: " <<oIdle <<endl;
	cout << "*********************************" <<endl;


	return 0;
}

/**************************************
 *
 * Function that reads in from the file
 * and them save each process in a queue.
 *
 * **********************************/
void readFile(const char* filename)
{
	string name;	//Temp vars to send to process object
	int priority;
	int arrivalTime;
	int processCounter = 100;
	pair<char, int> history[ARRAY_SIZE];


	ifstream infile("data4.txt", ios::in);	//setup read file

	while (infile >> name >> priority >> arrivalTime) {	//Read in the first line of process
		if (name == "STOPHERE")	//Stop if stophere
			break;

		infile >> history[0].first >> history[0].second	//Read in the second file and save the burst and numbers
			>> history[1].first >> history[1].second
			>> history[2].first >> history[2].second
			>> history[3].first >> history[3].second
			>> history[4].first >> history[4].second
			>> history[5].first >> history[5].second
			>> history[6].first >> history[6].second
			>> history[7].first >> history[7].second
			>> history[8].first >> history[8].second
			>> history[9].first >> history[9].second;

		entry_queue.push(new Process(name, priority, arrivalTime, processCounter, history));	//Make new Object and put on the entry queue
		processCounter++;	//Process counter
	}
}


/*********************************
 *
 * Function that is user to print
 * out the ctate check during the 
 * simulation. It prints out the 
 * current info.
 *
 * *****************************/
void state(int timer){
	queue<Process*> entry_temp;	//Temp queue to pop the Process out of for printing
	priority_queue<Process*, vector<Process*>, comp> ready_temp;
	priority_queue<Process*, vector<Process*>, comp> input_temp;
	priority_queue<Process*, vector<Process*>, comp> output_temp;


	cout << "-------------------------" <<endl;	//Print out info
	cout << "Current State Check." <<endl;
	cout << "  Cycle: " << timer <<endl;

	
	if(entry_queue.size() != 0){	//Display all Process in the entry queue
		cout << "  Entry Queue: ";
		entry_temp = entry_queue;
		while(entry_temp.size() != 0){
			cout << entry_temp.front()->processID << " ";
			entry_temp.pop();
		}
		cout <<endl;
	}

	if(Active == NULL)	//If the Process is NULL
		cout << "  Active ID: NULL" <<endl;
	else{	//Else print the ID of the process
	  cout << "  Active ID: " << Active->processID <<endl;
	}
	
	cout << "  Active Queue: ";
	ready_temp = ready_queue;	//Print all process in the queue
	while(ready_temp.size() != 0){
	  cout << ready_temp.top()->processID << " ";
	  ready_temp.pop();
	}
	cout <<endl;
	
	

	if(IActive == NULL) //If there is an input process
		cout << "  Input ID: NULL" <<endl;
	else{
	  cout << "  Input ID: " << IActive->processID <<endl;
	}
	cout << "  Input Queue: ";
	
	input_temp = input_queue; //Print all process input queue
	while(input_temp.size() != 0){
	  cout << input_temp.top()->processID << " ";
	  input_temp.pop();
	}
	cout <<endl;
	


	if(OActive == NULL)
		cout << "  Output ID: NULL" <<endl;
	else{
	  cout << "  Output ID: " << OActive->processID <<endl;
	}
	cout << "  Output Queue: ";
	
	output_temp = output_queue;
	while(output_temp.size() != 0){
	  cout << output_temp.top()->processID << " ";
	  output_temp.pop();
	}
	cout <<endl;
	cout << "-------------------------" <<endl;
}

