#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 3) return 1;
    
    Process ps[100];
    int n = 0, time = 0, done = 0;
    int total_wait = 0, total_turn = 0;
    int quantum = atoi(argv[2]);
    if (quantum <= 0) quantum = 2;
    
    char* line = strtok(argv[1], "\n");
    while (line && n < 100) {
        if (line[0] != '#') {
            if (sscanf(line, "%4[^,],%d,%d,%d", 
                      ps[n].pid, &ps[n].arrival, 
                      &ps[n].burst, &ps[n].priority) == 4) {
                ps[n].remaining = ps[n].burst;
                n++;
            }
        }
        line = strtok(NULL, "\n");
    }
    if (n == 0) return 1;
    
    int queue[100], front = 0, rear = 0;
    int in_queue[100] = {0};
    int completion[100];
    int gantt[200][3], gantt_count = 0;
    char logs[400][100];
    int log_count = 0;
    
    printf("{\n");
    printf("  \"algorithm\": \"Round Robin (Quantum=%d)\",\n", quantum);
    printf("  \"ganttChart\": [\n");
    
    while (done < n) {
        for (int i = 0; i < n; i++) {
            if (ps[i].arrival <= time && !in_queue[i] && ps[i].remaining > 0) {
                queue[rear++] = i;
                in_queue[i] = 1;
            }
        }
        
        if (front < rear) {
            int idx = queue[front++];
            int exec = (ps[idx].remaining < quantum) ? ps[idx].remaining : quantum;
            
            sprintf(logs[log_count++], "Time %d: [%s] Running (Quantum)", time, ps[idx].pid);
            
            if (gantt_count > 0 && gantt[gantt_count-1][0] == idx && 
                gantt[gantt_count-1][2] == time) {
                gantt[gantt_count-1][2] = time + exec;
            } else {
                gantt[gantt_count][0] = idx;
                gantt[gantt_count][1] = time;
                gantt[gantt_count][2] = time + exec;
                gantt_count++;
            }
            
            time += exec;
            ps[idx].remaining -= exec;
            
            for (int i = 0; i < n; i++) {
                if (ps[i].arrival <= time && !in_queue[i] && ps[i].remaining > 0) {
                    queue[rear++] = i;
                    in_queue[i] = 1;
                }
            }
            
            if (ps[idx].remaining > 0) {
                queue[rear++] = idx;
                sprintf(logs[log_count++], "Time %d: [%s] Quantum finished, re-queued", 
                        time, ps[idx].pid);
            } else {
                completion[idx] = time;
                done++;
                sprintf(logs[log_count++], "Time %d: [%s] Completed", time, ps[idx].pid);
            }
        } else {
            time++;
        }
    }
    
    for (int i = 0; i < gantt_count; i++) {
        printf("    {\"pid\": \"%s\", \"start\": %d, \"end\": %d}", 
               ps[gantt[i][0]].pid, gantt[i][1], gantt[i][2]);
        if (i < gantt_count - 1) printf(",");
        printf("\n");
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