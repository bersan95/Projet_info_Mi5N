#include "Fliptech.h"
#include <stdio.h>

// 1. Fonction qui décide à la place du joueur (Renvoie 1 pour piocher, 2 pour s'arrêter)
int faire_choix_ordinateur(Joueur bot) {
    
//2. On va lui donner une stratégie simple : il pioche s'il a moins de 4 cartes, et il s'arrête s'il a 4 cartes ou plus (pour ne pas prendre trop de risques).
    if (bot.nb_cartes_differentes >= 4) {
        printf(" L'ordinateur (%s) a reflechi et decide de s'arreter.\n", bot.nom);
        return 2; // 2 = s'arrêter
    } else {
        printf(" L'ordinateur (%s) decide de piocher !\n", bot.nom);
        return 1; // 1 = piocher
    }
}