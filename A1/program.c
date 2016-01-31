
#include "shell.h"
#define DEBUG

int main(int argv,char **argc){
	static char **args = NULL;
	char input[MAX_INPUT];
	char *command = NULL;
	pid_t childpid = 1;
	int status = 0;

	while (1 && childpid != 0){
		printf("$ ");
		fgets(input, MAX_INPUT, stdin);
		if (strcmp(input,"\n") == 0){
			break;
		}

		/*Get command and arguments*/
		command = getCommand(input);
		args = getArgs(input);

		if (strcmp(command,"exit") == 0){
			#ifdef DEBUG
			printf("Command recognized as exit\n");
			#endif
			free(command);
			exit(1);
		}
		else{
			fork();
			waitpid(childpid,&status,0);
			if (status == -1){
				printf("Invalid command\n");
			}
		}
	}

	/*Execute command in child process*/
	if (childpid == 0){
		printf("Im here!!");
		exit(execvp(command,args));
	}
	return 0;
}