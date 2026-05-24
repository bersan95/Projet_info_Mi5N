#include "Fliptech.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//---1ère étape : fonction pour configurer les joueurs (leur pseudo, s'ils sont humains ou bots, et réinitialiser leurs compteurs et statuts pour le début de la partie) ---
void initialiser_joueurs(Joueur joueurs[], int nb_joueurs) {
    int compteur_bot = 1; // Compteur indépendant pour numéroter proprement les ordinateurs (Ordi_1, Ordi_2...)

    for (int i = 0; i < nb_joueurs; i++) {
        int choix_type, saisie_type;

        // 1. On demande à l'utilisateur si le joueur est un humain ou un ordinateur avec les conditions de sécurité pour éviter les erreurs de saisie
        do {
            printf("\nLe joueur %d est-il un humain (1) ou un ordinateur (2) ? : ", i + 1);
            saisie_type = scanf("%d", &choix_type);
            
            if (saisie_type != 1) {
                printf(ROUGE "Erreur : Veuillez taper un chiffre \n" RESET);
                while(getchar() != '\n'); 
            } else if (choix_type != 1 && choix_type != 2) {
                printf(ROUGE "Erreur : Choisissez 1 ou 2 !\n" RESET);
            }
        } while (choix_type != 1 && choix_type != 2);

        // 2. En fonction du choix, on définit le type du joueur et son nom
        if (choix_type == 2) {
            joueurs[i].est_un_bot = true; 
            sprintf(joueurs[i].nom, "ordi_%d", compteur_bot); // Donne un nom auto à l'ordi
            printf(BLEU "🤖 Le joueur %d sera l'ordinateur : %s\n" RESET, i + 1, joueurs[i].nom);
            compteur_bot++; // Passe à l'indice suivant pour le prochain bot
        } else {
            joueurs[i].est_un_bot = false; 
            printf("👤 Entrez le pseudo du joueur %d : ", i + 1);
            
       // 3. On vide la mémoire laissée par le scanf pour éviter que fgets ne prenne le reste de la ligne comme nom du joueur
            while (getchar() != '\n'); 
            
       // 4. On lit la ligne avec fgets pour permettre les pseudos avec espaces, et on remplace le saut de ligne par une fin de chaîne pour éviter les problèmes d'affichage plus tard
            fgets(joueurs[i].nom, 50, stdin); 
            
            // 3. On parcourt le tableau case par case pour remplacer le caractère de saut de ligne par une fin de chaîne
            int k = 0;
            while (joueurs[i].nom[k] != '\0') { // Tant qu'on n'est pas à la fin du mot
                if (joueurs[i].nom[k] == '\n') {
                    joueurs[i].nom[k] = '\0';   // On remplace la touche "Entrée" par la fin de chaîne
                    break;                      // On arrête la boucle
                }
                k++;
            }
        }
        
        // 4. On mets les compteurs à 0 pour chaque joueur
        joueurs[i].score_manche = 0;    
        joueurs[i].score_partie = 0;
        joueurs[i].nb_cartes = 0;
        joueurs[i].bonus_addition = 0;
        joueurs[i].multiplicateur = 1;// le multiplicateur x1 au score de chaque joueur , x2 si bonus multiplicateur, pas de 0 pour éviter de faire 0 points
        joueurs[i].nb_cartes_differentes = 0;

        // 5. On initialise les statuts de chaque joueur à false (booléens)
        joueurs[i].a_perdu_manche = false;
        joueurs[i].est_sortie_manche = false;
        joueurs[i].a_2nd_chance = false;

        // 6. Vider la mémoire des cartes possédées pour chaque joueur
        for (int j = 0; j < 13; j++) {
            joueurs[i].cartes_possedees[j] = 0;
        }
    }
}

