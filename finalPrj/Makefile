# Name der ausf�hrbaren Datei 
TARGET = program

# zu kompilierende Source Dateien
# Beachte: keine Leerzeichen nach \\
CSRC = 	finalPrj.c \
		lib_src/mdlSim.c \
		regler.c \
		iir_filter_v1.c
		
# Suchpfade f�r Header-Dateien
INCDIRS = include
# Suchpfade f�r Bibliotheken
LIBDIRS = lib_src
# Verwendete eigene Bibliotheken
LIBS = fir_filter_v1 matrixmath


# Compiler Flags zentral definieren
# -Wall   : bei allen Warnungen abbrechen
# -Wextra : besonders pingelig warnen
# -g3     : Maximalmenge an Debug-Infos in die .exe packen
# -m32    : 32bit .exe erzeugen (Eclipse mag den 64bit GDB nicht)
LDFLAGS = -Wall -Wextra -g3
CFLAGS = -Wall -Wextra -g3

# Wahl der gcc Version (Name der .exe)
CC = gcc

# Namen der .o Dateien aus den .c-Dateien erzeugen
COBJ = $(subst .c,.o,$(CSRC))

# inc-Verzeichnisse um -I Option erg�nzen
INC_PARAMS = $(foreach d, $(INCDIRS), -I$d)
# Bibliotheksverzeichnisse um -L Potion erg�nzen
LIBDIR_PARAMS = $(foreach d, $(LIBDIRS), -L$d)
# Bibliotheken um -l erg�nzen
LIB_PARAMS = $(foreach d, $(LIBS),-l$d)
# erzeuge ausf�hrbare Datei
all: $(TARGET)

# linke alle .o-Dateien
$(TARGET): $(COBJ)
	$(CC) $(LDFLAGS) $(INC_PARAMS) $(LIBDIR_PARAMS) $^ -o $@ $(LIB_PARAMS)

# erzeuge alle .o Dateien
%.o: %.c
	$(CC) $(CFLAGS) $(INC_PARAMS)  $< -c -o $@ 

# all und clean sind keine Datei-Ziele
.PHONY: all clean

# alles wieder aufr�umen
clean:
	rm -rf $(COBJ) $(TARGET)