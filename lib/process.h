/* ===========================================
 * Process structure
 * Keeps process ID, status and exec time
 * ===========================================
 */

#ifndef __PROCESS__
#define __PROCESS__

typedef struct _process {
    int pid;
    int status;
    double runtime;
} process;

process* process_alloc(int pid);
void process_free(process* proc);
void process_print(process* proc);

#endif //__PROCESS__