#include "simcpu.h"

static void printThread(void *print);
static void printBurst(void *print);
static void printProcess(void *print);

static bool isEmpty(char *line){
	for (int i=0;i<strlen(line);i++){
		if (line[i] != ' ' && line[i] != '\t'){
			return false;
		}
	}
	return true;
}

static char *getCopy(char *line){
	char *copy = malloc(sizeof(char)
		*(strlen(line)+1));
	strcpy(copy,line);

	return copy;
}

static void initBurst(Burst *burst){
	burst->burstNum = 0;
	burst->cpuTm = 0;
	burst->IOTm = 0;
}

static void setBurst(Burst *burst,int cpuTm, int IOTm
	,int burstNum){

	burst->burstNum = burstNum;
	burst->cpuTm = cpuTm;
	burst->IOTm = IOTm;
}

static void initThread(Thread *thread){
	thread->threadNum =0;
	thread->burstCount=0;
	thread->arrivalTm=0;
	thread->burst = NULL;
}

static void setThread(Thread *thread, int threadNum
	, int arrivalTm, int burstCount){

	thread->threadNum = threadNum;
	thread->burstCount = burstCount;
	thread->arrivalTm = arrivalTm;
}

static void initProcess(Process *process){
	process->processNum = 0;
	process->threadCount = 0;
	process->thread = NULL;
}

static void setProcess(Process *process, int processNum
	, int threadCount){
	process->threadCount = threadCount;
	process->processNum = processNum;
}

static void initSimulation(Simulation *sim){
	sim->processCount = 0;
	sim->sameSwitchTm = 0;
	sim->diffSwitchTm = 0;
	sim->process = NULL;
}

static void setSimulation(Simulation *sim,int processCount
	, int sameSwitchTm, int diffSwitchTm){

	sim->processCount = processCount;
	sim->sameSwitchTm = sameSwitchTm;
	sim->diffSwitchTm = diffSwitchTm;
}

static void freeElement(void *elem){
	free(elem);
}

static char *getLine(){
	char *line=NULL, curChar=' ';
	int charNum=0;

	while (curChar != '\n' && curChar != EOF){
		curChar = fgetc(stdin);
		if (feof(stdin) || curChar == '\n'){ break; }

		/*Allocate memory for line*/
		if (!line){ line = malloc(sizeof(char)*2); }
		else{
			line = realloc(line,sizeof(char)
				*(strlen(line)+2));
		}

		line[charNum] = curChar;
		line[charNum+1] = '\0';
		charNum++;
	}

	return line;
}

static Simulation *getSimulation(char *line){
	char *token=NULL,*lineCopy;
	int processCount,sameSwitchTm,diffSwitchTm;
	Simulation *simulation=malloc(sizeof(Simulation));

	initSimulation(simulation);
	lineCopy = getCopy(line);

	/*Get process count*/
	token = strtok(lineCopy," ");
	if (!token){
		fprintf(stderr,"Error: invalid format\n");
		exit(1);
	}
	processCount = strtol(token,NULL,10);

	/*Get time to switch to thread in same process*/
	token = strtok(NULL," ");
	if (!token){
		fprintf(stderr,"Error: invalid format\n");
		exit(1);
	}
	sameSwitchTm = strtol(token,NULL,10);

	/*Get time to switch to thread in different process*/
	token = strtok(NULL," ");
	if (!token){
		fprintf(stderr,"Error: invalid format\n");
		exit(1);
	}
	diffSwitchTm = strtol(token,NULL,10);

	/*Store simulator properties*/
	setSimulation(simulation,processCount
		,sameSwitchTm,diffSwitchTm);
	
	free(lineCopy);
	return simulation;
}

