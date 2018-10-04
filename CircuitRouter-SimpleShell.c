#include <stdio.h>
#include "lib/commandlinereader.h"

int main(int argc, char** argv){
    
    int max = -1;
    if (argc >1){
        max = argv[1];
    }

    int n = 3;
    char** vec;
    vec = (char**)malloc(n);

    char* buffer;

    int res = readLineArguments(vec, n, buffer, 100);

    if(!strcmp(vec[0], "run")){
        int pid = fork();
        if (pid == 0){
            execl("/CircuitRouter-SeqSolver", "CircuitRouter-SeqSolver", vec[1]);
            printf("ERROR\n");
            exit(-1);
        }
        
    }
}