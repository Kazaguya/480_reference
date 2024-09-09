/*
    Milena Zlaticanin - Z1847022     
    CSCI 480 
    Assignment 4
    Due: 3/29/22
*/

#include "cpuScheduler.h"
#include <vector>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <iomanip>
#include <cstdlib>

using namespace std;

cpuScheduler::cpuScheduler(){}
cpuScheduler::~cpuScheduler(){}

void cpuScheduler::runProcesses() {
    this->loadReadyQueue();
    
    if (!this->active && !firstPass) {
        if (this->ready.size() > 0) {
            process* p = this->ready[0];
            
            p->CPUTimer = 0;
            p->ITimer = 0;
            p->OTimer = 0;
            
            this->active = p;
            this->ready.pop_front();
        } else {
            if(!this->hadPrintedIdle) {
                cout << "\nAt time " << this->CPUTimer << ", Active is 0, so we have idle time for a while\n\n";
                this->hadPrintedIdle = true;
            }
            this->idleTimer++;
        }
    }
    
    
    
    if (this->active) {
        this->active->CPUTimer++;
        this->active->CPUTotal++;
        
        if (this->active->CPUTimer >= this->active->getProcessTotal()) {
            this->active->history_sub++;
            this->active->CPUCount++;
            this->active->CPUTimer = 0;
            
            if (this->active->getProcessType() == "I") {
                this->input.push_back(this->active);
            } else if (this->active->getProcessType() == "O") {
                this->output.push_back(this->active);
            } else if (this->active->getProcessType() == "N") {
                this->active->endTime = this->CPUTimer;
                this->completed.push_back(this->active);
                
                string cpuburst = "bursts ";
                if (this->active->CPUTotal == 1) cpuburst = "burst ";
                string iburst = "bursts ";
                if (this->active->ITotal == 1) iburst = "burst ";
                string oburst = "bursts ";
                if (this->active->OTotal == 1) oburst = "burst ";
                
                cout << "\nProcess " << this->active->processID << " has ended." << endl;
                cout << "Name:  " << this->active->processName << endl;
                cout << "Started at time " << this->active->startTime << " and ended at time " << this->active->endTime << endl;
                cout << "Total CPU time =  " << this->active->CPUTotal << " in " << this->active->CPUCount << " CPU " << cpuburst << endl;
                cout << "Total Input time =  " << this->active->ITotal << " in " << this->active->ICount << " Input " << iburst << endl;
                cout << "Total Output time =  " << this->active->OTotal << " in " << this->active->OCount << " Output " << oburst << endl;
                cout << "Time spent in waiting:  " << this->active->processWait << endl << endl;
            }
            
            this->active = nullptr;
            this->hadPrintedIdle = false;
        }
    }
    
    if (!this->iActive) {
        if (this->input.size() > 0) {
            process* p = this->input[0];
            
            p->CPUTimer = 0;
            p->ITimer = 0;
            p->OTimer = 0;
            
            this->iActive = p;
            this->input.pop_front();
        }
    }
    
    if (this->iActive) {
        this->iActive->ITimer++;
        this->iActive->ITotal++;
        
        if (this->iActive->ITimer >= this->iActive->getProcessTotal()) {
            this->iActive->history_sub++;
            this->iActive->ICount++;
            this->iActive->ITimer = 0;
            
            this->ready.push_back(this->iActive);
            this->iActive = nullptr;
        }
    }
    
    if (!this->oActive) {
        if (this->output.size() > 0) {
            process* p = this->output[0];
            
            p->CPUTimer = 0;
            p->ITimer = 0;
            p->OTimer = 0;
            
            this->oActive = p;
            this->output.pop_front();
        }
    }
    
    if (this->oActive) {
        this->oActive->OTimer++;
        this->oActive->OTotal++;
        
        if (this->oActive->OTimer >= this->oActive->getProcessTotal()) {
            this->oActive->history_sub++;
            this->oActive->OCount++;
            this->oActive->OTimer = 0;
            
            this->ready.push_back(this->oActive);
            this->oActive = nullptr;
        }
    }
    
    for (process *p: this->ready) {
         if (!firstPass && !hadPrintedIdle) {p->processWait++;
        //cout << "POOP\n\n\n\n\n\n\n";}
         }
    }
    
    for (process *p: this->input) {
        if (!firstPass) {p->processWait++;
        //cout << "SHIT\n\n\n\n\n\n\n\n";}
        }
    }
    
    for (process *p: this->output) {
        if (!firstPass) {p->processWait++;
        //cout << "FUCK\n\n\n\n\n\n\n\n";
        }
    }
    
    if (this->CPUTimer%HOW_OFTEN == 0) {
        cout << this->toString() << endl;
    }
    
    if (this->isActive()) {
        this->CPUTimer++;
    }
    firstPass = false;
}
    
