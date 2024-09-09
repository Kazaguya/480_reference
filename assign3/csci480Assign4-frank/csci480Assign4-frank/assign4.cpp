/*

    Francia Morales Diaz
    CSCI 480 Assign 4
    03/29/22

    CPU scheduling simulator
*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <deque>
#include <stdlib.h>
#include <utility>
#include "process.h"

//constants
#define MAX_TIME 500
#define IN_USE 5
#define HOW_OFTEN 25

using namespace std;

int main(int argc, char* argv[]){

    //4 deques needed
    std::deque<Process*> EntryQueue,ReadyQueue,InputQueue,OutPutQueue;
    Process processes;
    string fileName;
    bool isIdle = false;
    Process* Iactive= NULL;
    Process* cpu_active = NULL;
    Process* Oactive = NULL;
    fileName = "tiny.txt";
    int processInPlay=0;
    int idleTIme=0;
    
    //if no command line argument given, tiny.txt default file to be used
    if (argc >1){
    fileName = argv[1];
    }
    string inputLine;
    ifstream inFile(fileName);

    //checks if file open was successful 
    if (!inFile){
        cout<<"ERROR OPENING FILE"<<endl;
    }
    string processName;
    string arrivalTime; 

    //default start
    processes.processID = 100;
    //reading from file
    while (inFile){
    inFile >> processName;  
    inFile >> arrivalTime;

    processes.processName = processName;
    processes.arrivalTime = stoi(arrivalTime);
   

    char cpuBurstType;
    int cpuDuration;
    pair <char,int> cpuPairs;
  
    //making pair of char and int to add to history of process
    for (int start =0; start < 10; start++){
            inFile >> cpuBurstType;
            inFile >> cpuDuration;

            cpuPairs.first = cpuBurstType;
            cpuPairs.second = cpuDuration;

            processes.history.push_back(cpuPairs);
    }

    //using STOPHERE as delimeter
    //wont add any process that has name 
    //initalization of entry queue 
    if (processes.processName!= "STOPHERE"){
        processes.processID++;
        EntryQueue.push_front(&processes);
    }
 
    }

    
    int timer = 0;
    cout<<"Simulation of CPU Scheduling"<<endl<<endl<<endl;


    while (timer <= MAX_TIME || (ReadyQueue.empty() && EntryQueue.empty() && InputQueue.empty()&& OutPutQueue.empty() && cpu_active == NULL && Iactive == NULL && Oactive == NULL)){
    



        if (timer % HOW_OFTEN == 0){

            //checking to see if there is currently a process with a CPU burst to work on 
            //if no process available, will look at ready queue first
            if (cpu_active == NULL){

                //if the ready queue is empty, need to look at entry queue to move process to ready queue 
                if (ReadyQueue.empty()){

                    for (unsigned long int start =0; start<EntryQueue.size();start++){          

                        //only adding process with arrival times that have already arrived and process that just arrived
                        if( EntryQueue[start]->arrivalTime <=timer){             
                        //adding process from entry to ready queue
                        ReadyQueue.push_front(EntryQueue[start]);
                         cout<<"Process "<<EntryQueue[start]->processID<<" has moved from the Entry Queue to the Ready Queue at time " <<timer<<endl;  
                        //removing item from entry queue once process in ready queue
                        EntryQueue.pop_front();
                        //# of processes in play increments
                        processInPlay++;
                        }
                    }
                }
            }
            cout<<endl<<endl;
            cout<<"Status at time "<<timer<<endl; 
            if (cpu_active == NULL){
                cout<<"Active is 0"<<endl;
            }
            else{
                cout<<"Active is "<<cpu_active->processID<<endl;
            }
            if (Iactive ==NULL){
                cout<<"Iactive is 0"<<endl;
            }
            else{
                cout<<"Iactive is "<<Iactive->processID<<endl;
            }
            if (Oactive == NULL){
                cout<<"Oactive is 0"<<endl;
            }
            else{
                cout<<"Oactvie is "<<Oactive->processID<<endl;
            }
            cout<<"Contents of the entry queue"<<endl;
            if (EntryQueue.empty()){
                cout<<"(Empty)"<<endl;
            }
            else{
                for (unsigned long int EntryQueuestart=0; EntryQueuestart<EntryQueue.size();EntryQueuestart++){
                    cout<<EntryQueue[EntryQueuestart]->processID<<endl;
                }
            }
            cout<<"Contents of the ready queue"<<endl;
            if(ReadyQueue.empty()){
                cout<<"(Empty)"<<endl;
            }
            else{
                for (unsigned long int ReadyQueuestart = 0; ReadyQueuestart < ReadyQueue.size();ReadyQueuestart++){
                    cout<<ReadyQueue[ReadyQueuestart]->processID<<endl;
                }
            }
            cout<<"Contents of the input queue"<<endl;
            if(OutPutQueue.empty()){
                cout<<"(Empty)"<<endl;
            }
            else{
                for (unsigned long int ReadyQueuestart = 0; ReadyQueuestart < OutPutQueue.size();ReadyQueuestart++){
                    cout<<OutPutQueue[ReadyQueuestart]->processID<<endl;
                }
            }
            cout<<"Contents of the output queue"<<endl;
            if(OutPutQueue.empty()){
                cout<<"(Empty)"<<endl;
            }
            else{
                for (unsigned long int ReadyQueuestart = 0; ReadyQueuestart < OutPutQueue.size();ReadyQueuestart++){
                    cout<<OutPutQueue[ReadyQueuestart]->processID<<endl;
                }
            }
            //first element in the ready queue will become the cpu active process 
            cpu_active = ReadyQueue.front();
        }
        if (cpu_active!= NULL){
            //subtracting value of burst from pair in history vector of process in ready queue
	    cpu_active->history[cpu_active->subHistory].second--;
            if (cpu_active->history[cpu_active->subHistory].second == 0){
                cpu_active->subHistory++;
                cpu_active->CPUCount++;
                //checking what the next burst is
                //if i ==> input burst
                //o ==> output burst
                //n ==> terminate
                switch(cpu_active->history[cpu_active->subHistory].first){
                    //input
		    case 'I':
                        //cout<<"Input burst detected at time "<<timer<<endl;
                        //moving cpu_active process to InputQueue
			InputQueue.push_front(cpu_active);


                        ReadyQueue.pop_front();
			//input  active process is first process from input queue
                        Iactive = InputQueue.front();

			//removing from inputqueue once input process becomes active
			InputQueue.pop_front();

			//incrementing timer counter second of input burst ticks
                        Iactive->InputBurstTimer++;

			//decrementing first tick of input duration
                        Iactive->history[Iactive->subHistory].second--;

			//cpu active becomes null 
                        cpu_active = NULL;
                        break;
		    //output
                    case 'O':
                        //cout<<"Output burst detected at time "<<timer<<endl;
                        OutPutQueue.push_back(cpu_active);
                        ReadyQueue.pop_front();
                        Oactive = OutPutQueue.front();
			OutPutQueue.pop_front();
                        Oactive->OutputBurstTimer++;
                        Oactive->history[Iactive->subHistory].second--;
                        cpu_active = NULL;
                        break;
                }
            }
            }
	if (Iactive != NULL){
            Iactive->history[Iactive->subHistory].second--;
            if (Iactive->history[Iactive->subHistory].second == 0){
                Iactive->subHistory++;
                Iactive->InputBurstCount++;
	        //cout<<"end of input burst at time: "<<timer<<endl;

		switch(Iactive->history[Iactive->subHistory].first){
                    case 'O':
                        OutPutQueue.push_back(Iactive);
                        Oactive = OutPutQueue.front();
                        OutPutQueue.pop_front();
                        Oactive->OutputBurstTimer++;
                        Oactive->history[Oactive->subHistory].second--;
                        Iactive = NULL;
                        break;
		    /*
                    case 'C':
                        ReadyQueue.push_back(Iactive);
                        cpu_active = ReadyQueue.front();
                        ReadyQueue.pop_front();
                        cpu_active->CPUTimer++;
                        cpu_active->history[cpu_active->subHistory].second--;
                        Iactive=NULL;
                        break;
		    */
                }


            }

        }
        if (Oactive != NULL){
            Oactive->history[Oactive->subHistory].second--;
	    if (Oactive->history[Oactive->subHistory].second ==0){
		Oactive->subHistory++;
		Oactive->OutputBurstCount++;
		cout<<"end of output burst at timer: "<<timer<<endl;
	    }
        }
        //clock ticking
        timer++;
    }
	//timer++;
}

