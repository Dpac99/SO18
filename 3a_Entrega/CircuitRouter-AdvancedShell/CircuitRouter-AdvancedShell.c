
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
#include "../lib/timer.h"
#include <pthread.h>


#define COMMAND_EXIT "exit"
#define COMMAND_RUN "run"

#define MAXARGS 3
#define BUFFER_SIZE 100

vector_t *children;
sig_atomic_t runningChildren = 0;


/*====================================================== 
 * waitForChild
 * ======================================================
 */
void waitForChild() {
    while (1) {
        child_t *child;
        int status, size = vector_getSize(children);
        pid_t pid = wait(&(status));
        if (pid < 0) {
            if (errno == EINTR) {
                /* Este codigo de erro significa que chegou signal que interrompeu a espera
                   pela terminacao de filho; logo voltamos a esperar */ 
                continue;
            } else {
                perror("Unexpected error while waiting for child.");
                exit (EXIT_FAILURE);
            }
        }
        TIMER_T end;
        TIMER_READ(end);
        for(int i=0; i<size; i++){
            child = vector_at(children, i);
            if(child->pid == pid){
                child->status = status;
                child->finish=end;
                return;
            }
        }
        return;
    }
} 


/*====================================================== 
 * printChildren
 * ======================================================
 */
void printChildren() {
    for (int i = 0; i < vector_getSize(children); ++i) {
        child_t *child = vector_at(children, i);
        int status = child->status;
        pid_t pid = child->pid;
        TIMER_T start = child->start, finish = child->finish;
        if (pid != -1) {
            const char* ret = "NOK";
            if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                ret = "OK";
            }
            printf("CHILD EXITED: (PID=%d; return %s; %f s)\n", pid, ret, TIMER_DIFF_SECONDS(start, finish));
        }
    }
    puts("END.");
}


/*====================================================== 
 * sendError
 * ======================================================
 */
void sendError(char *client){
    char msg[] = "Command not supported.\n";
    size_t size = 24;
    int fds, n;

    fds = open(client, O_WRONLY);
    if(fds < 0){
        perror("Error opening pipe\n");
        return;
    }
    write(fds, msg, size);
    n = close(fds);
    if( n< 0){
        perror("Error closing file\n");
        return;
    }
}


/*====================================================== 
 * handler
 * ======================================================
 */
static void handler(int sig){
    pid_t pid;
    int status;
    child_t* child;
    TIMER_T stop;
    TIMER_READ(stop);
    int size = vector_getSize(children);
    while( (pid = waitpid(-1, &status, WNOHANG) ) > 0 ) {
        runningChildren--;
        for(int i =0; i<size; i++){
            child = vector_at(children, i);
            if(child->pid == pid){
                child->status = status;
                child->finish = stop;
                break;
            }
        }
    }
    if(pid == -1){
        if(errno == ECHILD){
            return;
        }
        else{
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
    }
    return;
}


/*====================================================== 
 * readFdsArguments
 * ======================================================
 */
int readFdsArguments(char **argVector, int vectorSize, int fds, char* client){
    int numTokens = 0, n;
    char *s = " \r\n\t", buffer[BUFFER_SIZE+1];
    memset(buffer, '\0', BUFFER_SIZE+1);
    fd_set mask;
    FD_SET(fds, &mask);
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec=0;

    int i;

    char *token;

    if (argVector == NULL ||  vectorSize <= 0 ){
        return 0;  
    }

    while( select(fds+1, &mask, NULL, NULL, &timeout)){
        if ( (n =read(fds, buffer, BUFFER_SIZE) ) < 1) {
            if(n == -1 && errno == EINTR){
                continue;
            }
            return n;
        }
        FD_SET(fds, &mask);
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
    char *currClient = (char*)malloc(BUFFER_SIZE * sizeof(char));
    if(!currClient){
        perror("Malloc error");
        exit(EXIT_FAILURE);
    }
    int pipe_ds;
    fd_set mask;
    bool_t commandline;

    struct sigaction action;
    memset (&action, '\0', sizeof(action));
    action.sa_handler = &handler;
    action.sa_flags = SA_RESTART| SA_NOCLDSTOP;
    if (sigaction(SIGCHLD, &action, NULL) < 0) {
		perror ("sigaction");
		return 1;
	}

    if(argv[1] != NULL){
        MAXCHILDREN = atoi(argv[1]);
    }

    children = vector_alloc(MAXCHILDREN);


    printf("Welcome to CircuitRouter-SimpleShell\n\n");

    path = (char*)malloc( (strlen(argv[0]) + 6) *sizeof(char) );
    if(path == NULL){
        perror("Error allocating memory\n");
        exit(EXIT_FAILURE);
    }
    strcpy(path, argv[0]);
    strcat(path, ".pipe");
    mkfifo(path, 0666);
    pipe_ds = open(path, O_RDWR);
    if (pipe_ds < 0){
        perror("Error opening pipe\n");
    }
    FD_ZERO(&mask);
    FD_SET(pipe_ds, &mask);
    FD_SET(STDIN_FILENO, &mask);
 
    while (1) {
        int numArgs;
        fd_set tempmask = mask;

        while ( select(pipe_ds+1, &tempmask, 0, 0, NULL) < 0){
            if(errno == EINTR){
                continue;
            }
            perror("select");
            exit(EXIT_FAILURE);
        }

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
                //waitForChild();
                //handler(0);
                //runningChildren --;
            }

            printChildren();
            unlink(path);
            free(path);
            free(currClient);
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
            while (MAXCHILDREN != -1 && runningChildren >= MAXCHILDREN) {
                //waitForChild();
                //handler(0);
                //runningChildren--;
            }

           pid = fork();
            if (pid < 0) {
                perror("Failed to create new process.");
                unlink(path);
                exit(EXIT_FAILURE);
            }

            if (pid > 0) {
                runningChildren++;
                child_t* child = (child_t*)malloc(sizeof(child_t));
                if(!child){
                    perror("Error allocating memory");
                    exit(EXIT_FAILURE);
                }
                child->pid = pid;
                TIMER_READ (child->start);
                vector_pushBack(children, child);
                printf("%s: background child started with PID %d.\n\n", COMMAND_RUN, pid);
                continue;
            } else {
                char seqsolver[] = "../CircuitRouter-SeqSolver/CircuitRouter-SeqSolver";
                char *output = (commandline? NULL : currClient);
                char *newArgs[4] = {seqsolver, args[1], output, NULL};

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