// --- 2ème étape : Fonction qui gère les effets des cartes spéciales ---
void appliquer_carte_speciale(Joueur joueurs[], int nb_joueurs, int i, Carte carte_piochee, Carte paquet[], int *index_pioche, int stats_cartes[] , int taille_paquet   ) {
    int cible; 
    
    // --- Carte stop---
    if (strcmp(carte_piochee.nom, "Stop") == 0) { 
        printf(BLEU "=> 🛑 Qui voulez-vous forcer a s'arreter pour cette manche 🛑 ?\n" RESET);
        
        // 1. On affiche la liste des cibles possibles
        for (int j = 0; j < nb_joueurs; j++) {
            // On ne propose que les joueurs qui n'ont pas encore perdu ou qui ne sont pas déjà arrêtés
            if (joueurs[j].a_perdu_manche == false && joueurs[j].est_sortie_manche == false) {
                printf("%d. %s\n", j + 1, joueurs[j].nom);
            }
        }
        
        // 2. Le joueur choisit sa cible avec  les conditions de sécurité pour éviter les erreurs de saisie
        int cible_valide = 0;
        
        // Si c'est le bot qui joue, il choisit au hasard
        if (joueurs[i].est_un_bot) {
            // SECURITE : 1. On compte s'il y a des cibles valides avant de lancer la boucle                      
            int cibles_possibles = 0;

            for (int j = 0; j < nb_joueurs; j++) {
                if (j != i && joueurs[j].a_perdu_manche == false && joueurs[j].est_sortie_manche == false) {
                    cibles_possibles++;
                }
            }

            //  S'il y a au moins une cible, le bot peut choisir
            if (cibles_possibles > 0) {
                do {
                    cible = rand() % nb_joueurs;
                    if ( cible != i && joueurs[cible].a_perdu_manche == false && joueurs[cible].est_sortie_manche == false) {
                        cible_valide = 1;
                    }
                } while (cible_valide == 0);
                
                printf(BLEU "🤖 L'ordinateur a choisi de cibler %s !\n" RESET, joueurs[cible].nom);
            } else {
                // 3. S'il n'y a personne, l'effet est annulé pour éviter le freeze
                printf(BLEU "🤖 L'ordinateur n'a aucun cible valide a arreter ! L'effet est annule.\n" RESET);
                return; // On sort de la fonction sans appliquer le Stop
            }
            
        } else {
            // Si c'est un humain, on lui demande de taper un numéro
            do {
                printf("Entrez le numero du joueur : ");
                int saisie = scanf("%d", &cible);

                if (saisie != 1) {
                    printf(ROUGE "Erreur : Veuillez taper un nombre !\n" RESET);
                    while (getchar() != '\n'); 
                } else if (cible < 1 || cible > nb_joueurs) {
                    printf(ROUGE "Erreur : Ce numero n'existe pas !\n" RESET);
                } else if (joueurs[cible - 1].a_perdu_manche == true || joueurs[cible - 1].est_sortie_manche == true) {
                    printf(ROUGE "Erreur : Ce joueur a deja perdu ou est sorti !\n" RESET);
                } else {
                    cible_valide = 1; 
                }
            } while (cible_valide == 0);
            
            cible = cible - 1; // On adapte pour l'index du tableau
        }

        // 3. On applique l'effet à la cible
        printf(ROUGE " => 🛑 %s est forcé de s'arreter 🛑 !\n" RESET, joueurs[cible].nom);
        joueurs[cible].est_sortie_manche = true; 
        
    // --- Carte seconde chanche  --- 
    } else if (strcmp(carte_piochee.nom, "Seconde chance") == 0) { 
        printf(VERT "=> 🛡️ Tu es protégé contre ton prochain doublon 🛡️ !\n" RESET);
        joueurs[i].a_2nd_chance = true; 
        
    // --- Carte 3 à la suite  --- 
    } else if (strcmp(carte_piochee.nom, "Trois a la suite") == 0) {
        printf(BLEU "=> 🃙 🃈 🂥  Qui doit piocher 3 cartes de suite 🃙 🃈 🂥     ?\n" RESET);
        
        // 1. On affiche la liste (ici, on peut se cibler soi-même )
        for (int j = 0; j < nb_joueurs; j++) {
            if (joueurs[j].a_perdu_manche == false && joueurs[j].est_sortie_manche == false) {
                printf("%d. %s\n", j + 1, joueurs[j].nom);
            }
        }
        
        // 2. Le joueur choisit sa cible avec les conditions de sécurité pour éviter les erreurs de saisie
        int cible_valide = 0;
        
        // Si c'est le bot qui joue, il choisit au hasard
        if (joueurs[i].est_un_bot) {
            // Sécurité 2 : On compte s'il y a des cibles valides avant de lancer la boucle                      
            int cibles_possibles = 0;

            for (int j = 0; j < nb_joueurs; j++) {
                if (j != i && joueurs[j].a_perdu_manche == false && joueurs[j].est_sortie_manche == false) {
                    cibles_possibles++;
                }
            }

            // S'il y a au moins une cible, le bot peut choisir
            if (cibles_possibles > 0) {
                do {
                    cible = rand() % nb_joueurs;
                    if (cible != i && joueurs[cible].a_perdu_manche == false && joueurs[cible].est_sortie_manche == false) {
                        cible_valide = 1;
                    }
                } while (cible_valide == 0);
                
                printf(BLEU "🤖 L'ordinateur a choisi de cibler %s !\n" RESET, joueurs[cible].nom);
            } else {
                // S'il n'y a personne, l'effet est annulé pour éviter la boucle infinie
                printf(BLEU "🤖 L'ordinateur n'a aucune cible valide ! L'effet est annule.\n" RESET);
                return; // On sort de la fonction
            }
            
        } else {
            // Si c'est un humain, on lui demande de taper un numéro
            do {
                printf("Entrez le numero du joueur : ");
                int saisie = scanf("%d", &cible);

                if (saisie != 1) {
                    printf(ROUGE "Erreur : Veuillez taper un nombre !\n" RESET);
                    while (getchar() != '\n'); 
                } else if (cible < 1 || cible > nb_joueurs) {
                    printf(ROUGE "Erreur : Ce numero n'existe pas !\n" RESET);
                } else if (joueurs[cible - 1].a_perdu_manche == true || joueurs[cible - 1].est_sortie_manche == true) {
                    printf(ROUGE "Erreur : Ce joueur a deja perdu ou est sorti !\n" RESET);
                } else {
                    cible_valide = 1; 
                }
            } while (cible_valide == 0);
            
            cible = cible - 1; // On adapte pour l'index du tableau
        }

        // 3. On annonce l'effet
        printf(JAUNE " 🃙 🃈 🂥 %s doit piocher 3 cartes   🃙 🃈 🂥 !\n" RESET, joueurs[cible].nom);
        
        for (int k = 0; k < 3; k++) {
            // Securite 3 : on verifie si la pioche n'est pas vide et on verifie si la pioche ne dépasse pas la taille de la mémoire 
            if (*index_pioche >= taille_paquet) {
                printf(ROUGE "   🎴 La pioche est vide, la rafale s'arrete !\n" RESET);
                break;
            }

            //  On tire la carte
            Carte c = paquet[*index_pioche];
            (*index_pioche)++; 
            printf("   🎴 Pioche %d : %s\n", k + 1, c.nom);

            if (c.type == CARTE_NUMERO) {
                stats_cartes[c.valeur]++; // On met à jour les stats même pendant la rafale !

                // 1. Cas du Doublon 
                if (joueurs[cible].cartes_possedees[c.valeur] == 1) {
                    if(joueurs[cible].a_2nd_chance) {
                        printf(JAUNE "   🛡️ Sauve par sa Seconde Chance  🛡️ !\n" RESET);
                        joueurs[cible].a_2nd_chance = false;
                    } else {
                        printf(ROUGE "   ❌ Doublon ! %s a perdu la manche ❌  !\n" RESET, joueurs[cible].nom);
                        joueurs[cible].score_manche = 0;
                        joueurs[cible].a_perdu_manche = true;
                        break; // Sort de la boucle for (arrête de piocher)
                    }
                } else {
                    // 2. Ajout normal de la carte
                    joueurs[cible].cartes_possedees[c.valeur] = 1;
                    joueurs[cible].score_manche += c.valeur;
                    joueurs[cible].nb_cartes_differentes++;

                    // 3. Cas du Flip 7 pendant la rafale
                    if (joueurs[cible].nb_cartes_differentes == 7) {
                        printf(VERT "   🎉 FLIP 7 pendant la rafale  🎉 !\n" RESET);
                        break; 
                    }
                }
            } 
            // 4.gestion des Bonus
            else if (c.type == CARTE_BONUS) {
                printf(JAUNE "  *** ⭐ Bonus obtenu pendant la rafale ⭐ ! ***\n" RESET);
                if (c.valeur == 2 && c.nom[6] == 'x') joueurs[cible].multiplicateur = 2;
                else joueurs[cible].bonus_addition += c.valeur;
            } 
            // 5.Gestion des Speciales
            else if (c.type == CARTE_SPECIAL) {
                printf(BLEU "   ⚡ Carte speciale obtenue pendant la rafale ⚡ !\n" RESET);
                appliquer_carte_speciale(joueurs, nb_joueurs, cible, c, paquet, index_pioche, stats_cartes , taille_paquet   );
            }
        }
    }
}

