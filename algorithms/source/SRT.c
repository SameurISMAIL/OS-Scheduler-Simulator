#include "../../process.h"
#include <limits.h>

// Fonctions utilitaires pour lire et afficher les processus
int lire_processus(const char* nom_fichier, Process** processus_ptr) {
    FILE* f = fopen(nom_fichier, "r");
    if (f == NULL) {
        perror("Erreur d'ouverture du fichier de configuration");
        return -1;
    }

    char ligne[100];
    int count = 0;
    while (fgets(ligne, sizeof(ligne), f) != NULL) {
        if (ligne[0] != '#' && strlen(ligne) > 1) {
            count++;
        }
    }

    *processus_ptr = (Process*)malloc(count * sizeof(Process));
    if (*processus_ptr == NULL) {
        perror("Erreur d'allocation mémoire");
        fclose(f);
        return -1;
    }
    
    rewind(f);

    int i = 0;
    while (fgets(ligne, sizeof(ligne), f) != NULL) {
        if (ligne[0] != '#' && strlen(ligne) > 1) {
            sscanf(ligne, "%s %d %d %d",
                   (*processus_ptr)[i].pid,
                   &(*processus_ptr)[i].t_arv,
                   &(*processus_ptr)[i].t_exec,
                   &(*processus_ptr)[i].priorite);
            (*processus_ptr)[i].rnt = (*processus_ptr)[i].t_exec;
            (*processus_ptr)[i].a_deja_commence = 0;
            i++;
        }
    }

    fclose(f);
    return count;
}

void afficher_tableau(Process processus[], int n) {
    printf("| PID | Arrival Time | Burst Time | Priority |\n");
    printf("|-----|--------------|------------|----------|\n");
    for (int i = 0; i < n; i++) {
        printf("| %-3s | %-12d | %-10d | %-8d |\n", processus[i].pid, processus[i].t_arv, processus[i].t_exec, processus[i].priorite);
    }
    printf("\n");
}

// Programme principal pour l'algorithme SRT
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <fichier_params>\n", argv[0]);
        return 1;
    }

    Process* processus;
    int n = lire_processus(argv[1], &processus);
    if (n <= 0) return 1;

    printf("\nWelcome to Shortest Remaining Time Scheduling Algorithm\n");
    afficher_tableau(processus, n);

    int temps_actuel = 0;
    int processus_termines = 0;
    char gantt_pids[1024] = "|";
    int gantt_times[100];
    int gantt_count = 0;
    
    char dernier_pid[5] = ""; // Pour savoir quand le processus change

    gantt_times[gantt_count++] = 0;

    while(processus_termines < n) {
        int idx_choisi = -1;
        int rnt_min = INT_MAX;

        // A chaque unité de temps, on cherche le processus dispo avec le plus petit rnt
        for (int i=0; i<n; i++) {
            if (processus[i].t_arv <= temps_actuel && processus[i].rnt > 0) {
                if (processus[i].rnt < rnt_min) {
                    rnt_min = processus[i].rnt;
                    idx_choisi = i;
                }
            }
        }

        if (idx_choisi != -1) {
            // Si le processus qui s'exécute change, on le note dans le diagramme
            if (strcmp(dernier_pid, processus[idx_choisi].pid) != 0) {
                 strcat(gantt_pids, " ");
                 strcat(gantt_pids, processus[idx_choisi].pid);
                 strcat(gantt_pids, " |");
                 // Sauf pour le premier, on note le temps de changement
                 if(temps_actuel > 0) gantt_times[gantt_count++] = temps_actuel;
                 strcpy(dernier_pid, processus[idx_choisi].pid);
            }

            processus[idx_choisi].rnt--;

            if (processus[idx_choisi].rnt == 0) {
                processus_termines++;
            }
        }
        temps_actuel++;
    }
    gantt_times[gantt_count] = temps_actuel-1; // Temps final

    printf("The Gantt chart is:\n");
    printf("%s\n", gantt_pids);
    for(int i=0; i<= gantt_count; i++) {
        printf("%-4d", gantt_times[i]);
    }
    printf("\n");

    free(processus);
    return 0;
}
