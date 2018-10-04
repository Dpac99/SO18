#include <stdio.h>
#include "process.h"

process* process_alloc(int pid){
    process* proc = (process*)malloc(sizeof(struct _process));
    proc->pid = pid;
    return proc;
}

void process_free(process* proc){
    free(proc);
}

void process_print(process* proc){
    printf("CHILD EXITED (PID= %d; return %s)\n", proc->pid, proc->status? "OK": "NOK");
}