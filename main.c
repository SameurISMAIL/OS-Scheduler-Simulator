#include <stdio.h>
#include <stdlib.h> // Pour system()
#include <string.h> // Pour snprintf()

// Fonction pour afficher le menu des choix à l'utilisateur
void afficher_menu() {
    printf("\n========= Algorithm Game =========\n");
    printf("1. RR (Round Robin)\n");
    printf("2. SJF (Shortest Job First)\n");
    printf("3. FIFO (First In First Out)\n");
    printf("4. SRT (Shortest Remaining Time)\n");
    printf("5. PRIORITY\n");
    printf("6. Exit\n");
    printf("==================================\n");
    printf("Enter your choice: ");
}

// Le programme principal qui sert de point d'entrée
int main(int argc, char *argv[]) {
    // Étape 1: Vérifier que l'utilisateur a bien fourni le nom du fichier de configuration en argument
    // argc doit être 2 : 1 pour le nom du programme, 1 pour le nom du fichier
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <fichier_params>\n", argv[0]);
        return 1; // Quitter avec une erreur
    }

    // Stocker le nom du fichier de configuration pour l'utiliser plus tard
    const char* nom_fichier_params = argv[1];
    
    int choix;
    char commande[256]; // Une chaîne de caractères pour construire la commande à exécuter

    // Étape 2: Boucler tant que l'utilisateur ne choisit pas de quitter (option 6)
    do {
        afficher_menu();
        scanf("%d", &choix); // Lire le choix de l'utilisateur

        // Étape 3: Exécuter une action en fonction du choix
        switch (choix) {
            case 1:
                // Construire la commande: "./algorithms/build/RR params.txt"
                snprintf(commande, sizeof(commande), "./algorithms/build/RR %s", nom_fichier_params);
                system(commande); // Exécuter la commande dans le terminal
                break;
            case 2:
                // Construire la commande: "./algorithms/build/SJF params.txt"
                snprintf(commande, sizeof(commande), "./algorithms/build/SJF %s", nom_fichier_params);
                system(commande);
                break;
            case 3:
                // Construire la commande: "./algorithms/build/FIFO params.txt"
                snprintf(commande, sizeof(commande), "./algorithms/build/FIFO %s", nom_fichier_params);
                system(commande);
                break;
            case 4:
                // Construire la commande: "./algorithms/build/SRT params.txt"
                snprintf(commande, sizeof(commande), "./algorithms/build/SRT %s", nom_fichier_params);
                system(commande);
                break;
            case 5:
                // Construire la commande: "./algorithms/build/PRIORITY params.txt"
                snprintf(commande, sizeof(commande), "./algorithms/build/PRIORITY %s", nom_fichier_params);
                system(commande);
                break;
            case 6:
                printf("Exiting...\n");
                break; // Ce break va permettre de sortir de la boucle do-while
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choix != 6);

    return 0; // Terminer le programme avec succès
}
