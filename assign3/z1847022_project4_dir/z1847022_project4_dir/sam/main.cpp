#include "cpuScheduler.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define QUEUE_SIZE  15
#define MAX_TIME   500

using namespace std;

int main(int argc, char** argv) {
    cpuScheduler cpu = cpuScheduler();
    
    ifstream infile;
    string filename = argv[1];
    
    infile.open(filename);
    
    int id = 101;
    string line1 = "", line2 = "";
    
    cout << "Simulation of CPU Scheduling\n\n";
    
    while(getline(infile, line1) && getline(infile, line2)) {
        cpu.newProcess(id, line1, line2);
        id++;
    }
    
    cpu.loadReadyQueue();
    do {
        cpu.runProcesses();
    } while(cpu.isActive());
    
    cout << cpu.lastCyclePrintout();
    return 0;
}