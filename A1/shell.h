
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>    

#define MAX_INPUT 1000
#define __EXIT 0
#define __NOARG 1
#define __ARG 2

char *getCommand(char *buffer);
char **getArgs(char *buffer);