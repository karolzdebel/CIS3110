#ifndef SIMCPU_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "list.h"

#define _FCFS 0				//First come first serve
#define _RR 1				//Round robin

typedef enum SimType{
	fcfs,
	rr
} SimType;
/*State during parsing*/
typedef enum ParseState{
	simulateprops,			//Parsing simulation props
	processprops,			//Parsing process props
	threadprops,			//Parsing thread props
	burstprops				//Parsing burst props
} ParseState;

/*State during simulation*/
typedef enum SimState{
	new,					//New process
	ready,					//Ready process
	running,				//Running process
	blocked,				//Blocked process
	terminated				//Terminated process
} SimState;

/*Event structure*/
typedef struct StateEvent {
	int time;				//Time an event occurred
	int threadNum;			//Thread number
	int processNum;			//Process number
	SimState from;			//State switched from
	SimState to;			//State switched to
} StateEvent;

/*Burst Simulation Structure*/
typedef struct SimEvent {
	int arrivalTm;			//Arrival time of event
	int cpuTm;				//Number of CPU time
	int IOTm;				//Number of IO time
	int processNum;			//Process number of event
	int sameSwitchTm;		//Switch time in same process
	int diffSwitchTm; 		//Switch time in different process
	int threadNum; 			//Thread number associated with event
} SimEvent;

/*Burst structure*/
typedef struct BurstData {
	int burstNum;			//Burst number
	int cpuTm;				//Number of CPU time
	int IOTm;				//Number of IO time
} Burst;

/*Thread simulation stats structure*/
typedef struct ThreadRes {
	int threadNum; 			//Thread number
	int arrivalTm;			//Arrival time
	int serviceTm;			//Serivce time
	int turnTm;				//Turnaround time
	int finishTm;			//Finish time
	int IOTm;				//IO Time
	int processNum; 		//Associated process num
} ThreadRes;

/*Thread data structure*/
typedef struct ThreadData {
	int threadNum;			//Thread number
	int burstCount;			//number of CPU bursts
	int arrivalTm;			//Arrival time
	List *burst; 			//Queue of bursts
} Thread;

/*Process structure*/
typedef struct ProcessData {
	int processNum;			//Process number
	int threadCount;		//Number of threads 
	List *thread;			//List of threads
} Process;

/*Simulation results*/
typedef struct SimulationRes {
	int totalTm;			//Total time
	double avgTurnTm;		//Average turnaround time
	int cpuUtil;			//CPU utilization
	List *threadRes;		//Thread results
	List *stateEvents;		//State changes that occured
} SimulationRes;

/*Simulate structure*/
typedef struct SimulationData {
	SimType type;			//Type of simulation
	int rrQuantum;			//Round robin time quantum
	int processCount;		//Number of processes
	int sameSwitchTm;		//Thread time switch in same proc
	int diffSwitchTm;		//Thread time switch in diff proc
	List *process;			//List of processes
} Simulation;

#define SIMCPU_H
#endif