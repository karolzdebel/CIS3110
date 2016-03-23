#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define _MEM_SIZE 128
#define _SIZE_LINE 128
#define _ERR0_MSG "Error: invalid number of arguments. Use following syntax: \"./holes [filename]\"\n"
#define _ERR1_MSG "Error: couldn't open file\n"

typedef struct Process{
	char id;
	int size;
}Process;