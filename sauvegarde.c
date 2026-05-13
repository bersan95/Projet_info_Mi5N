#include "Fliptech.h"
#include <stdio.h>
#include <string.h> //  pour utiliser strcmp()

//---1ère étape : Fonction pour sauvegarder les scores finaux dans un fichier texte---
void sauvegarder_scores(Joueur joueurs[], int nb_joueurs) {
    char reponse[10]; // Tableau de caractères pour stocker le mot entier
    int saisie_valide = 0; // Variable pour contrôler la boucle

    // Boucle qui continue tant que l'utilisateur ne tape pas "oui" ou "non"
    while (!saisie_valide) {
        printf("Voulez-vous sauvegarder les scores de la partie ? (oui / non) : ");
        scanf("%9s", reponse); // %9s pour lire un mot (limité à 9 caractères pour éviter les dépassements)

        // On vérifie si la réponse est "oui" (on accepte aussi "Oui" ou "OUI")
        if (strcmp(reponse, "oui") == 0 || strcmp(reponse, "Oui") == 0 || strcmp(reponse, "OUI") == 0) {
            saisie_valide = 1; // La saisie est bonne, on sortira de la boucle
            char nom_fichier[100];
            
            printf("Entrez le nom du fichier (ex: resultats) : ");
            scanf("%99s", nom_fichier);

            //---2ème étape : On ouvre (ou crée) le fichier en mode écriture ("w" pour write)
            FILE *fichier = fopen(nom_fichier, "w");
            
            if (fichier != NULL) {
                fprintf(fichier, " RESULTATS DE LA PARTIE FLIPTECH ! \n");
                for (int i = 0; i < nb_joueurs; i++) {
                    fprintf(fichier, "- %s : %d points\n", joueurs[i].nom, joueurs[i].score_partie);
                }
                fclose(fichier); // On referme le fichier 

                // ---3ème étape : Confirmation de la sauvegarde ---
                printf(VERT " Succès ! Scores sauvegardés dans le fichier '%s' !\n" RESET, nom_fichier);
            } else {
                printf(ROUGE "Erreur : Impossible de créer le fichier de sauvegarde.\n" RESET);
            }
        } else if (strcmp(reponse, "non") == 0 || strcmp(reponse, "Non") == 0 || strcmp(reponse, "NON") == 0) {
            saisie_valide = 1; // La saisie est bonne, on sortira de la boucle
            printf("Sauvegarde annulée.\n");
            
        // Si ce n'est ni "oui", ni "non"
        } else {
            // Le message d'erreur 
            printf(ROUGE "Erreur : Saisie invalide. Veuillez repondre par 'oui' ou par 'non'.\n\n" RESET);
        }
    }
}