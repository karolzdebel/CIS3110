
#include "shell.h"
#define DEBUG

int main(int argv,char **argc){
	static char **args = NULL;
	char input[MAX_INPUT];
	char *command = NULL;
	pid_t childpid;
	int status = 0;

	/*Execute command in child process*/

	while (1){
		printf("> ");
		fgets(input, MAX_INPUT, stdin);

		/*Empty input*/
		if (isEmpty(input) == 0){
			continue;
		}

		/*Free any memory from the previous input*/
		if (command != NULL && args != NULL){
			free(command);
			freeArgs(&args);
		}
		
		/*Get command and arguments*/
		command = getCommand(input);
		args = getArgs(input);
		if (strcmp(command,"exit") == 0){
			printf("Logout\n\n[Process Completed]\n");
			free(command);
			freeArgs(&args);
			exit(1);
		}
		else{
			childpid = fork();
			waitpid(childpid,&status,0);
			/*Let the child process execute the command*/
			if (childpid == 0){
				execvp(command,args);

				/*Free memory if error returned*/
				free(command);
				freeArgs(&args);
				exit(errno);
			}
			if (WEXITSTATUS(status)){
				printf("Abnormal return code: %d\n",WEXITSTATUS(status));
			}
		}
	}
	return 0;
}