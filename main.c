
#include "Fliptech.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>



#ifdef _WIN32
#include <windows.h>
#endif




int main() {

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8); // pour permettre à la console Windows de lire les emojis 
#endif
    srand((unsigned int)time(NULL));//Initialise le générateur de nombres aléatoires avec le temps actuel, converti en entier (unsigned int) pour éviter les avertissements du compilateur.


    //---1ère étape : On prépare la pioche de cartes ---
    ConfigurerPartie maConfig;
    Carte paquet[MAX_PAQUET]; 
   
int mode;
int saisie_mode;

    //1. Boucle de sécurité pour le choix du mode
    do {
        printf(CYAN "1. Mode Standard (Classique)\n" RESET);
        printf(CYAN "2. Mode Personnalisé\n" RESET);
        printf("Choix : ");
        saisie_mode = scanf("%d", &mode);

        
        if (saisie_mode != 1) {
            printf(ROUGE "Erreur : Veuillez entrer un chiffre (1 ou 2) !\n\n" RESET);
            while (getchar() != '\n'); // Vide la mémoire pour éviter la boucle infinie
            mode = 0; // Force la boucle à recommencer
        } else if (mode != 1 && mode != 2) {
            printf(ROUGE "Erreur : Choix invalide, tapez 1 ou 2 !\n\n" RESET);
        }
    } while (mode != 1 && mode != 2);



    if (mode == 2) {
        initialiser_partie_personnalisee(paquet, &maConfig);
    } else {
        initialiser_paquet(paquet);
        maConfig.temps_limite_secondes = 1200; // 20 minutes par defaut 
        maConfig.nb_total_cartes = 94; // Taille classique
    }

    //2.  On passe la bonne taille au mélange !
    melanger_paquet(paquet, maConfig.nb_total_cartes);

    // 3. Affichage du paquet mélangé
    printf(VIOLET "Paquet de cartes mélangé :\n" RESET);
    for (int i = 0; i < maConfig.nb_total_cartes; i++) {
        printf("%s (Type: %d, Valeur: %d)\n", paquet[i].nom, paquet[i].type, paquet[i].valeur);
    }

    //---2ème étape : on configure la partie : nombre de joueurs, leurs noms, leurs scores ---


//1.Message de bienvenue 
    int nb_joueurs;
printf(CYAN "---------------------------------------------------\n\n" RESET);

printf ("  🎲 BIENVENUE DANS LE JEU FLIPTECH ! 😀\n\n"); 

printf(CYAN "---------------------------------------------------\n\n" RESET);

// 2.On demande le nombre de joueurs et on vérifie que c'est un nombre valide et au moins 2 joueurs

int saisie_valide;
do {
    printf("Entrez le nombre de joueurs (ex: 3) : ");
    saisie_valide = scanf("%d", &nb_joueurs);

    if (saisie_valide != 1) {
        printf(ROUGE "Erreur : Veuillez entrer un chiffre !\n" RESET);
        while (getchar() != '\n'); // Vide la mémoire pour éviter la boucle infinie
        nb_joueurs = -1;
    } else if (nb_joueurs < 2) {
        printf(ROUGE "Erreur : Il faut au moins 2 joueurs !\n" RESET);
    }
} while (nb_joueurs < 2);

Joueur joueurs[nb_joueurs]; // On crée le tableau 






    // 3.Appel de ta fonction pour initialiser les joueurs
    initialiser_joueurs(joueurs, nb_joueurs);


    //4. Initialisation du tableau de statistiques des cartes piochées
    int stats_cartes[13] = {0};


    //---3 ème étape : le déroulement du jeu  (En boucle)---

//1. On lance le chronomètre au début de la partie pour gérer la limite de temps de 20 minutes
    time_t debut_partie = time(NULL); 

    bool fin_de_partie = false;
    int index_pioche = 0; 
