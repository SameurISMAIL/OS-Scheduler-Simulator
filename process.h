#ifndef PROCESS_H
#define PROCESS_H

typedef struct {
    char pid[5];
    int arrival;
    int burst;
    int priority;
    int remaining;
} Process;

#endif