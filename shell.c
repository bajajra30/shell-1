#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "shell.h"
#include "list.h" 

int main(int argc, char **argv)
{
	
	char *input = (char*)malloc(BUFFER_SIZE);
	printf("$ ");
	input = fgets(input, BUFFER_SIZE, stdin);
	myPath = newList();
	myHistory = newList();

	while((strncmp(input, "exit", 4)) != 0){
		while(!end(input))
		{
			char*temp = (char*)malloc(sizeof(char)*BUFFER_SIZE);
			temp = fgets(temp, BUFFER_SIZE, stdin);
			input = realloc(input, strlen(input)+BUFFER_SIZE+1);
			input = strcat(input, temp);
			printf("%s\n", input);
		}

		/* remove trailing newline from input */
        	if(input[strlen(input)-1] == '\n')
        	{
                	input[strlen(input)-1] = '\0';
        	}

		/* if !n format, replace input with equivalent from history */
		if(strncmp(input, "!", 1)==0){
			input = bang(input);
		}

		/* insert command into history */
        	char*newPath = (char*)malloc(strlen(input));
        	strcpy(newPath, input);
        	push(myHistory, (void*)newPath);

		if(strncmp(input, "path", 4) == 0){
			path(input);
		} else if(strncmp(input, "cd", 2) == 0){
			changeDirectory(input);
		} else if(strncmp(input, "history", 7) == 0){
			history(input);
		} else if(strncmp(input, "\n", 1) == 0) { 
			/* ignore blank input */ 
		} else {
			forkExec(input);
		}

		fflush(stdin);
		printf("$ ");
		input = fgets(input, BUFFER_SIZE, stdin);
	
	}
	free(input);
	free(myPath);
	free(myHistory);
	return 0;
}

/* interprets the bang (!n) and returns new command */
char* bang(char* input)
{
	char *temp =(char*)malloc(sizeof(char*)); 
	temp = &input[1];
	int index = atoi(temp);
	char*output = (char*)malloc(BUFFER_SIZE);
	int length = size(myHistory);
	output = get(myHistory, length-index);	
	return output;
}

/* prints out the history list */
void history(input)
{
	int count = 1;
	int length = size(myHistory);
	int max = 0;
	if(100 > length) max = length;
	else max = 101;
	int index = max-1;
	while(index >0 && count <= 101){
		char* temp  = (char*)get(myHistory, index);
		printf("[%d] %s\n", count, temp);
		count++;
		index--;
	}
}

/* helper method to capture all the input by finding the
	end of the line */
int end(char* input)
{
	int length = strlen(input);
	int i;
	for(i = 0; i<length;i++)
	{
		if(input[i] == '\n') return 1;
	}
	return 0;
}

/* adds or subtracts the given path based on input */
void path(char*input)
{
	/* remove trailing newline from input*/ 
        if(input[strlen(input)-1] == '\n')
        {
                input[strlen(input)-1] = '\0';
        }

        /* parse the input line */
        char **args = (char**)malloc(strlen(input));
        args[0] = strtok(input, " ");
        int i = 0;
        while(args[i] != NULL)
        {
                i++;
                args[i] = strtok(NULL, " ");
        }

	if(args[1] == NULL)
	{ 
		/* print all listed paths */
		struct node *myNode = myPath->head;
		if(myNode == NULL) return;
		while(myNode != NULL){
			char* temp = (char*)myNode->value;
			printf("%s:", temp);
			myNode = myNode->next;
		}
		printf("\n");

	} else if(args[2] == NULL){
		fputs("error: invalid path\n", stderr);
	} else if(strncmp(args[1], "+", 1)==0){ 
		/* add to list of paths */
		char *newPath = (char*)malloc(strlen(args[2]));
		strcpy(newPath, args[2]);
		if(newPath[strlen(newPath)-1] == '/'){
			newPath[strlen(newPath)-1] = '\0';
		}
		if(newPath[0] != '/'){
			char*n = (char*)malloc(strlen(newPath)+1);
			strcpy(n, "/");
			strcat(n, newPath);
			newPath = n;	
		}
		push(myPath, (void*)newPath);

	} else if(strncmp(args[1], "-", 1)==0){ 
		/* delete from list of paths*/
		delete((void*)args[2]);	

	} else
		fputs("error: invalid path\n", stderr);

}


