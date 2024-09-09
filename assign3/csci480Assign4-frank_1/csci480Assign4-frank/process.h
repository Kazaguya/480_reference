#ifndef H_PROCESS
#define H_PROCESS


#include <string>
#include <vector>
#include <utility>


class Process{

    public:

    std::string processName;
    int processID;
    int arrivalTime;
    std::vector< std::pair <char,int> > history;
    int subHistory=0;
    int CPUTimer=0;
    int InputBurstTimer=0;
    int OutputBurstTimer =0;
    int CPUTotal=0;
    int InputBurstTotal=0;
    int OutputBurstTotal =0;
    int CPUCount=0;
    int InputBurstCount=0;
    int OutputBurstCount=0;


};
#endif