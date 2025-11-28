#include "../../process.h"

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
    printf("| PID | Temps d'arrive | Temps d'execution | Priorite |\n");
    printf("|-----|----------------|-------------------|----------|\n");
    for (int i = 0; i < n; i++) {
        printf("| %-3s | %-14d | %-17d | %-8d |\n", processus[i].pid, processus[i].t_arv, processus[i].t_exec, processus[i].priorite);
    }
    printf("\n");
}

// Programme principal pour l'algorithme de Priorité (non préemptif)
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <fichier_params>\n", argv[0]);
        return 1;
    }

    Process* processus;
    int n = lire_processus(argv[1], &processus);
    if (n <= 0) return 1;

    printf("\nWelcome to Priority Scheduling Algorithm\n");
    afficher_tableau(processus, n);

    int temps_actuel = 0;
    int processus_termines = 0;
    int termines[n];
    for(int i = 0; i < n; i++) termines[i] = 0;

    char gantt_chart[1024] = "|";

    while (processus_termines < n) {
        int idx_choisi = -1;
        // La plus basse valeur numérique signifie la plus haute priorité
        int priorite_min = 99999; 

        // Chercher le processus arrivé avec la plus haute priorité
        for (int i = 0; i < n; i++) {
            if (processus[i].t_arv <= temps_actuel && termines[i] == 0) {
                if (processus[i].priorite < priorite_min) {
                    priorite_min = processus[i].priorite;
                    idx_choisi = i;
                }
            }
        }

        if (idx_choisi != -1) {
            char buffer[50];
            // Format de sortie comme dans la documentation : |0-P1-4-P3-6-...|
            if (temps_actuel == 0) {
                 sprintf(buffer, "0-%s-", processus[idx_choisi].pid);
            } else {
                 sprintf(buffer, "%s-", processus[idx_choisi].pid);
            }
           
            temps_actuel += processus[idx_choisi].t_exec;
            sprintf(buffer + strlen(buffer), "%d-", temps_actuel);
            strcat(gantt_chart, buffer);

            termines[idx_choisi] = 1;
            processus_termines++;
        } else {
            temps_actuel++;
        }
    }
    
    gantt_chart[strlen(gantt_chart)-1] = '|'; // Remplacer le dernier '-' par '|'
    
    printf("Gantt chart:\n%s\n", gantt_chart);

    free(processus);
    return 0;
}
