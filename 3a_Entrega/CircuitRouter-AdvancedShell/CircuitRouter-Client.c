#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "../lib/commandlinereader.h"
#include <errno.h>
#include <signal.h>

#define MAXLINHA 1024

char *global_path;

void sigpipe_handler(int sig){
    printf("Pipe has been closed. Program will exit\n");
    unlink(global_path);
    exit(EXIT_SUCCESS);
}

void sendMsg(int wfd, char* new_path){
    int commandSize, bufferSize;
    char buff[MAXLINHA + 1], msg[2*MAXLINHA];
    fd_set mask;
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec=0;
    int diff, write_ret;

    
    while(1){

        sprintf(msg, "%s ", new_path);
        fgets(buff, MAXLINHA, stdin);
        strcat(msg, buff);
        
        commandSize = strlen(msg);
        bufferSize = strlen(buff);
        
        write_ret = write(wfd, msg, commandSize+1); 
        if(write_ret == -1 && errno != EPIPE){
            perror("write");
            exit(EXIT_FAILURE);
        }
        memset(buff, 0, bufferSize);
        
        int read_ds = open(new_path, O_RDONLY);
        if(read_ds < 0){
            perror("Error opening pipe");
            exit(EXIT_FAILURE);
        }

        FD_SET(read_ds, &mask);
        diff=0;
        bufferSize=0;
        do {
            diff= read(read_ds, buff + diff, MAXLINHA+1 - diff);
            bufferSize+=diff;
            FD_SET(read_ds, &mask);
        }
        while( select(read_ds+1, &mask, NULL, NULL, &timeout) && diff);
        
        printf("%s\n", buff);
        memset(buff, 0, bufferSize);
        memset(msg, 0,commandSize);

    }
}


int main(int argc, char** argv){
    if(argc ==2 ){
        struct sigaction sa;
        sa.sa_handler = &sigpipe_handler;
        sigaction(SIGPIPE, &sa, NULL);
        int write_ds = open(argv[1], O_WRONLY);
        if(write_ds < 0){
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }
        char path[100], *new_path;
        strcpy(path, "/tmp/clientXXXXXX");

        new_path = mktemp(path);
        if( !strcmp("", new_path)){
            perror("Error generating filename");
            exit(EXIT_FAILURE);
        }
         global_path = new_path;
        
        mkfifo(new_path, 0777);

        sendMsg(write_ds, new_path);
    }
    else{
        perror("Wrong arguments");
        exit(EXIT_FAILURE);
    }

    return 0;

}