static Process *getProcess(char *line){
	char *token=NULL,*lineCopy=NULL;
	int processNum,threadCount;
	Process *process=malloc(sizeof(Process));

	initProcess(process);
	lineCopy = getCopy(line);

	/*Get process number*/
	token = strtok(lineCopy," ");
	if (!token){
		fprintf(stderr,"Error: invalid format\n");
		exit(1);
	}
	processNum = strtol(token,NULL,10);

	/*Get number of threads in process*/
	token = strtok(NULL," ");
	if (!token){
		fprintf(stderr,"Error: invalid format\n");
		exit(1);
	}
	threadCount = strtol(token,NULL,10);

	/*Store process properties*/
	setProcess(process,processNum,threadCount);
	
	free(lineCopy);
	return process;
}

static Thread *getThread(char *line){
	char *token=NULL,*lineCopy;
	int threadNum, arrivalTm, burstCount;
	Thread *thread=malloc(sizeof(Thread));

	initThread(thread);
	lineCopy = getCopy(line);

	/*Get thread number*/
	token = strtok(lineCopy," ");
	if (!token){
		fprintf(stderr,"Error: invalid format\n");
		exit(1);
	}
	threadNum = strtol(token,NULL,10);

	/*Arrival time of thread*/
	token = strtok(NULL," ");
	if (!token){
		fprintf(stderr,"Error: invalid format\n");
		exit(1);
	}
	arrivalTm = strtol(token,NULL,10);

	/*Get the number of CPU bursts*/
	token = strtok(NULL," ");
	if (!token){
		fprintf(stderr,"Error: invalid format\n");
		exit(1);
	}
	burstCount = strtol(token,NULL,10);

	/*Store thread properties*/
	setThread(thread,threadNum,arrivalTm,burstCount);
	
	free(lineCopy);
	return thread;
}

static Burst *getBurst(char *line){
	char *token=NULL,*lineCopy;
	int burstNum=0,cpuTm=0,IOTm=0;
	Burst *burst=malloc(sizeof(Burst));

	initBurst(burst);
	lineCopy = getCopy(line);

	/*Get burst number*/
	token = strtok(lineCopy," ");
	if (!token){
		fprintf(stderr,"Error: invalid format\n");
		exit(1);
	}
	burstNum = strtol(token,NULL,10);

	/*CPU time of burst*/
	token = strtok(NULL," ");
	if (!token){
		fprintf(stderr,"Error: invalid format\n");
		exit(1);
	}
	cpuTm = strtol(token,NULL,10);

	/*IO time of burst*/
	token = strtok(NULL," ");
	/*Check if IO time is present*/
	if (token && !isEmpty(lineCopy)){
		IOTm = strtol(token,NULL,10);		
	}

	/*Store thread properties*/
	setBurst(burst,cpuTm,IOTm,burstNum);
	
	free(lineCopy);
	return burst;
}