bool cpuScheduler::newProcess(int id, string p1, string p2) {
    
    process* newP = new process();
    
    stringstream ss(p1);
    ss >> newP->processName;
    if(newP->processName == "STOPHERE") return false;
    newP->processID = id;
    string s = "";
    ss >> s;
    
    newP->arrivalTime = stoi(s);
    
    //cout << p1 << endl << p2 << endl;
    stringstream ss2(p2);
    do{
        string type, temp;
        int size;
        ss2 >> type;
        ss2 >> temp;
        
        if(type != "" and temp != ""){
            size = stoi(temp);
            newP->history.push_back(make_pair(type, size));
        }
        
    }while(ss2.rdbuf()->in_avail() != 0);
    
    this->entry.push_back(newP);
    return true;
}

void cpuScheduler::loadReadyQueue() {
    if (!this->active && this->ready.size() <= IN_USE) {
        for (long unsigned int i = 0; i < this->entry.size(); i++) {
            if (this->entry[i]->arrivalTime <= this->CPUTimer && this-> totalProcesses() <= IN_USE) {
                cout << "\nProcess " << this->entry[i]->processID << " moved from the Entry Queue into the Ready Queue at time " << this-> CPUTimer << endl << endl;
                     
                this->entry[i]->startTime = this->CPUTimer;
                this->ready.push_back(this->entry[i]);
                this->entry.erase(this->entry.begin()+i);
                i-=2;
            }
        }
    }
}

bool cpuScheduler::isActive() {
    bool listFilled = this->entry.size() > 0 || this->ready.size() > 0 ||
                      this->output.size() > 0 || this->input.size() > 0;
    bool processorsActive = this->active || this->iActive || this->oActive;
    
    return this->CPUTimer < MAX_TIME && (listFilled || processorsActive);
}

string cpuScheduler::toString() {
    stringstream ss;
    ss << "\nStatus at time " << this->CPUTimer << "\n";
    ss << "Active is ";
    if (this->active) ss << this->active->processID;
    else ss << "0";
    ss << endl;
    ss << "IActive is ";
    if (this->iActive) ss << this->iActive->processID;
    else ss << "0";
    ss << endl;
    ss << "OActive is ";
    if (this->oActive) ss << this->oActive->processID;
    else ss << "0";
    ss << endl;
    ss << "Contents of the Entry Queue\n";
    if (this->entry.size() > 0) {
        for (process* p: this->entry) {
            ss << p->toString();
        }
    } else {
        ss << "(Empty)";
    }
    
    //ss << endl;
    
    ss << this->dequeFormatter(this->ready, this->active, "Ready");
    ss << this->dequeFormatter(this->input, this->iActive, "Input");
    ss << this->dequeFormatter(this->output, this->oActive, "Output");
    ss << endl;
    return ss.str();
}

string cpuScheduler::lastCyclePrintout() {
    stringstream ss;
    double average = 0;
    for (process *p: this->completed) {
        average+=p->idleTime;
    }
    average /= this->completed.size();
    ss << "\nThe run has ended\n"
       << "The final value of the timer was: " << this->CPUTimer << endl
       << "The amount of time spent idle was: " << this->idleTimer << endl
       << "Number of terminated processes = " << this->completed.size() << endl
       << "Average waiting time for all terminated processes = " << average << endl;
    ss << "Active is ";
    if (this->active) ss << this->active->processID;
    else ss << "0";
    ss << endl;
    ss << "IActive is ";
    if (this->iActive) ss << this->iActive->processID;
    else ss << "0";
    ss << endl;
    ss << "OActive is ";
    if (this->oActive) ss << this->oActive->processID;
    else ss << "0";
    ss << endl;
    ss << "Contents of the Entry Queue\n";
    if (this->entry.size() > 0) {
        for (process* p: this->entry) {
            ss << p->toString();
        }
    } else {
        ss << "(Empty)";
    }
    
    //ss << endl;
    
    ss << this->dequeFormatter(this->ready,  this->active,  "Ready");
    ss << this->dequeFormatter(this->input,  this->iActive, "Input");
    ss << this->dequeFormatter(this->output, this->oActive, "Output");
    ss << endl;
       
    return ss.str();
}

string cpuScheduler::dequeFormatter(deque<process*> d, process* active, string title) {
    stringstream ss;
    
    ss << "\nContents of the " << title << " Queue:\n";
    if (d.size() > 0) {
        for (process* p: d) {
            ss << p->toString();
        }
    } else {
        ss << "(Empty)";
    }
    
    return ss.str();
}

int cpuScheduler::getCPUTimer() { return this->CPUTimer; }

deque<process*> cpuScheduler::getReadyQueue()     { return this->ready;     }
deque<process*> cpuScheduler::getInputQueue()     { return this->input;     }
deque<process*> cpuScheduler::getOutputQueue()    { return this->output;    }
deque<process*> cpuScheduler::getEntryQueue()     { return this->entry;     }
deque<process*> cpuScheduler::getCompletedQueue() { return this->completed; }

int cpuScheduler::totalProcesses() {
    int total = 0;
    if (this->active) total++;
    if (this->iActive) total++;
    if (this->oActive) total++;
    total += this->ready.size() + this->input.size() + this->output.size();
    
    return total;
}