// 2. Boucle principale de la partie : tant que la partie n'est pas terminée, on joue des manches
    while (fin_de_partie == false) {
        printf(JAUNE "    ✨ NOUVELLE MANCHE ! ✨ \n" RESET);



// 3.Réinitialisation des données des joueurs pour la nouvelle manche
        reinitialiser_pour_nouvelle_manche(joueurs, nb_joueurs);


// 4. On passe au tour de chaque joueur tant que la manche n'est pas terminée 
        bool manche_en_cours = true; 

        while (manche_en_cours == true) {
            for (int i = 0; i < nb_joueurs; i++) {
                

if (index_pioche >= maConfig.nb_total_cartes) {
                    printf(ROUGE "🎴 La pioche est epuisee ! Fin immediate de la manche.\n" RESET);
                    manche_en_cours = false;
                    break; // On casse la boucle "for" pour arrêter les tours
                }
                

                //5.Si le joueur a perdu ou est sorti de la manche, on passe au joueur suivant
                if (joueurs[i].a_perdu_manche == true || joueurs[i].est_sortie_manche == true) {
                    continue;
                }


                
               
// 6.C'est un bot, il fait son choix tout seul 
                int choix = 0;
                if (joueurs[i].est_un_bot == true) {
                    
                    choix = faire_choix_ordinateur(joueurs[i]);
                } else {
                    double temps_ecoule_actuel = difftime(time(NULL), debut_partie);
                    int temps_restant = maConfig.temps_limite_secondes - (int)temps_ecoule_actuel;
                    int min_restantes = temps_restant / 60;
                    int sec_restantes = temps_restant % 60;

                    if (temps_restant > 0) {
                        printf(CYAN "\n⏳ Temps restant : %02d:%02d\n" RESET, min_restantes, sec_restantes);
                    } else {
                        printf(ROUGE "\n⏳ Le temps est ecoule ! Derniere chance !\n" RESET);
                    }
                    

                   
                   
  // 7.C'est un humain, on lui demande de faire son choix entre piocher, s'arrêter ou voir les statistiques
    int saisie;
    do {
        printf(VIOLET "▶ C'est au tour de %s !\n" RESET, joueurs[i].nom);
        printf(CYAN "Choix :" RESET " 1=Piocher une carte, 2= s'arrêter, 3= Voir statistiques \n" JAUNE "Choix du joueur: " RESET);
        saisie = scanf("%d", &choix);

        //Pour sécurité
        if (saisie != 1) { 
            // Cas 1 : Le joueur a tapé une lettre ou un mot
            printf(ROUGE "Erreur : Pas de lettres, choisissez 1, 2 ou 3 !\n\n" RESET);
            while (getchar() != '\n'); // On vide la mémoire
            choix = 0; // On force la boucle à recommencer
            
        } else if (choix == 3) {
            // Cas 2 : Le joueur veut voir les stats
            afficher_statistiques(joueurs[i], stats_cartes);
            
        } else if (choix < 0) {
            // Cas 3 : Le joueur a tapé un nombre négatif
            printf(ROUGE "Erreur : Pas de nombres negatifs, choisissez 1, 2 ou 3 !\n\n" RESET);
            
        } else if (choix != 1 && choix != 2) {
            // Cas 4 : Le joueur a tapé 0, 4, 5, 100...
            printf(ROUGE "Erreur : Ce nombre n'est pas valide, choisissez 1, 2 ou 3 !\n\n" RESET);
        }
        
    } while (choix != 1 && choix != 2);
                        
                    
                } //  Maintenant le bot et l'humain peuvent jouer la suite 

                // le joueur choisit de s'arrêter
                if (choix == 2) {
                    printf("%s a choisi de s'arrêter avec un score de manche de " JAUNE "%d" RESET " points.\n", joueurs[i].nom, joueurs[i].score_manche);
                    joueurs[i].est_sortie_manche = true;

                // le joueur choisit de piocher une carte 
                } else if (choix == 1) {
                    // Appel de ta fonction pour jouer le tour
                    jouer_tour(joueurs, nb_joueurs, i, paquet, &index_pioche, &manche_en_cours, stats_cartes , maConfig.nb_total_cartes  );
                } 
            } // Fin de la boucle for des joueurs
   
            // 8.Condition pour éviter la boucle infinie si tous les joueurs perdent ou sortent de la manche 
            int joueurs_actifs = 0;
            for (int i = 0; i < nb_joueurs; i++) {
                if (joueurs[i].a_perdu_manche == false && joueurs[i].est_sortie_manche == false) {
                    joueurs_actifs++;
                }
            }

            if(joueurs_actifs == 0) {
                printf(ROUGE "Tous les joueurs ont perdu ou sont sortis de la manche. La manche se termine.\n" RESET);
                manche_en_cours = false;
            }

        } // Fin de while(manche_en_cours)

        // 9. Appel de ta fonction pour afficher les résultats de la manche
        afficher_resultats_manche(joueurs, nb_joueurs);
        
        //10. Vérifications des conditions de fin de partie 
        double temps_ecoule = difftime(time(NULL), debut_partie);

        // 11.Appel de ta fonction pour vérifier la fin de partie 
        verifier_fin_partie(joueurs, nb_joueurs, temps_ecoule, maConfig, index_pioche, &fin_de_partie);

    } // Fin while(fin_de_partie)

    // ---4ème étape :  Classement et annonce du vainqueur si plus de pioche ou temps écoulé --- 



// 1. Tri à bulles : On trie le tableau des joueurs par ordre décroissant de points
    for (int i = 0; i < nb_joueurs - 1; i++) {
        for (int j = 0; j < nb_joueurs - i - 1; j++) {
            if (joueurs[j].score_partie < joueurs[j+1].score_partie) {
                // Si le score du joueur actuel est plus petit que le suivant, on les échange !
                Joueur temp = joueurs[j];        // On met le joueur J en sécurité dans "temp"
                joueurs[j] = joueurs[j+1];       // Le joueur J+1 prend la place de J
                joueurs[j+1] = temp;             // Le joueur J (dans temp) prend la place de J+1
            }
        }
    }


    //2. On détermine le gagnant (le joueur avec le score de partie le plus élevé) et on affiche un message de félicitations, ainsi que les scores des autres joueurs pour le suspense !
    int max = -1, gagnant = 0;

    for (int i = 0; i < nb_joueurs; i++) {
        if (joueurs[i].score_partie > max) {
            max = joueurs[i].score_partie;
            gagnant = i;
        }
    }
printf(VERT "-------------------------🥇 Le Vainqueur de la partie 🥇--------------------------\n" RESET);
printf(VERT "\n🏆 Le gagnant de la partie est %s avec un score de "JAUNE" %d points ! 🏆\n" RESET, joueurs[gagnant].nom, joueurs[gagnant].score_partie);

printf(VERT "-----------------------------------------------------------------------------------\n" RESET);

    //  Affichage des scores des autres joueurs 


 printf(CYAN "----------------------📊 Le résultat des autres joueurs 📊----------------------\n" RESET);
 
    for (int i = 0; i < nb_joueurs; i++) {
        if (i != gagnant) {
            printf("- %s : " JAUNE "%d points\n" RESET, joueurs[i].nom, joueurs[i].score_partie);
        }
    }
printf(CYAN "---------------------------------------------------------------------------------\n" RESET);


    // ---5ème étape : Sauvegarde des scores dans un fichier texte --- 



    sauvegarder_scores(joueurs, nb_joueurs); 
    return 0;
}