static Simulation *fillSimProps(){
	char *line=NULL;
	int burstNum=0,threadNum=0,processNum=0;
	bool finished = false;
	Process *curProcess=NULL;
	Thread *curThread=NULL;
	Burst *curBurst=NULL;
	List *processList=NULL,*threadList=NULL;
	List *burstList=NULL;
	State state=simulateprops;
	Simulation *sim;

	/*Get all the data from the file and store it
	inside data structures contained inside a 
	Simulation structure*/
	do{
		line = getLine();
		if (!line){ break; }

		/*Finished storing all thread properties*/	
		if (state == burstprops && burstNum == 0){
			if (threadNum > 0){				
				state = threadprops;
				threadNum--;
			}
			else if (processNum > 0){
				state = processprops;
				processNum--;
			}
		}
		/*Finished storing all thread properties*/	
		else if (state == threadprops && threadNum == 0){
			if (processNum > 0){				
				state = processprops;
				processNum--;
			}
		}
		else if (state == processprops){
			if (processNum == 0){
				finished = true;
			}
		}

		switch(state){

			case simulateprops:
				sim = getSimulation(line);
				state = processprops;
				processNum = sim->processCount;

				if (sim->processCount > 0){
					/*Create process list*/
					createList(&processList);
				}
				createList(&(sim->process));

				break;

			case processprops:

				curProcess = getProcess(line);
				threadNum = curProcess->threadCount;

				if (curProcess->threadCount > 0){
					/*Create thread list*/
					if (!curProcess->thread){
						createList(&(curProcess->thread));
					}
				}

				/*Add process to list*/
				addToList(sim->process,curProcess);

				if (threadNum > 0){
					state = threadprops;
				}
				break;

			case threadprops:

				curThread = getThread(line);
				burstNum = curThread->burstCount;

				if (curThread->burstCount > 0){
					/*Create burst list*/
					if (!curThread->burst){
						createList(&(curThread->burst));
					}
				}

				/*Add thread to list*/
				addToList(curProcess->thread,curThread);

				/*Get next bursts properties*/
				if (burstNum > 0){
					state = burstprops;
				}

				break;

			case burstprops:

				/*Get next burst*/
				curBurst = getBurst(line);

				/*Add burst to list*/
				addToList(curThread->burst,curBurst);

				burstNum--;
				if (burstNum == 0){
					state = threadprops;	
					threadNum--;
				}

				break;
		}

		free(line);

	}while (!finished);

	return sim;
}

static void printProcess(void *print){
	Process *proc = (Process*)print;

	printf("processNum:%d\tthreadCount:%d\n"
		,proc->processNum,proc->threadCount);
}

static void printThread(void *print){
	Thread *thread = (Thread*)print;
	
	printf("threadNum:%d\tburstCount:%d\tarrivalTm:%d\n"
		,thread->threadNum,thread->burstCount,thread->arrivalTm);
}

static void printBurst(void *print){
	Burst *burst = (Burst*)print;

	printf("burstNum:%d\tcpuTm:%d\tIOTm:%d\n"
		,burst->burstNum,burst->cpuTm,burst->IOTm);
}

static void printSimulation(Simulation *sim){
	Process *process;
	Thread *thread;
	Node *procNext;
	Node *threadNext;
	Node *burstNext;

	printf("-----Simulation Properties-----\n");
	printf("processCount:%d\tsameSwitchTm:%d\tdiffSwitchTm:%d\n"
		,sim->processCount,sim->sameSwitchTm,sim->diffSwitchTm);
	printf("--------------------------------\n");

	printf("-----------Process------------\n");

	/*Traverse processes and print each one*/
	procNext = getHead(sim->process);
	if (!procNext){ return; }

	do{
		printNode(*procNext,printProcess);

		/*Traverse threads and print each one*/
		process = getData(procNext);

		if (process->thread){
			threadNext = getHead(process->thread);

			do{
				printNode(*threadNext,printThread);

				/*Traverse bursts and print each one*/
				thread = getData(threadNext);
				if (thread->burst){
					burstNext = getHead(thread->burst);	
					do{
						printNode(*burstNext,printBurst);
						burstNext = getNext(burstNext);

					}while(burstNext);
				}

				threadNext = threadNext->next;

			}while(threadNext);

		}

		procNext = getNext(procNext);

	}while(procNext);

	printf("----------------------------------\n");
}

int main(int argc, char **argv){
	bool detInfo=false,verbMode=false;
	int robQuantum;
	Simulation *simulation=NULL;

	for (int i=0;i<argc;i++){
		if (!strcmp(argv[i],"-d")){
			detInfo = true;
		}
		else if (!strcmp(argv[i],"-v")){
			verbMode = true;
		}
		else if (!strcmp(argv[i],"-r")){
			if (argv[i+1]){
				robQuantum = strtol(argv[i+1],NULL,10);
			}
			else{
				fprintf(stderr,"Error: round robin enabled but time quantum not specified.\n");
				return(EXIT_FAILURE);
			}
		}
	}

	simulation = fillSimProps();
	printSimulation(simulation);

	return 0;
}