#include "Fliptech.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void initialiser_paquet(Carte paquet[]) {
   
    int index = 0; //compteur pour ranger les cartes une par une ( de la case 0 à 93)

    //---1ère étape : création des 79 cartes numéros---

    //1.On commence par créer les cartes numéros de valeur  0 à 12 avec leur quantité 
    for (int i=0; i <= 12; i++) {
        int quantite;
        if (i==0) {
            quantite = 1; //1 carte 0
        } else {
            quantite = i;
        }
     
        //2.Création des 79 cartes numéros en fonction de leur valeur et de leur quantité
        for (int j=0; j<quantite; j++) {
            paquet[index].type = CARTE_NUMERO;
            paquet[index].valeur = i;
            sprintf(paquet[index].nom, "Carte %d", i); // on donne un nom à la carte
            index++;// on passe à la carte numeroté  suivante
        }
    }
 
    //---2ème étape : création des 6 cartes bonus---

    //1.La carte multiplicateur (x2) : elle double le score du joueur pour chaque manche
    paquet[index].type = CARTE_BONUS;
    paquet[index].valeur = 2 ; //la valeur de la carte bonus multiplicateur
    sprintf(paquet[index].nom, "Bonus x2"); 
    index++; // on passe à la carte bonus suivante

    //2.Les 5 cartes d'addition(+2, +4, +6, +8, +10):ajoutent un nombre de points au score de chaque manche

    int valeurs_additions[5] = {2, 4, 6 ,8 , 10}; // On déclare le tableau de bonus d'addition 
         
     for (int i=0; i<5; i++) {
        paquet[index].type = CARTE_BONUS ;
        paquet[index].valeur = valeurs_additions[i]; //on attribue la valeur correspondante à chaque carte d'addition
        sprintf(paquet[index].nom, "Bonus + %d", valeurs_additions[i]); // on donne un nom à la carte
        index++;
    }
    
   //---3ème étape : création des 9 cartes spéciales---


   //1.On crée une liste avec le nom des 3 types de cartes spéciales
   const  char* noms_cartes_speciales [3] = {"Stop", "Seconde chance", "Trois a la suite"};

   //2.On passe en revue les 3 types de cartes spéciales 
    for (int i=0; i<3; i++) {

   //3.Pour chaque type de carte spéciale, on crée 3 exemplaires (pour avoir un total de 9 cartes spéciales)
        for (int j=0; j<3; j++) {
            paquet[index].type = CARTE_SPECIAL ;
            paquet[index].valeur = i+1; //on attribue une valeur à chaque type de carte spéciale (1 pour stop, 2 pour seconde chance, 3 pour trois à la suite)
            sprintf(paquet[index].nom,"%s", noms_cartes_speciales[i]); //
            index++;
       }
    }
}

//---4ème étape : Fonction pour mélanger le paquet de cartes de manière aléatoire ---

void melanger_paquet(Carte paquet[], int taille) { 
    

    //1 . On parcourt le paquet de la dernière carte à la première 
    for (int i = taille - 1; i > 0; i--) {
        int j = rand() % (i + 1); // On choisit une carte aléatoire entre 0 et i

    //2 . On échange la carte à l'indice i avec la carte à l'indice j
        Carte temp = paquet[i];
        paquet[i] = paquet[j];
        paquet[j] = temp;
    }
}




// --- 5ème étape : fonction pour permettre au joueur de personnaliser le paquet de cartes et la durée de la partie --- 

int initialiser_partie_personnalisee(Carte paquet[], ConfigurerPartie *configurer) {
    int index = 0;
    int quantite;
    int minutes;
    int nb_types_differents = 0;

    
    printf("\n--- CONFIGURATION DE LA PARTIE --- \n");
    
    // 1. Personnalisation du temps de jeu

    printf("Duree de la partie en minutes (ex: 20) : ");
    scanf("%d", &minutes);
    (*configurer).temps_limite_secondes = minutes * 60; 

    // 2. Personnalisation des CARTES NUMEROS (0 à 12) 
    for (int i = 0; i <= 12; i++) {
        
        int saisie_valide; // Pour vérifier si le joueur a bien tapé un nombre
        
        // Boucle pour forcer le joueur à entrer un nombre >= 0 et pas une lettre
        do {
            printf("Nombre de cartes '%d' : ", i);
            saisie_valide = scanf("%d", &quantite);
            
            // Si le joueur a tapé une lettre (alors scanf n'a pas pu lire 1 nombre)
            if (saisie_valide != 1) {
                printf("Erreur : Vous devez taper un nombre, pas une lettre !\n");
                // vide la lettre bloquée dans la mémoire
                while (getchar() != '\n'); 
                quantite = -1; // On met -1 pour forcer la boucle à recommencer
            }
            // Si le joueur a tapé un nombre, mais qu'il est négatif
            else if (quantite < 0) {
                printf("Erreur : La quantite ne peut pas être negative,  tapez un nombre positif.\n");
            }
            
        } while (quantite < 0);




        if (quantite > 0) {
          nb_types_differents++ ; // On compte combien de numéros différents existent
        }

        int espace_restant = (MAX_PAQUET - 15) - index; // On regarde combien de places il reste dans le paquet (en retirant les 15 cartes bonus et spéciales qui seront ajoutées après les numéros)

        if (quantite > espace_restant) {
            int cartes_rejetees = quantite - espace_restant; // On calcule combien ne rentreront pas
            
            if (espace_restant == 0) {
                printf(ROUGE "Attention : Le paquet est totalement plein ! %d carte(s) '%d' rejetees.\n" RESET, cartes_rejetees, i);
            } else {
                printf(ROUGE "Attention : Plus assez de place ! Seulement %d carte(s) '%d' ajoutees. (%d rejetees)\n" RESET, espace_restant, i, cartes_rejetees);
            }
            
            quantite = espace_restant; // On réduit la quantité au maximum possible pour ne pas faire planter le jeu
        }

        // Boucle pour ajouter les cartes numéros dans le paquet en fonction de la quantité choisie
        for (int j = 0; j < quantite; j++) {
            paquet[index].type = CARTE_NUMERO;
            paquet[index].valeur = i;
            sprintf(paquet[index].nom, "Carte %d", i);
            index++;
        }
    }

    // 3. Ajout automatique des  6 cartes  Bonus et 9 cartes  Spéciales 
    // Bonus x2
    paquet[index].type = CARTE_BONUS;
    paquet[index].valeur = 2;
    sprintf(paquet[index].nom, "Bonus x2");
    index++;

    // Les bonus d'addition (+2, +4, +6, +8, +10) 
    int adds[5] = {2, 4, 6, 8, 10};
    for(int k = 0; k < 5; k++) {
        paquet[index].type = CARTE_BONUS;
        paquet[index].valeur = adds[k];
        sprintf(paquet[index].nom, "Bonus +%d", adds[k]);
        index++;
    }

    // les 3 cartes spéciales  (Stop, Seconde chance, 3 à la suite) 
    const char* noms[3] = {"Stop", "Seconde chance", "Trois a la suite"};
    for (int k = 0; k < 3; k++) {

    // 3 exemplaires de chaque carte spéciale

        for (int l = 0; l < 3; l++) { 
            paquet[index].type = CARTE_SPECIAL;
            paquet[index].valeur = k + 1;
            sprintf(paquet[index].nom, "%s", noms[k]);
            index++;
        }
    }
    

    // ---6ème étape : affichage du résumé de la configuration choisie par le joueur---


// On enregistre le nombre total de cartes créées dans la configuration de la partie
    (*configurer).nb_total_cartes = index;
// On indique au programme combien de cartes ont été fabriquées
    return index;
}