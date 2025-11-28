#ifndef PROCESS_H
#define PROCESS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure pour représenter un processus, basée sur votre documentation
typedef struct Process {
    char pid[5];      // Nom du processus (ex: "P1")
    int t_arv;        // Temps d'arrivée
    int t_exec;       // Temps d'exécution total (Burst Time)
    int priorite;     // Priorité
    int rnt;          // Temps d'exécution restant (Remaining Time)
    int temps_attente; // Temps d'attente
    int temps_sejour;  // Temps de séjour (Turnaround time)
    int a_deja_commence; // Flag pour SRT et RR
} Process;

// Fonction pour lire les processus depuis le fichier de configuration
// On la déclare ici pour qu'elle soit utilisable par tous les algorithmes
int lire_processus(const char* nom_fichier, Process** processus_ptr);

#endif //PROCESS_H