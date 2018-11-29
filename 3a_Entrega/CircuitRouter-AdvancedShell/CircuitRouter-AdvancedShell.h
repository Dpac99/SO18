#ifndef CIRCUITROUTER_SHELL_H
#define CIRCUITROUTER_SHELL_H

#include "../lib/vector.h"
#include <sys/types.h>
#include "../lib/timer.h"

typedef struct {
    pid_t pid;
    int status;
    TIMER_T start, finish;
} child_t;

void waitForChild();
void printChildren();

#endif /* CIRCUITROUTER_SHELL_H */
