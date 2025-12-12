#include "process.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) return 1;
    
    Process ps[100];
    int n = 0, time = 0, done = 0;
    int total_wait = 0, total_turn = 0;
    int last = -1;
    
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
    
    int completion[100];
    int gantt[200][3], gantt_count = 0;
    char logs[400][100];
    int log_count = 0;
    
    printf("{\n");
    printf("  \"algorithm\": \"Shortest Remaining Time (SRT)\",\n");
    printf("  \"ganttChart\": [\n");
    
    while (done < n) {
        int idx = -1, min_rem = 999999;
        
        for (int i = 0; i < n; i++) {
            if (ps[i].arrival <= time && ps[i].remaining > 0 && ps[i].remaining < min_rem) {
                min_rem = ps[i].remaining;
                idx = i;
            }
        }
        
        if (idx != -1) {
            if (idx != last) {
                if (last != -1 && ps[last].remaining > 0) {
                    sprintf(logs[log_count++], "Time %d: [%s] Preempted", time, ps[last].pid);
                    if (gantt_count > 0) gantt[gantt_count-1][2] = time;
                }
                sprintf(logs[log_count++], "Time %d: [%s] Started/Resumed (SRT)", 
                        time, ps[idx].pid);
                gantt[gantt_count][0] = idx;
                gantt[gantt_count][1] = time;
                gantt[gantt_count][2] = time + 1;
                gantt_count++;
                last = idx;
            } else if (gantt_count > 0) {
                gantt[gantt_count-1][2] = time + 1;
            }
            
            ps[idx].remaining--;
            
            if (ps[idx].remaining == 0) {
                completion[idx] = time + 1;
                done++;
                sprintf(logs[log_count++], "Time %d: [%s] Completed", time + 1, ps[idx].pid);
                last = -1;
            }
        } else if (last != -1 && gantt_count > 0) {
            gantt[gantt_count-1][2] = time;
            last = -1;
        }
        
        time++;
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