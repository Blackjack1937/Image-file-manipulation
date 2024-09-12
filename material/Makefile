#######################################
# Unified Makefile for PBM, PGM, PPM  #
#                                     #
#######################################

# Programs
PROGS = pgmtopgm 3-image-conversion ppmtopgm

# Variables for file compilation
CC        =  gcc
CFLAGS    =  -g -Wall
CPPFLAGS  =  -DDEBUG
LDFLAGS   =  -g -lm

# Object files
OBJFILES = Util.o

# Default target: compile all programs
all: $(PROGS)

# Rule to compile pgmtopgm
pgmtopgm: pgmtopgm.o $(OBJFILES)
	$(CC) $(LDFLAGS) -o pgmtopgm pgmtopgm.o $(OBJFILES)

pgmtopgm.o: pgmtopgm.c Util.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c pgmtopgm.c

# Rule to compile 3-image-conversion
3-image-conversion: 3-image-conversion.o $(OBJFILES)
	$(CC) $(LDFLAGS) -o 3-image-conversion 3-image-conversion.o $(OBJFILES)

3-image-conversion.o: 3-image-conversion.c Util.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c 3-image-conversion.c

# Rule to compile ppmtopgm
ppmtopgm: ppmtopgm.o $(OBJFILES)
	$(CC) $(LDFLAGS) -o ppmtopgm ppmtopgm.o $(OBJFILES)

ppmtopgm.o: ppmtopgm.c Util.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c ppmtopgm.c

# Rule to compile Util.o (shared utility functions)
Util.o: Util.c Util.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c Util.c

# Clean up object files
clean:
	@rm -f *.o

# Clean all object files and executables
cleanall: clean
	@rm -f $(PROGS)
