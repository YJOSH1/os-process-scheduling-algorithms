/* 
    Author: Yash Joshi
    Description: global header file
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//struct that stores a processes state
typedef enum {
    READY, RUNNING, EXIT
} process_state_t;

//struct that represents PCB for a process
typedef struct {
    char process_name[11];

    int entryTime;
    int serviceTime;
    int remainingTime;
    int deadlineTime;

    int deadlineMet;
    int turnaroundTime;
    int waitTime;

    int timeQuantum;

    process_state_t state;
} pcb_t;

//intermediary struct which holds process information between reading processes file and running FCFS algorithm
typedef struct Process {
    char process_name[11];

    int entryTime;
    int serviceTime;
    int remainingTime;

    int deadlineTime;
    
} Process;

//function which checks command line arguments for input file, if no input file is give processes.txt is set as input file
//takes two input parameters, argc and argv
//returns char pointer containing input file name
char *checkArgs(int argc, char *argv[]);

//function which creates results.txt file
//has no input parameters
//returns pointer holding file descriptor of results.txt
FILE *createOutFile();

//function which opens processes file
//takes one input parameter, a char pointer which holds the processe file name, default is processes.txt
//returns pointer holding file descriptor of input file
FILE *openProcessFile(char *inFileName);

//function which reads processes line by line from input file, creates process struct and stores that struct in processes storage array, then prints to cmd line
//takes one input parameter, a file descriptor representing the input file
//has no return value
void readStorePrintProcesses(FILE *inFile);

//function which creates a PCB for processes that has entered the system
//takes no input parameteres
//has no return value
pcb_t createPCB(int i);

//function which removes PCB for a processes that has finished execution
//has no input parameters
//has no return value
void removePCB();

//function which sets a processes state to exit and calculates its turnaround time, wait time and if it has met its deadline time
//has no input parameters
//has no return value
void calculateResults();

//function which writes process results to results file
//takes on input parameter, the file descriptor of the results file
//has no return value
void printResults(FILE *outFile);

//function which removes a processes PCB that has finsihed execution from the process queue
//has no input parameters
//has no return value
void removeProcessFromQueue();

//function which moves a processes PCB that has been preempted to the end of the process queue
//has no input parameters
//has no return value
void moveProcessToEndOfQueue();

//function which finds the process in the process queue with the shortest deadline time
//has no input parameters
//has a return value of int which represents the index of the process with the shortest deadline in the process queu
int findShortestDeadline();

//function that implements first come first serve scheduling algorithm
//takes one input parameterm, the file descriptor of the results file
//has no return value
void firstComeFirstServe(FILE *outFile);

//function that implements round robin scheduling algorithm
//takes one input parameterm, the file descriptor of the results file
//has no return value
void roundRobin(FILE *outFile);

//function that implements deadline-based scheduling algorithm
//takes one input parameterm, the file descriptor of the results file
//has no return value
void deadline(FILE *outFile);
