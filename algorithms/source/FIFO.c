#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cmp(const void* x, const void* y) { 
    return ((Process*)x)->arrival - ((Process*)y)->arrival; 
}

int main(int argc, char* argv[]) {
    if (argc < 2) return 1;
    
    Process ps[100];
    int n = 0, time = 0, total_wait = 0, total_turn = 0;
    char logs[200][100];
    int log_count = 0;
    
    char* line = strtok(argv[1], "\n");
    while (line && n < 100) {
        if (line[0] != '#') {
            if (sscanf(line, "%4[^,],%d,%d,%d", 
                      ps[n].pid, &ps[n].arrival, 
                      &ps[n].burst, &ps[n].priority) == 4) {
                n++;
            }
        }
        line = strtok(NULL, "\n");
    }
    if (n == 0) return 1;
    
    qsort(ps, n, sizeof(Process), cmp);
    
    printf("{\n");
    printf("  \"algorithm\": \"First In, First Out (FIFO)\",\n");
    printf("  \"ganttChart\": [\n");
    
    for (int i = 0; i < n; i++) {
        if (time < ps[i].arrival) time = ps[i].arrival;
        
        sprintf(logs[log_count++], "Time %d: [%s] Started execution", time, ps[i].pid);
        
        int wait = time - ps[i].arrival;
        int turn = wait + ps[i].burst;
        
        printf("    {\"pid\": \"%s\", \"start\": %d, \"end\": %d}", 
               ps[i].pid, time, time + ps[i].burst);
        if (i < n - 1) printf(",");
        printf("\n");
        
        total_wait += wait;
        total_turn += turn;
        time += ps[i].burst;
        
        sprintf(logs[log_count++], "Time %d: [%s] Finished execution", time, ps[i].pid);
    }
    
    printf("  ],\n");
    printf("  \"logs\": [\n");
    for (int i = 0; i < log_count; i++) {
        printf("    \"%s\"", logs[i]);
        if (i < log_count - 1) printf(",");
        printf("\n");
    }
    printf("  ],\n");
    
    printf("  \"metrics\": {\n");
    printf("    \"avgWaitingTime\": %.2f,\n", (float)total_wait / n);
    printf("    \"avgTurnaroundTime\": %.2f,\n", (float)total_turn / n);
    printf("    \"throughput\": %.3f\n", (float)n / time);
    printf("  }\n");
    printf("}\n");
    
    return 0;
}