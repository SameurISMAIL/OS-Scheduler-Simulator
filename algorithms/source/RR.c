#include "../../process.h"
#include <stdbool.h>

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

// Programme principal pour l'algorithme Round Robin
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <fichier_params>\n", argv[0]);
        return 1;
    }

    Process* processus;
    int n = lire_processus(argv[1], &processus);
    if (n <= 0) return 1;

    printf("\nWelcome to Round Robin Scheduling Algorithm\n");
    afficher_tableau(processus, n);

    int quantum;
    printf("Enter Time Slice OR Quantum Number: ");
    scanf("%d", &quantum);

    int temps_actuel = 0;
    int processus_termines = 0;
    int file_attente[100];
    int debut_file = 0, fin_file = 0;
    bool en_file[n];
    for(int i=0; i<n; i++) en_file[i] = false;
    
    char gantt_chart[1024] = "|";

    while(processus_termines < n) {
        // Ajouter les nouveaux processus arrivés à la file d'attente
        for(int i=0; i<n; i++) {
            if(processus[i].t_arv <= temps_actuel && !en_file[i] && processus[i].rnt > 0) {
                file_attente[fin_file++] = i;
                en_file[i] = true;
            }
        }

        if(debut_file < fin_file) { // Si la file n'est pas vide
            int idx_courant = file_attente[debut_file++];
            
            char buffer[50];
            if (temps_actuel == 0) sprintf(buffer, "0-%s-", processus[idx_courant].pid);
            else sprintf(buffer, "%s-", processus[idx_courant].pid);

            int temps_a_executer = (processus[idx_courant].rnt < quantum) ? processus[idx_courant].rnt : quantum;
            
            processus[idx_courant].rnt -= temps_a_executer;
            temps_actuel += temps_a_executer;

            sprintf(buffer + strlen(buffer), "%d-", temps_actuel);
            strcat(gantt_chart, buffer);
            
            // Ajouter les processus arrivés PENDANT l'exécution de ce quantum
            for(int i=0; i<n; i++) {
                if(processus[i].t_arv <= temps_actuel && !en_file[i] && processus[i].rnt > 0) {
                    file_attente[fin_file++] = i;
                    en_file[i] = true;
                }
            }
            
            if (processus[idx_courant].rnt > 0) {
                // S'il n'est pas terminé, le remettre à la fin de la file
                file_attente[fin_file++] = idx_courant;
            } else {
                processus_termines++;
                en_file[idx_courant] = false; // Il est terminé, il ne peut plus être en file
            }

        } else {
            temps_actuel++; // CPU inactif
        }
    }
    
    gantt_chart[strlen(gantt_chart)-1] = '|';
    gantt_chart[strlen(gantt_chart)] = '\0';

    printf("Gantt chart:\n%s\n", gantt_chart);

    free(processus);
    return 0;
}
