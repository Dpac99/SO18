#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "../lib/commandlinereader.h"

#define MAXLINHA 1024

void sendMsg(int wfd, char* new_path){
    int commandSize, bufferSize;
    char buff[MAXLINHA + 1], msg[2*MAXLINHA];
    
    while(1){

        sprintf(msg, "%s ", new_path);
        fgets(buff, MAXLINHA, stdin);
        strcat(msg, buff);
        
        commandSize = strlen(msg);
        bufferSize = strlen(buff);
        
        write(wfd, msg, commandSize+1); 
        memset(buff, 0, bufferSize);
        
        int read_ds = open(new_path, O_RDONLY);
        if(!strcmp("",new_path) || read_ds < 0){
            perror("Error creating pipe");
            exit(EXIT_FAILURE);
        }

        bufferSize = read(read_ds, buff, MAXLINHA+1);
        
        printf("%s\n", buff);
        memset(buff, 0, bufferSize);
        memset(msg, 0,commandSize);

    }
}


int main(int argc, char** argv){
    if(argc ==2 ){
        int write_ds = open(argv[1], O_WRONLY);
        if(write_ds < 0){
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }
        char path[100], *new_path;
        strcpy(path, "clientXXXXXX");

        new_path = mktemp(path);
        
        mkfifo(new_path, 0666);

        sendMsg(write_ds, new_path);
    }
    else{
        perror("Wrong arguments");
        exit(EXIT_FAILURE);
    }

    return 0;

}