/* deletes the given path from the given list --
	just use this as a helper method because path() 
	was becoming too long */
void delete(void*path)
{
	struct node *current = myPath->head;
        struct node *prev = myPath->head;
	
        if(prev == NULL) return;
        current = prev->next;

        /* honestly these next couple ifs are somewhat bandaids --
        I couldn't get the while loop to work stand-alone and eliminate
        everything that was necessary */
        if(current == NULL){
                if(strcmp((char*)prev->value, (char*)path) == 0){
                        myPath->head = NULL;
                }
        }
        if(strcmp((char*)prev->value, (char*) path)==0){
                myPath->head = current;
        }
        while(current != NULL){
                if(strcmp((char*)current->value, (char*)path)==0){
                        prev->next = current->next;
                        current = prev->next;
                }
                else{
                        prev = current;
                        current = current->next;
                }
        }	
}


/* implements a change of directory */
int changeDirectory(char*input)
{

        /* remove trailing newline from input */
        if(input[strlen(input)-1] == '\n')
        {
                input[strlen(input)-1] = '\0';
        }

        /* parse the input line */
        char **args = (char**)malloc(strlen(input));
        args[0] = strtok(input, " ");
        int i = 0;
        while(args[i] != NULL)
        {
                i++;
                args[i] = strtok(NULL, " ");
        }

	if(chdir(args[1]) == -1){
		printf("error: cd failed, %s\n", strerror(errno));
		rem(myHistory);
	}

	return 0;
}

/* forks and executes the program */
void forkExec(char* input)
{	
	/* remove trailing newline from input */
        if(input[strlen(input)-1] == '\n')
        {
                input[strlen(input)-1] = '\0';
        }

	struct stat st;
	char*temp = (char*)malloc(strlen(input));
	strncpy(temp, input, strlen(input));
	temp = strtok(temp, " ");
	if(stat(temp, &st) == 0)
	{
		run(input);
		return;
	}

	struct node *n = myPath->head;
	while(n != NULL){
		char *value = (char*)n->value;
		int size = strlen(value)+strlen(input)+1;
		char *name = (char*)malloc(size);
		if(name == NULL){
			perror("error: malloc failed");
			return;
		}
		strncpy(name, value, strlen(value));
		strcat(name, "/");
		strcat(name, input);
		temp = (char*)realloc(temp, strlen(name));
		temp = memset(temp, '\0', sizeof(temp));
		strcpy(temp, name);
		temp = strtok(temp, " ");
		if(stat(temp, &st)==0)
		{
			run(name);
			return;
		}
		else{
			n = n->next;
		}
	}
	
	fputs("error: No such file or command\n", stderr);
	
}


/* actually executesy the process */
void run(char* input)
{
        /* parse the input line */
        char **args = (char**)malloc(strlen(input));
        args[0] = strtok(input, " ");
        int i = 0;
        while(args[i] != NULL)
        {
                i++;
                args[i] = strtok(NULL, " ");
        }

        /* forks and executes the program */
        int pid = fork();
        if(pid == 0)
        {
                //if(execl(input, *args, NULL)== -1)
                if(execv(input, args) == -1)  
                {
                        printf("error: %s\n", strerror(errno));
                        exit(1);  
                }
        }
        else if(pid>0)
        {
                int status;
                if(waitpid(-1, &status, 0) == -1)
                {
                        fputs("error: waitpid failed\n", stderr);
                }
        }
        else
        {
                perror("error: fork failed");
        }

}


