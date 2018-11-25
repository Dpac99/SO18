
/*
// Projeto SO - exercise 1, version 1
// Sistemas Operativos, DEI/IST/ULisboa 2018-19
*/

#include "../lib/commandlinereader.h"
#include "../lib/vector.h"
#include "CircuitRouter-AdvancedShell.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>


#define COMMAND_EXIT "exit"
#define COMMAND_RUN "run"

#define MAXARGS 3
#define BUFFER_SIZE 100

void waitForChild(vector_t *children) {
    while (1) {
        child_t *child = malloc(sizeof(child_t));
        if (child == NULL) {
            perror("Error allocating memory");
            exit(EXIT_FAILURE);
        }
        child->pid = wait(&(child->status));
        if (child->pid < 0) {
            if (errno == EINTR) {
                /* Este codigo de erro significa que chegou signal que interrompeu a espera
                   pela terminacao de filho; logo voltamos a esperar */
                free(child);
                continue;
            } else {
                perror("Unexpected error while waiting for child.");
                exit (EXIT_FAILURE);
            }
        }
        vector_pushBack(children, child);
        return;
    }
}

void printChildren(vector_t *children) {
    for (int i = 0; i < vector_getSize(children); ++i) {
        child_t *child = vector_at(children, i);
        int status = child->status;
        pid_t pid = child->pid;
        if (pid != -1) {
            const char* ret = "NOK";
            if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                ret = "OK";
            }
            printf("CHILD EXITED: (PID=%d; return %s)\n", pid, ret);
        }
    }
    puts("END.");
}

void sendError(char *client){
    char msg[] = "Command not supported.\n";
    size_t size = 24;
    int fds;

    fds = open(client, O_WRONLY);
    write(fds, msg, size);
    close(fds);
}

/*====================================================== 
 * reads line from pipe and parses.
 * @params: modifiable array containing args, size of array, file descriptor and client info vector
 * @returns: number of arguments read.
 * This function functions similarly to the one defined for reading a line from
 * stdin. All code made for stdin input (from SimpleShell) will still work
 * ======================================================
 */

int readFdsArguments(char **argVector, int vectorSize, int fds, char* client){
  int numTokens = 0;
  char *s = " \r\n\t", buffer[BUFFER_SIZE+1];

  int i;

  char *token;

  if (argVector == NULL ||  vectorSize <= 0 )
     return 0;

  if (read(fds, buffer, BUFFER_SIZE) <1 ) {
    return -1;
  }


  /* get the first token */
  token = strtok(buffer, s);

  /* walk through other tokens */
  while( numTokens < vectorSize-1 && token != NULL ) {
    argVector[numTokens] = token;
    numTokens ++;

    token = strtok(NULL, s);
  }

  for (i = numTokens; i<vectorSize; i++) {  
    argVector[i] = NULL;
  }

  strcpy(client,argVector[0]);

  for(i=0; i<vectorSize-1; i++){
    argVector[i] = argVector[i+1];
  }

  return numTokens - 1;
}

int main (int argc, char** argv) {

    char *args[MAXARGS + 1], *path;
    char buffer[BUFFER_SIZE];
    int MAXCHILDREN = -1;
    vector_t *children;
    char currClient[BUFFER_SIZE];
    int runningChildren = 0;
    int pipe_ds;
    fd_set mask;
    bool_t commandline;

    if(argv[1] != NULL){
        MAXCHILDREN = atoi(argv[1]);
    }

    children = vector_alloc(MAXCHILDREN);


    printf("Welcome to CircuitRouter-SimpleShell\n\n");

    path = (char*)malloc( (strlen(argv[0]) + 6) *sizeof(char) );
    strcpy(path, argv[0]);
    strcat(path, ".pipe");
    mkfifo(path, 0666);
    pipe_ds = open(path, O_RDONLY | O_NONBLOCK);
    FD_ZERO(&mask);
    FD_SET(pipe_ds, &mask);
    FD_SET(STDIN_FILENO, &mask);

    while (1) {
        int numArgs, n;
        fd_set tempmask = mask;

        n = select(pipe_ds+1,&tempmask,0,0,NULL);

        if( FD_ISSET(pipe_ds, &tempmask)){
            numArgs = readFdsArguments(args, MAXARGS+1, pipe_ds, currClient);
            commandline=FALSE;
            if(numArgs == -1){
                sendError(currClient);
                continue;
            }
        }

         if( FD_ISSET(STDIN_FILENO, &tempmask) ){
            numArgs = readLineArguments(args, MAXARGS+1, buffer, BUFFER_SIZE);
            commandline=TRUE;
        }

        /* EOF (end of file) do stdin ou comando "sair" */
        if (numArgs < 0 || (numArgs > 0 && (strcmp(args[0], COMMAND_EXIT) == 0) && commandline)) {
            printf("CircuitRouter-SimpleShell will exit.\n--\n");

            /* Espera pela terminacao de cada filho */
            while (runningChildren > 0) {
                waitForChild(children);
                runningChildren --;
            }

            printChildren(children);
            printf("--\nCircuitRouter-SimpleShell ended.\n");
            break;
        }

        else if (numArgs > 0 && strcmp(args[0], COMMAND_RUN) == 0){
            int pid;
            if (numArgs < 2) {
                if(commandline){
                    printf("%s: invalid syntax. Try again.\n", COMMAND_RUN);
                }
                else{
                    sendError(currClient);
                }
                continue;
            }
            if (MAXCHILDREN != -1 && runningChildren >= MAXCHILDREN) {
                waitForChild(children);
                runningChildren--;
            }

            pid = fork();
            if (pid < 0) {
                perror("Failed to create new process.");
                exit(EXIT_FAILURE);
            }

            if (pid > 0) {
                runningChildren++;
                printf("%s: background child started with PID %d.\n\n", COMMAND_RUN, pid);
                continue;
            } else {
                char seqsolver[] = "../CircuitRouter-SeqSolver/CircuitRouter-SeqSolver";
                char *newArgs[4] = {seqsolver, args[1], currClient, NULL};

                execv(seqsolver, newArgs);
                perror("Error while executing child process"); // Nao deveria chegar aqui
                exit(EXIT_FAILURE);
            }
        }

        else if (numArgs == 0){
            /* Nenhum argumento; ignora e volta a pedir */
            continue;
        }
        else{
            if(commandline){
                printf("Unknown command. Try again.\n");
            }
            else{
                sendError(currClient);
            }
        }

    }

    for (int i = 0; i < vector_getSize(children); i++) {
        free(vector_at(children, i));
    }
    vector_free(children);

    return EXIT_SUCCESS;
}