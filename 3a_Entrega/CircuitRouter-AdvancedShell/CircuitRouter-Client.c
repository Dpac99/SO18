#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "../lib/commandlinereader.h"

#define MAXLINHA 1024

void sendMsg(int fd){
    int numArgs;
    char buff[MAXLINHA + 1];
    char *args[3];

    while(1){

        numArgs = readLineArguments(args,4,buff, MAXLINHA+1);

        if (numArgs!=2) {
            perror("Error");
        }
        else{
            int commandSize = strlen(args[0]) + strlen(args[1]);
            char buffer[commandSize+1];
            strcpy(buffer, args[0]);
            strcat(buffer, " ");
            strcat(buffer, args[1]);
            write(fd, buffer, commandSize+1); 
            memset(buff, 0, commandSize+1);
        }
    }
}


int main(int argc, char** argv){
    if(argc ==2 ){
        int write_ds = open(argv[1], O_WRONLY);
        if(write_ds < 0){
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }
        char path[100];
        strcpy(path, "clientXXXXXX");
        size_t size = strlen(path)*sizeof(char);
        char* new_path = mktemp(path);
        write(write_ds, new_path, size);
        sendMsg(write_ds);
        /*int read_ds = open(new_path, O_RDONLY);
        if(!strcmp("",new_path) || read_ds < 0){
            perror("Error creating pipe\n");
            exit(EXIT_FAILURE);
        }*/
    }
    else{
        perror("Wrong arguments");
        exit(EXIT_FAILURE);
    }

    return 0;

}