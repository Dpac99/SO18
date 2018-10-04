#include <stdio.h>
#include "lib/commandlinereader.h"
#include "lib/list.h"

#define BUFFSIZE 1024
#define MAXARGS 3
#define eq(A,B) (!strcmp(A,B))



int main(int argc, char** argv){
    
    list_t* active_process = list_alloc(NULL);
    list_t* dead_process = list_alloc(NULL);

    int max = -1;
    int current = 0;
    char* buffer;
    char * args[MAXARGS];


    if(argc > 1){
        if (argc > 2){ printf("ERROR: Too many arguments"); exit(0);}
        max = argv[1];
    }

    while(1){
        int n_args = readLineArguments(args, MAXARGS, buffer, BUFFSIZE);
        if(n_args > 2){
            printf("ERROR: Too many arguments");
        }
        else if( eq(args[0], "run")){
            while(list_getSize(active_process) >= MAXARGS){
                continue;
            }

            args[0] = "CircuitRouter-SeqSolver";
            int pid = fork();
            
            if( pid < 0){
                printf("Unable to fork");
                continue;
            }

            else if (pid == 0){
                child_process(args); //TODO: define child process procedure
                printf("ERROR: Process encoutered error\n");
                exit(-1);
            }

            else {
                parent_process(); //TODO: define parent process procedure
            }
        }
        else {
            break;
        }
    }


}