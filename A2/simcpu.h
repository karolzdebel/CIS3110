#ifndef SIMCPU_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "list.h"

/*Burst structure*/
typedef struct Burst {
	int burstNum;			//Burst number
	int cpuTm;				//Number of CPU time
	int IOTm;				//Number of IO time
} Burst;

/*Thread structure*/
typedef struct Thread {
	int threadNum;			//Thread number
	int burstCount;			//number of CPU bursts
	int arrivalTm;			//arrival time
	List *burst; 			//List of bursts
} Thread;

/*Process structure*/
typedef struct Process {
	int processNum;			//Process number
	int threadCount;		//Number of threads 
	List *thread;			//List of threads
} Process;

/*Simulate structure*/
typedef struct Simulation {
	int processCount;
	int sameSwitchTm;
	int diffSwitchTm;
	List *process;
} Simulation;

typedef enum State{
	simulateprops,
	processprops,
	threadprops,
	burstprops
}State;

#define SIMCPU_H
#endif