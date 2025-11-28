// Placez cette fonction au début de chaque fichier .c d'algorithme
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