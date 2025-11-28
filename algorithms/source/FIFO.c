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

// Fonction de comparaison pour trier les processus par temps d'arrivée
int compare_fifo(const void* a, const void* b) {
    Process* p1 = (Process*)a;
    Process* p2 = (Process*)b;
    return (p1->t_arv > p2->t_arv);
}

// Programme principal pour l'algorithme FIFO
int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <fichier_params>\n", argv[0]);
        return 1;
    }

    Process* processus;
    int n = lire_processus(argv[1], &processus);
    if (n <= 0) return 1;

    printf("\nWelcome to First In First Out Scheduling Algorithm\n");
    afficher_tableau(processus, n);

    qsort(processus, n, sizeof(Process), compare_fifo);

    int temps_actuel = 0;
    printf("The Gantt chart is:\n");
    
    printf("|");
    for (int i = 0; i < n; i++) {
        printf(" %s |", processus[i].pid);
    }
    printf("\n");

    printf("0");
    for (int i = 0; i < n; i++) {
        if (temps_actuel < processus[i].t_arv) {
            temps_actuel = processus[i].t_arv;
        }
        temps_actuel += processus[i].t_exec;
        printf("   %d", temps_actuel);
    }
    printf("\n");
    
    free(processus);
    return 0;
}
