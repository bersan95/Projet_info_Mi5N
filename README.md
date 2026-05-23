# PROJET FLIPTECH - CY TECH (pré-ing1)
-----------------------------------
Groupe:MI5_N

Auteurs : Bersan Sayhan et Ould el mochtar Iman

## FICHIERS INCLUS
Le projet est modulé et structuré autour des fichiers suivants :
- Fliptech.h    : Fichier d'en-tête contenant les structures et prototypes.
- main.c        : Cœur du programme 
- cartes.c      : Initialisation et mélange du paquet 
- deroulement.c : La logique des tours, conditions de victoire et cartes spéciales.
- affichage.c   : Interface utilisateur, statistiques et résultats.
- ordinateur.c  : Création d'un ou des bots pour les parties solo/mixtes.
- sauvegarde.c  : Exportation des scores de fin de partie.
- makefile      : Pour compilation automatique.


 ## Compilation 

  Pour le gcc :      gcc main.c cartes.c deroulement.c sauvegarde.c affichage.c ordinateur.c -o fliptech

  Sinon compilation automatique avec la commande :     make


## EXECUTION
Une fois la compilation terminée, l'exécutable 'fliptech' est généré. Lancez le jeu avec la commande suivante :

- Sous Linux / macOS :    ./fliptech

- Sous Windows :          .\fliptech


## NETTOYAGE
Pour nettoyer votre dossier et supprimer les fichiers objets (.o) ainsi que l'exécutable générés lors de la compilation, utilisez la commande : 

make clean



## UTILISATION ET FONCTIONNALITÉS
Toute la configuration se fait de manière interactive et sécurisée au lancement :

1. Configuration de la partie : Choix entre le mode Standard (20 minutes, 94 cartes= 85 cartes numérotées+6 cartes bonus+ 9 cartes spéiales ) ou le mode Personnalisé (durée et cartes numéros ajustables ).
2. Paramétrage des joueurs : Saisie du nombre de joueurs, les pseudos et possibilité d'ajouter des ordinateurs (Bots).
3. Déroulement d'un tour : Saisir 1 pour piocher, 2 pour vous arrêter, ou 3 pour consulter  les cartes que vous possèdées et vos statistiques
4. Fin de partie : Le classement s'affiche automatiquement (par le tri à bulles) et propose de sauvegader les résultats dans un fichier texte au choix par oui ou non .Si oui , saisir le nom du fichier 


  
