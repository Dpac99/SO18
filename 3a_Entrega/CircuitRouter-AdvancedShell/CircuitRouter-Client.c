#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "../lib/commandlinereader.h"

#define MAXLINHA 1024

void sendMsg(int wfd, int rfd){
    int commandSize=0;
    char buff[MAXLINHA + 1], msg[2*MAXLINHA];
    

    while(1){

        sprintf(msg, "%d", getpid());
        fgets(buff, MAXLINHA, stdin);
        strcat(msg, buff);
        write(wfd, buff, commandSize+1); 
        memset(buff, 0, commandSize+1);
        read(rfd, buff, MAXLINHA+1);
        printf("%s\n", buff);
        memset(buff, 0, commandSize+1);
        memset(msg, 0, 2*MAXLINHA);

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
        size_t size = strlen(path)*sizeof(char);

        new_path = mktemp(path);
        write(write_ds, new_path, size);
        mkfifo(new_path, 0666);

        int read_ds = open(new_path, O_RDONLY);
        if(!strcmp("",new_path) || read_ds < 0){
            perror("Error creating pipe");
            exit(EXIT_FAILURE);
        }
        
        sendMsg(write_ds, read_ds);
    }
    else{
        perror("Wrong arguments");
        exit(EXIT_FAILURE);
    }

    return 0;

}