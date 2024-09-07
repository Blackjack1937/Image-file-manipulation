#######################################
# Makefile pour 3-image-conversion    #
#                                     #
#######################################

# Nom du programme
PROG = 3-image-conversion

# Variables de compilation
CC        = gcc
CFLAGS    = -g -Wall
CPPFLAGS  = -DDEBUG
LDFLAGS   = -g -lm

# Liste des fichiers objets à générer
OBJFILES = 3-image-conversion.o Util.o

# Cible principale (par défaut)
all: $(PROG)

# Règle pour construire le programme final
$(PROG): $(OBJFILES)
	$(CC) $(LDFLAGS) -o $(PROG) $(OBJFILES)

# Règle pour construire 3-image-conversion.o
3-image-conversion.o: 3-image-conversion.c Util.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c 3-image-conversion.c

# Règle pour construire util.o (fichier contenant les fonctions utilitaires)
Util.o: Util.c Util.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c Util.c

# Nettoyage des fichiers objets
clean:
	@rm -f *.o

# Nettoyage des fichiers objets et de l'exécutable
cleanall: clean
	@rm -f $(PROG)
