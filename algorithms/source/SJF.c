#include "../../process.h"
#include <limits.h> // Pour INT_MAX

// Fonctions utilitaires pour lire et afficher les processus
int lire_processus(const char* nom_fichier, Process** processus_ptr) {
    FILE* f = fopen(nom_fichier, "r");
    if (f == NULL) {
        perror("Erreur d'ouverture du fichier de configuration");
        return -1;
    }

    char ligne[100];
    int count = 0;
    // Compter le nombre de processus pour allouer la mémoire
    while (fgets(ligne, sizeof(ligne), f) != NULL) {
        if (ligne[0] != '#' && strlen(ligne) > 1) {
            count++;
        }
    }

    // Allouer la mémoire pour le tableau de processus
    *processus_ptr = (Process*)malloc(count * sizeof(Process));
    if (*processus_ptr == NULL) {
        perror("Erreur d'allocation mémoire");
        fclose(f);
        return -1;
    }
    
    // Rembobiner le fichier pour le lire à nouveau
    rewind(f);

    int i = 0;
    while (fgets(ligne, sizeof(ligne), f) != NULL) {
        if (ligne[0] != '#' && strlen(ligne) > 1) {
            sscanf(ligne, "%s %d %d %d",
                   (*processus_ptr)[i].pid,
                   &(*processus_ptr)[i].t_arv,
                   &(*processus_ptr)[i].t_exec,
                   &(*processus_ptr)[i].priorite);
            (*processus_ptr)[i].rnt = (*processus_ptr)[i].t_exec; // Initialiser le temps restant
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

// Programme principal pour l'algorithme SJF (non préemptif)
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <fichier_params>\n", argv[0]);
        return 1;
    }

    Process* processus;
    int n = lire_processus(argv[1], &processus);
    if (n <= 0) return 1;

    printf("\nWelcome to Shortest Job First Scheduling Algorithm\n");
    afficher_tableau(processus, n);

    int temps_actuel = 0;
    int processus_termines = 0;
    // Tableau pour marquer les processus qui sont terminés
    int termines[n];
    for(int i = 0; i < n; i++) {
        termines[i] = 0; // 0 = non terminé, 1 = terminé
    }

    // Chaînes de caractères pour construire le diagramme de Gantt
    char gantt_pids[512] = "|";
    char gantt_times[512] = "0";

    // La boucle de simulation s'exécute tant que tous les processus ne sont pas terminés
    while (processus_termines < n) {
        int idx_choisi = -1;
        int t_exec_min = INT_MAX; // On cherche le temps d'exécution le plus petit possible

        // Étape 1: Chercher le prochain processus à exécuter
        // Il doit être arrivé (t_arv <= temps_actuel) et non terminé
        for (int i = 0; i < n; i++) {
            if (processus[i].t_arv <= temps_actuel && termines[i] == 0) {
                // S'il est meilleur que le meilleur trouvé jusqu'à présent, on le choisit
                if (processus[i].t_exec < t_exec_min) {
                    t_exec_min = processus[i].t_exec;
                    idx_choisi = i;
                }
            }
        }

        // Étape 2: Exécuter le processus choisi ou avancer le temps
        if (idx_choisi != -1) {
            // Un processus a été trouvé, on le simule
            
            // Mettre à jour le diagramme de Gantt
            char temp[20];
            sprintf(temp, " %s |", processus[idx_choisi].pid);
            strcat(gantt_pids, temp);

            // Le temps avance de la durée totale du processus
            temps_actuel += processus[idx_choisi].t_exec;

            sprintf(temp, "   %d", temps_actuel);
            strcat(gantt_times, temp);

            // Marquer le processus comme terminé
            termines[idx_choisi] = 1;
            processus_termines++;
        } else {
            // Aucun processus n'est prêt, le CPU est donc inactif.
            // On avance simplement le temps d'une unité pour attendre qu'un processus arrive.
            temps_actuel++;
        }
    }
    
    // Étape 3: Afficher le résultat final
    printf("The Gantt chart is:\n");
    printf("%s\n", gantt_pids);
    printf("%s\n", gantt_times);
    
    // Libérer la mémoire allouée
    free(processus);
    return 0;
}
