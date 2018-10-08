#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "../lib/commandlinereader.h"
#include "../lib/list.h"
#include "../lib/process.h"

#define BUFFSIZE 256
#define MAXARGS 4
#define eq(A,B) (!strcmp(A,B))

void child_process(char** args){
    execv("../solver/CircuitRouter-SeqSolver", args);
    printf("ERROR: error with child process");
    exit(-1);
}

int main(int argc, char** argv){
    
    list_t* dead_process = list_alloc(NULL);

    int max = -1;
    int current = 0;
    char buff [BUFFSIZE];
    char* args[MAXARGS];
    int status;
    int finished;
    list_iter_t iter;
    int n_args;
    process* proc;

    if(argc > 1){
        if (argc > 2){ printf("ERROR: Too many arguments\n"); exit(0);}
        max = strtol(argv[1], NULL, 10);
        if(!max){max=-1;}
        //printf("max = %d\n",max); 
    }

    while(1){
        n_args = readLineArguments(args, MAXARGS, buff, BUFFSIZE);
        if(n_args > 2){
            printf("ERROR: Too many arguments");
            continue;
        }
        else if( eq(args[0], "run")){

            if(current == max){
                finished = wait(&status);
                process* new = process_alloc(finished,status);
                list_insert(dead_process, new);
                current--;
            }

            args[0] = "CircuitRouter-SeqSolver";
            int pid = fork();
            
            if( pid < 0){
                printf("Unable to fork");
                continue;
            }

            else if (pid == 0){
                child_process(args); 
            }

            else {
                current++ ;
            }
        }
        else if (eq(args[0], "exit")){
            while(current > 0){
                finished = wait(&status);
                process* new = process_alloc(finished,status);
                list_insert(dead_process, new);
                current--;
            }
            if(!list_isEmpty(dead_process)){
                list_iter_reset(&iter, dead_process);
                while(list_iter_hasNext(&iter, dead_process)){
                    list_iter_next(&iter, dead_process);
                    proc = ((process*)iter->dataPtr);
                    process_print(proc);
                    process_free(proc);
                }
                list_clear(dead_process);
            }
            list_free(dead_process);
            exit(0);
        }
        else{
            printf("ERROR: Unknown command\n");
            continue;
        }
    }
}
