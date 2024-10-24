# Variables
CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -L. -lseg
LIBS = libseg.a
TARGET = client
SRC = main.c

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

# Phony pour éviter des conflits si des fichiers s'appellent clean ou all
.PHONY: all clean cleanall
