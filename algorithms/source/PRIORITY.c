#include "process.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) return 1;
    
    Process ps[100];
    int n = 0, time = 0, done = 0;
    int total_wait = 0, total_turn = 0;
    int completed[100] = {0};
    int completion[100];
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
    
    printf("{\n");
    printf("  \"algorithm\": \"Non-Preemptive Priority\",\n");
    printf("  \"ganttChart\": [\n");
    
    while (done < n) {
        int idx = -1, max_pri = -1;
        
        for (int i = 0; i < n; i++) {
            if (!completed[i] && ps[i].arrival <= time && ps[i].priority > max_pri) {
                max_pri = ps[i].priority;
                idx = i;
            }
        }
        
        if (idx != -1) {
            if (time < ps[idx].arrival) time = ps[idx].arrival;
            
            sprintf(logs[log_count++], "Time %d: [%s] Started (Highest Priority)", 
                    time, ps[idx].pid);
            
            printf("    {\"pid\": \"%s\", \"start\": %d, \"end\": %d}", 
                   ps[idx].pid, time, time + ps[idx].burst);
            if (done < n - 1) printf(",");
            printf("\n");
            
            time += ps[idx].burst;
            completion[idx] = time;
            completed[idx] = 1;
            done++;
            
            sprintf(logs[log_count++], "Time %d: [%s] Completed", time, ps[idx].pid);
        } else {
            time++;
        }
    }
    
    for (int i = 0; i < n; i++) {
        int turn = completion[i] - ps[i].arrival;
        total_turn += turn;
        total_wait += turn - ps[i].burst;
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