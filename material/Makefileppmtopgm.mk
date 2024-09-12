#######################################
# Makefile pour ppmtopgm    #
#                                     #
#######################################

# Nom du programme
PROG = ppmtopgm

# Variables de compilation
CC        = gcc
CFLAGS    = -g -Wall
CPPFLAGS  = -DDEBUG
LDFLAGS   = -g -lm

# Liste des fichiers objets à générer
OBJFILES = ppmtopgm.o Util.o

# Cible principale (par défaut)
all: $(PROG)

# Règle pour construire le programme final
$(PROG): $(OBJFILES)
	$(CC) $(LDFLAGS) -o $(PROG) $(OBJFILES)

# Règle pour construire 3-image-conversion.o
ppmtopgm.o: ppmtopgm.c Util.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c ppmtopgm.c

# Règle pour construire util.o (fichier contenant les fonctions utilitaires)
Util.o: Util.c Util.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c Util.c

# Nettoyage des fichiers objets
clean:
	@rm -f *.o

# Nettoyage des fichiers objets et de l'exécutable
cleanall: clean
	@rm -f $(PROG)