// --- 3ème étapes :fonction qui exécute l'action de piocher ---
void jouer_tour(Joueur joueurs[], int nb_joueurs, int i, Carte paquet[], int *index_pioche, bool *manche_en_cours, int stats_cartes[] , int taille_paquet   ) {
    Carte carte_piochee = paquet[*index_pioche];  
    int valeur_carte = carte_piochee.valeur;
    (*index_pioche)++; // Passer à la carte suivante dans la pioche 
    
    if(carte_piochee.type == CARTE_NUMERO){
        //1. On enregistre la carte dans les statistiques de la pioche
        stats_cartes[valeur_carte]++;

        // Vérifier si le joueur possède déjà la carte piochée 
        if (joueurs[i].cartes_possedees[valeur_carte] == 0) {
            printf("%s ajoute la carte " VERT "%d" RESET " a sa collection. ✅\n", joueurs[i].nom, valeur_carte);
            joueurs[i].cartes_possedees[valeur_carte] = 1; // on enregistre que le joueur possède désormais cette carte
            joueurs[i].nb_cartes_differentes++; // on augmente le nombre de cartes différentes possédées par le joueur
            
            joueurs[i].score_manche += valeur_carte; // On ajoute les points normaux de la carte ! 

            //2.Condition du Flip 7 ( 7 cartes différentes ) pour terminer la manche immédiatement + 15 points bonus
            if (joueurs[i].nb_cartes_differentes == 7) {
                printf(VERT "🎉 Bravo %s ! Tu réalise un Flip 7 et gagnes 15 points supplémentaires ! 🎉\n" RESET, joueurs[i].nom);
                *manche_en_cours = false; // La manche se termine immédiatement
            }
        // 3. Si le joueur possède déjà la carte piochée (doublon) , il perd la manche 
        } else {
            // --- GESTION SECONDE CHANCE SI POSSÉDÉE --- 
            if(joueurs[i].a_2nd_chance) {
                printf(JAUNE "🛡️  %s pioche un doublon de %d, mais sa SECONDE CHANCE le sauve 🛡️ !\n" RESET, joueurs[i].nom, valeur_carte);
                joueurs[i].a_2nd_chance = false; // La carte est utilisée
            } else {
                printf(ROUGE "❌ %s a pioche un doublon de carte %d et perd la manche ❌  !\n" RESET, joueurs[i].nom, valeur_carte);
                joueurs[i].score_manche = 0; // Il perd les points de la manche 
                joueurs[i].a_perdu_manche = true; // Le joueur perd la manche
            }
        }

    // Si la carte piochée est une carte spéciale, on applique son effet 
    } else if (carte_piochee.type == CARTE_SPECIAL) {
        printf(BLEU "\n ⚡ %s a pioché une carte spéciale : %s  ⚡ ! \n" RESET, joueurs[i].nom, carte_piochee.nom);
        
        //APPEL à la fonction des cartes spéciales
        appliquer_carte_speciale(joueurs, nb_joueurs, i, carte_piochee, paquet, index_pioche, stats_cartes  , taille_paquet   );
        
    // Ajout du traitement des Bonus 
    } else if (carte_piochee.type == CARTE_BONUS) {
        printf(JAUNE "\n*** ⭐ %s a pioché un bonus : %s ! ***\n" RESET, joueurs[i].nom, carte_piochee.nom);
        if (valeur_carte == 2 && carte_piochee.nom[6] == 'x') {
            joueurs[i].multiplicateur = 2; 
        } else {
            joueurs[i].bonus_addition += valeur_carte; 
        }
    }
}

