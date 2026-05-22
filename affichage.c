#include "Fliptech.h"
#include <stdio.h>
#include <stdbool.h>

//---  1ère étape : Fonction pour  afficher les statistiques pour aider le joueur 
void afficher_statistiques(Joueur j, int stats_cartes[]) {
    printf(CYAN "\n--- 📊 VOS CARTES ET STATISTIQUES 📊 ---\n" RESET);
    
    // 1. Affichage des cartes possédées par le joueur
    printf("Vos cartes actuelles : ");
    bool a_cartes = false;
    for(int k=0; k<=12; k++) {
        if(j.cartes_possedees[k] == 1) {
            printf(VERT "[%d] " RESET, k);
            a_cartes = true;
        }
    }
    if(!a_cartes) printf("Aucune");
    printf("\n");

    // 2. Affichage des bonus possédées en cours
    printf(JAUNE "\n--- VOS BONUS ACTUELS ---\n" RESET);
    printf("Multiplicateur de score : x%d\n", j.multiplicateur);
    printf("Bonus d'addition : +%d points\n", j.bonus_addition);
    if (j.a_2nd_chance) {
        printf(VERT "Bouclier Seconde Chance : ACTIF 🛡️\n" RESET);
    } else {
        printf(ROUGE "Bouclier Seconde Chance : INACTIF\n" RESET);
    }
    printf("\n");

    // 3. Affichage des cartes déjà piochées par tout le monde
    printf("Cartes numeros déjà sorties de la pioche depuis le melange :\n");
    for(int k=0; k<=12; k++) {
        if(stats_cartes[k] > 0) {
            printf(JAUNE " - Numero %d : pioche %d fois\n" RESET, k, stats_cartes[k]);
        }
    }
    printf(CYAN "-------------------------------------\n\n" RESET);
}

// --- 2ème étape : Fonction pour afficher les résultats 
void afficher_resultats_manche(Joueur joueurs[], int nb_joueurs) {
    printf(CYAN "\n----- 🏁 RESULTATS DE LA MANCHE  🏁 ------\n" RESET);
    for (int i = 0; i < nb_joueurs; i++) {
        if(joueurs[i].a_perdu_manche == false) {
            //1. On multiplie d'abord les points des numéros, puis on ajoute les bonus [+]
            joueurs[i].score_manche = (joueurs[i].score_manche * joueurs[i].multiplicateur) + joueurs[i].bonus_addition;

            //2. On ajoute les 15 points après la multiplication 
            if (joueurs[i].nb_cartes_differentes == 7) {
                joueurs[i].score_manche += 15;
            }
        }
        joueurs[i].score_partie += joueurs[i].score_manche;
        printf("- %s : " VERT "%d points" RESET " (Total : " JAUNE "%d points" RESET ")\n", joueurs[i].nom, joueurs[i].score_manche, joueurs[i].score_partie);
    }
    printf(CYAN "------------------------------------------\n\n" RESET);
}