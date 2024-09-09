#include <deque>
#include <string>
#include "process.h"

#define MAX_TIME    500
#define IN_USE        5
#define HOW_OFTEN    25
#define QUEUE_SIZE   15
#define VECTOR_SIZE  15

class cpuScheduler {
    public:
        cpuScheduler();
        ~cpuScheduler();
        void loadReadyQueue();
        void runProcesses();
        bool newProcess(int, string, string);
        bool isActive();
        int getCPUTimer();
        deque<process*> getEntryQueue();
        deque<process*> getReadyQueue();
        deque<process*> getInputQueue();
        deque<process*> getOutputQueue();
        deque<process*> getCompletedQueue();
        string toString();
        string lastCyclePrintout();
        
    private:
        int CPUTimer = 0, idleTimer = 0;
        bool hadPrintedIdle = false;
        process *active = nullptr, *iActive = nullptr, *oActive = nullptr;
        
        deque<process*> entry, ready, input, output, completed;
        
        string dequeFormatter(deque<process*>, process*, string);
        int totalProcesses();
};