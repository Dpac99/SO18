/* 
 * Process struct code
 */ 

#include <stdio.h>
#include <stdlib.h>
#include "process.h"

/* =============================================================================
 * process_alloc
 * =============================================================================
 */
process* 
process_alloc(int pid, int status){
    process* proc = (process*)malloc(sizeof(struct _process));
    proc->pid = pid;
    proc->status = status;
    return proc;
}


/* =============================================================================
 * process_free
 * =============================================================================
 */
void 
process_free(process* proc){
    free(proc);
}


/* =============================================================================
 * process_print
 * =============================================================================
 */
void 
process_print(process* proc){
    printf("CHILD EXITED (PID=%d; return %s)\n", proc->pid, proc->status? "NOK": "OK");
}
