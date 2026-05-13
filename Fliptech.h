#ifndef Fliptech_H
#define Fliptech_H

 #include <stdio.h>
 #include <stdlib.h>
 #include <time.h>
 #include <stdbool.h>
 #include <string.h>

//---1ère étape :paramètrage du jeu : nombre de cartes avec sa répartition , score de victoire et minuteur ---_

 #define TOTAL_CARTES 94
 #define NB_CARTES_NUMERO 79
 #define NB_CARTES_BONUS 6
 #define NB_CARTES_SPECIALES 9
 #define SCORE_VICTOIRE  200
 #define DUREE_MANCHE 1200//20 minutes en secondes

#define MAX_PAQUET 200 // Capacité maximale du paquet de cartes 


// --- 2ème étape : création des structures pour les cartes, les joueurs et la configuration de la partie --- 

//1. On ajoute une structure pour regrouper les réglages de la partie
typedef struct {
    int temps_limite_secondes;
    int nb_total_cartes;
} ConfigurerPartie;

// 2.On définit les différents types de  cartes possibles avec l'énum
 typedef enum {
     CARTE_NUMERO, 
     CARTE_BONUS, 
     CARTE_SPECIAL,
 } TypeCarte ;

//3. On définit la structure d'une carte
    typedef struct {
        TypeCarte type; 
        int valeur; 
        char nom[50]; 
    } Carte ;

// 4. On définit la structure d'un joueur 
    typedef struct {
        char nom[50]; 
        Carte main[TOTAL_CARTES];
        int nb_cartes;
        int score_manche;
        int score_partie ;
        int multiplicateur;
        int bonus_addition ;
        int nb_cartes_differentes;
        bool a_2nd_chance;
        bool cartes_possedees [13];
        bool est_sortie_manche;
        bool a_perdu_manche; 
        bool est_un_bot;
           
    } Joueur ;

    //---3ème étape : On annonce les fonctions pour gérer le jeu dans les autres fichiers .c (moteur_jeu.c, Cartes.c) et éviter les erreurs de compilation dues à des fonctions non déclarées.---

//1. Les Prototypes : Annonce des fonctions pour gérer le jeu
void initialiser_paquet(Carte paquet[]);

void melanger_paquet(Carte paquet[], int taille); // CORRIGE ICI : ajout de "int taille"

int faire_choix_ordinateur (Joueur bot);

int initialiser_partie_personnalisee(Carte paquet[], ConfigurerPartie *configurer);


//2. Prototypes : gestion du déroulement de la partie
void initialiser_joueurs(Joueur joueurs[], int nb_joueurs);

void afficher_resultats_manche(Joueur joueurs[], int nb_joueurs);

void appliquer_carte_speciale(Joueur joueurs[], int nb_joueurs, int i, Carte carte_piochee, Carte paquet[], int *index_pioche, int stats_cartes[], int taille_paquet   );

void jouer_tour(Joueur joueurs[], int nb_joueurs, int i, Carte paquet[], int *index_pioche, bool *manche_en_cours, int stats_cartes[] , int taille_paquet   );

void verifier_fin_partie(Joueur joueurs[], int nb_joueurs, double temps_ecoule, ConfigurerPartie maConfig, int index_pioche, bool *fin_de_partie);



//3. Prototypes : statistiques , Sauvegarde et réinitialisation pour nouvelle manche
void afficher_statistiques(Joueur j, int stats_cartes[]);

void sauvegarder_scores(Joueur joueurs[], int nb_joueurs);

void reinitialiser_pour_nouvelle_manche(Joueur joueurs[], int nb_joueurs);


// --- 4ème étape : les couleurs et emojis pour rendre le jeu plus fun ! --- 

//1. Les couleurs et emojis (Terminal ANSI) 
#define RESET   "\033[0m"
#define ROUGE   "\033[1;31m"
#define VERT    "\033[1;32m"
#define JAUNE   "\033[1;33m"
#define BLEU    "\033[1;34m"
#define CYAN    "\033[1;36m"
#define VIOLET  "\033[1;35m"
#define BLANC   "\033[1;37m"
#define NOIR    "\033[1;30m"
#define GRIS    "\033[1;90m"


#define EFFACER "\033[2J\033[H" // pour nettoyer l'écran
#endif