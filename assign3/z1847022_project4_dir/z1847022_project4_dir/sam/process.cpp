/*
    Milena Zlaticanin - Z1847022     
    CSCI 480 
    Assignment 3
    Due: 3/29/22
*/

#include "process.h"
#include <sstream>
using namespace std;

process::process() {
    this->processName = "";
    this->processID = 0;
    this->arrivalTime = 0;
    this->processWait = 0;
    this->CPUTimer = 0;
    this->ITimer = 0;
    this->OTimer = 0;
    this->CPUTotal = 0;
    this->ITotal = 0;
    this->OTotal = 0;
    this->CPUCount = 0;
    this->ICount = 0;
    this->OCount = 0;
    
    this->history_sub = 0;
    this->history = vector<pair<string, int>>();
}

string process::toString() {
    stringstream ss;
    ss << this->processID << "    ";
    return ss.str();
}

string process::getProcessType() { return this->history[this->history_sub].first; }
int process::getProcessTotal() { return this->history[this->history_sub].second; }