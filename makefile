#1. Je définis le compilateur, les options d'erreur et le nom de l'exécutable.
CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = fliptech

# 2. Je liste les sources (.c) pour générer la liste des objets (.o).
SRCS = main.c cartes.c moteur_jeu.c sauvegarde.c
OBJS = $(SRCS:.c=.o)

# 3. J'assemble tous mes fichiers objets pour créer mon exécutable final.

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# 4. Je compile chaque .c en .o avec Fliptech.h comme dépendance.
%.o: %.c Fliptech.h
	$(CC) $(CFLAGS) -c $< -o $@

# 5. Je définis une commande pour supprimer les fichiers compilés (les .o et l'exécutable) et repartir à zéro.

clean:
ifeq ($(OS),Windows_NT) #pour système windows
	@echo "Suppression des fichiers objets et de l'executable"
	@del /f /q *.o $(TARGET).exe 2>nul

#pour système linux	
else
	@echo "Suppression des fichiers objets et de l'exécutable"
	@rm -f $(OBJS) $(TARGET) 
endif
