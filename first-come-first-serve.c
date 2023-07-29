/* 
    Author: Yash Joshi
    Description: implements First Come First Serve scheduling algorithm
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "global.h"

//global variables
//array which holds contents of procesess file
Process processes[100];

//counter which holds number of processes in processes file
int numOfProcesses = 0;
//counter which keeps track of number of finsihed processes
int completedProcesses = 0;

//array that stores PCBs of active processes
pcb_t *processQueue;
//counter which keeps track process queue start position index - is always 0
int qStart = 0;
//counter which keeps track process queue end position index - is a dynamic value
int qEnd = 0;

//counter which keeps trac of current clock time
int currentTime = 0;

//function to check command line arguments for input file
//see global.h for detailed description
char *checkArgs(int argc, char *argv[]) {
    //if number of command line arguemnts is greater than 1 then check if less than 3, if yes then return arguemnt if not exit program
    //if no command line arguments provided return processes.txt
    if (argc > 1) {
        if (argc < 3) {
            return argv[1];
        } else {
            fprintf(stderr, "ERROR WRONG NUMBER OF ARGUMENTS\n");
            exit(1);
        }
    } else {
        return "processes.txt";
    }    
}

//function to create results file
//see global.h for detailed description
FILE *createOutFile() {
    //open oufile results-1.txt for writing, if it doesnt exist it will be created
    FILE *outFile = fopen("results-1.txt", "w");
    
    //error handling for opening file, if error then exit program with status 1 and write error to cmd line
    if (outFile == NULL) {
        fprintf(stderr, "ERROR CREATING RESULT FILE\n");
        exit(1);
    }

    //return file descriptor
    return outFile;
}

//function to open input file
//see global.h for detailed description
FILE *openProcessFile(char *inFileName) {
    //open infile for reading
    FILE *inFile = fopen(inFileName, "r");
    
    //error handling for opening file, if error then exit program with status 1 and write error to cmd line
    if (inFile == NULL) {
        fprintf(stderr, "ERROR READING FILE\n");
        exit(1);
    }
    
    //return file descriptor
    return inFile;
}

//function to transfer processes from file and store in array
//see global.h for detailed description
void readStorePrintProcesses(FILE *inFile) {
    //temporary buffer for reading from file and writing to process array
    char readBuff[200];
    //reads first line of file to temp buffer
    fgets(readBuff, 200, inFile);

    //loop to continue reading each line/process until end of file is reached
    while (!feof(inFile)) {
        Process* p = processes + numOfProcesses;
        sscanf(readBuff, "%s %d %d %d", p->process_name, &p->entryTime, &p->serviceTime, &p->deadlineTime);
        fgets(readBuff, 200, inFile);
        numOfProcesses++;
    }

    //print number of processes to cmd line
    printf("Number of processes: %d\n\n", numOfProcesses);

    //print processes contained in processes array to cmd line
    for (int i = 0; i < numOfProcesses; i++) {
        printf("%s %d %d %d\n", processes[i].process_name, processes[i].entryTime, processes[i].serviceTime, processes[i].deadlineTime);
    }
    printf("\n");    
}

//function to create PCB for processes that has entered the system
//see global.h for detailed description
pcb_t createPCB(int i) {
    //create empty PCB
    pcb_t newProcess;
    
    //transfer process infomration to PCB
    strcpy(newProcess.process_name, processes[i].process_name);
    newProcess.entryTime = processes[i].entryTime;
    newProcess.remainingTime = processes[i].serviceTime;
    newProcess.serviceTime = processes[i].serviceTime;
    newProcess.waitTime = 0;
    newProcess.deadlineTime = processes[i].deadlineTime;
    newProcess.state = READY;

    //return new PCB
    return newProcess;
}

//function to remove a process from the queue once it has finished execution
//see global.h for detailed description
void removeProcessFromQueue() {
    //remove PCB from array by moving each array position down one index, thus overwriting first position and removing PCB of finished process
    for (int i = 0; i < qEnd; i++) {
        processQueue[i] = processQueue[i + 1];
    } 
}

//function to calculate processes results for results file
//see global.h for detailed description
void calculateResults() {
    //set process state to exit
    processQueue[qStart].state = EXIT;
    
    //calculate process turn around time
    processQueue[qStart].turnaroundTime = currentTime - processQueue[qStart].entryTime;
    
    //calculate process wait time
    processQueue[qStart].waitTime = processQueue[qStart].turnaroundTime - processQueue[qStart].serviceTime;
    
    //calculate if process met deadline
    if (processQueue[qStart].turnaroundTime <= processQueue[qStart].deadlineTime) {
        processQueue[qStart].deadlineMet = 1;
    } else {
        processQueue[qStart].deadlineMet = 0;
    }
}

//function to write process results to results file
//see global.h for detailed description
void printResults(FILE *outFile) {
    fprintf(outFile,"%s %d %d %d\n", processQueue[qStart].process_name, processQueue[qStart].waitTime, processQueue[qStart].turnaroundTime, processQueue[qStart].deadlineMet);
}

//function that implements first come first serve scheduling algorithm
//see global.h for detailed description
void firstComeFirstServe(FILE *outFile) {
    //print to cmd line that scheudling is starting
    printf("STARTING FIRST COME FIRST SERVE SCHEDULING ALGORITHM\n");
    printf("----------------------------------------------------\n");
    sleep(1);

    //create array to hold PCBs for amount of processes read, does not create PCBs for indvidual processes 
    processQueue = malloc(numOfProcesses * sizeof(pcb_t));
    
    //run the algorithm while the number of completed processes is less than the number counted from the processes file
    while (completedProcesses < numOfProcesses) {
        //iterate through process list, if process entry time is equal to current clock time create pcb for process and insert into running queue
        for (int i = 0; i < numOfProcesses; i++) {
            if (processes[i].entryTime == currentTime) {
                //create PCB for process and insert into process queue array
                processQueue[qEnd] = createPCB(i);

                //print to cmd line that process has entered the system
                printf("Time %i: %s has entered the system.\n", currentTime, processQueue[qEnd].process_name);
                sleep(1);
                
                //increment end of queue since new process has been added, so next process to be added is added to the end of the queue
                qEnd++;
            }
        }

        //check if active process remaining time is equal to 0 if so process has finished executing
        if (processQueue[qStart].remainingTime == 0) {
            //calculate process results for results output file
            calculateResults();

            //write calculated results to results output file
            printResults(outFile);    

            //print to cmd line that process has finished execution at current clock time
            printf("Time %i: %s has finished execution.\n", currentTime, processQueue[qStart].process_name);
            sleep(1);

            //remove finished process PCB from PCB array, ensures process information is only held while process is active in the system
            removeProcessFromQueue();

            //decrement queue end since process has been removed
            qEnd--;
            //increment number of processes that have been completed
            completedProcesses++;
        }

        //check if process is running or needs to start running, if it needs to start set state to running and print to cmd line
        if (processQueue[qStart].remainingTime == processQueue[qStart].serviceTime && qStart != qEnd) {
            //set process state to running and print to cmd line it is running at the current clock time
            processQueue[qStart].state == RUNNING;
            printf("Time %i: %s is in the running state.\n", currentTime, processQueue[qStart].process_name);
            sleep(1);
        }

        //run process
        processQueue[qStart].remainingTime--;            
        
        //increment clock time
        currentTime++;
    }
    
    //remove process PCB array
    free(processQueue); 
}

int main(int argc, char *argv[]) {
    //check for command line arguments
    char *inFileName = checkArgs(argc, argv);
    
    //open file containing processes
    FILE *inFile = openProcessFile(inFileName);
    
    //read each processes into a process struct and store in array
    readStorePrintProcesses(inFile);

    //close processes file
    fclose(inFile);

    //create results file
    FILE *outFile = createOutFile();
  
    //start FCFS
    firstComeFirstServe(outFile);

    //close results file
    fclose(outFile);
    
    return 0;
}
