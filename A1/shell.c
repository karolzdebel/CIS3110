
#include "shell.h"

static char *getCopy(char *buffer){
	char *copy;
	copy = malloc(sizeof(char)
		*(strlen(buffer)+1));
	strcpy(copy,buffer);
	return copy;
}

char **getArgs(char *buffer){

	char **tokens=NULL,**args=NULL,*token;
	char *input;
	int i,tokenCount=0,argNum;

	/*Copy the contents of buffer*/
	input = getCopy(buffer);

	/*Remove end of line character*/
	token = strtok(input,"\n");

	/*Get all the tokens seperated by a space*/
	token = strtok(input," ");
	if (token == NULL){
		printf("token is empty. Exiting");
		exit(1);
	}

	while (token != NULL){
		tokenCount++;

		if (tokens == NULL){
			tokens = malloc(sizeof(char*));
		}
		else{
			tokens = realloc(tokens
				,sizeof(char*)*tokenCount);
		}
		
		tokens[tokenCount-1] = malloc(sizeof(char)
			*(strlen(token)+1));
		strcpy(tokens[tokenCount-1],token);

		token = strtok(NULL," ");
	}

	/*Store all tokens that have the proper argument
	  syntax in the argument array*/
	argNum = 0;
	for (i=0;i<tokenCount;i++){
		if (tokens[i][0] == '-'){
			argNum++;

			if (args == NULL){
				args = malloc(sizeof(char*));
			}
			else{
				args = realloc(args
					,sizeof(char*)*argNum);
			}
			
			args[argNum-1] = malloc(sizeof(char)
				*(strlen(tokens[i])+1));
			strcpy(args[argNum-1],tokens[i]);
		}
	}

	/*Free token array*/
	if (tokens != NULL){
		for (i=0;i<tokenCount;i++){
			free(tokens[i]);
		}
		free(tokens);
	}

	return args;
}
char *getCommand(char *buffer){
	char *token,*input,*command;

	/*Copy the contents of buffer*/
	input = getCopy(buffer);

	/*Remove the new line*/
	token = strtok(input,"\n");

	/*Look for exit tokens*/
	if (strcmp(input,"exit") == 0){
		return "exit";
	}

	token = strtok(token," ");
	command = malloc(sizeof(char)
		*(strlen(token)+1));
	strcpy(command,token);

	free(input);
	return command;
}
