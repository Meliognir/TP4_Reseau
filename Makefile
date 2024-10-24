# Variables
CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -L. -lseg
LIBS = libseg.a
TARGET = client
SRC = main.c
SERVER = segserv_2021

# Règle par défaut
all: $(TARGET)

# Compilation du programme final
$(TARGET): $(SRC) $(LIBS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

# Nettoyage des fichiers générés (utile pour repartir de zéro)
clean:
	rm -f $(TARGET)

# Nettoyage des fichiers objets intermédiaires si tu en as
cleanall: clean
	rm -f *.o

# Utilisation d'une librairie statique (facultatif si déjà compilée)
#libseg.a:
#ar rcs libseg.a seg1.o seg2.o 

# Phony pour éviter des conflits si des fichiers s'appellent clean ou all
.PHONY: all clean cleanall