// --- 4ème étape : fonction pour vérifier la fin de la partie ---
void verifier_fin_partie(Joueur joueurs[], int nb_joueurs, double temps_ecoule, ConfigurerPartie maConfig, int index_pioche, bool *fin_de_partie) {
    bool joueur_a_gagne = false;
    for (int i = 0; i < nb_joueurs; i++) {
        if (joueurs[i].score_partie >= 200) {
            printf(VERT "🏆 Félicitations %s  🏆 ! Tu as atteint 200 points et gagné la partie ! 🏆\n" RESET, joueurs[i].nom);
            joueur_a_gagne = true;
            break;
        }
    }

   //1. Utilisation de maConfig.temps_limite_secondes
    if (joueur_a_gagne == true || temps_ecoule >= maConfig.temps_limite_secondes || index_pioche >= maConfig.nb_total_cartes) {
        *fin_de_partie = true;
        printf(JAUNE "\n🚩 La partie est terminée 🚩  !\n" RESET);

        if (temps_ecoule >= maConfig.temps_limite_secondes) {  
            printf("⏳ Le temps de jeu est écoulé ⏳   !\n");
        } else if (index_pioche >= maConfig.nb_total_cartes) {
            printf("🎴 La pioche de cartes est épuisée 🎴 !\n");
        }
    }
}

// --- 5ème étape : fonction pour réinitialiser les données des joueurs pour une nouvelle manche ---
void reinitialiser_pour_nouvelle_manche(Joueur joueurs[], int nb_joueurs) {
    for (int i = 0; i < nb_joueurs; i++) {
        joueurs[i].score_manche = 0;
        joueurs[i].nb_cartes = 0;
        joueurs[i].nb_cartes_differentes = 0;
        joueurs[i].bonus_addition = 0;
        joueurs[i].multiplicateur = 1;
        joueurs[i].a_perdu_manche = false;
        joueurs[i].est_sortie_manche = false;
        joueurs[i].a_2nd_chance = false;
        for (int j = 0; j < 13; j++) joueurs[i].cartes_possedees[j] = 0;
    }
}


