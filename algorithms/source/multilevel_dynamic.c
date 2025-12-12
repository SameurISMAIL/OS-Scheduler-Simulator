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
                if (ps[n].priority > 4) ps[n].priority = 4;
                if (ps[n].priority < 0) ps[n].priority = 0;
                n++;
            }
        }
        line = strtok(NULL, "\n");
    }
    if (n == 0) return 1;
    
    int completion[100];
    int gantt[200][3], gantt_count = 0;
    int last = -1, current_pri = -1;
    char logs[400][100];
    int log_count = 0;
    
    printf("{\n");
    printf("  \"algorithm\": \"Dynamic Multilevel Queue\",\n");
    printf("  \"ganttChart\": [\n");
    
    while (done < n) {
        int max_pri = -1;
        for (int i = 0; i < n; i++) {
            if (ps[i].arrival <= time && ps[i].remaining > 0 && ps[i].priority > max_pri) {
                max_pri = ps[i].priority;
            }
        }
        
        if (max_pri != -1) {
            if (max_pri != current_pri) {
                current_pri = max_pri;
                last = -1;
            }
            
            int idx = -1;
            for (int i = 1; i <= n; i++) {
                int cand = (last + i) % n;
                if (ps[cand].arrival <= time && ps[cand].remaining > 0 && 
                    ps[cand].priority == max_pri) {
                    idx = cand;
                    break;
                }
            }
            
            if (idx != -1) {
                int exec = (ps[idx].remaining < quantum) ? ps[idx].remaining : quantum;
                
                sprintf(logs[log_count++], "Time %d: [%s] Running (Dynamic P=%d)", 
                        time, ps[idx].pid, max_pri);
                
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
                last = idx;
                
                if (ps[idx].remaining == 0) {
                    completion[idx] = time;
                    done++;
                    sprintf(logs[log_count++], "Time %d: [%s] Completed", time, ps[idx].pid);
                } else {
                    if (ps[idx].priority > 0) {
                        ps[idx].priority--;
                        sprintf(logs[log_count++], "Time %d: [%s] Priority decreased to %d", 
                                time, ps[idx].pid, ps[idx].priority);
                    } else {
                        sprintf(logs[log_count++], "Time %d: [%s] Priority at minimum", 
                                time, ps[idx].pid);
                    }
                }
            } else {
                time++;
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