#
# Makefile für statische Bibliotheken
#
# Name der Bibliothek
LIBNAME = fir_filter_v1
# Name des Archivs erzeugen
TARGET = lib$(LIBNAME).a
TARGET_DIR = release
# Header-Datei der Bibliothek
HEADER = $(LIBNAME).h
# zu kompilierende Source Dateien
# Beachte: keine Leerzeichen nach \\
CSRC = 	fir_filter_v1.c
		
# Suchpfade für Header-Dateien
INCDIRS = include
# Suchpfade für Bibliotheken
LIBDIRS = lib

# Compiler Flags zentral definieren
# -Wall   : bei allen Warnungen abbrechen
# -Wextra : besonders pingelig warnen
# -g3     : Maximalmenge an Debug-Infos in die .exe packen
# -m32    : 32bit .exe erzeugen (Eclipse mag den 64bit GDB nicht)
LDFLAGS = -Wall -Wextra -g3
CFLAGS = -Wall -Wextra -g3
# Flags für Archivprogramm definieren
# r : Vorherige Version ersetzen
# v : Ausführliche Ausgabe in Konsole (verbose)
# s : erzeugt eine Symboltabelle, die von gcc benötigt wird
ARFLAGS = rvs


# Wahl der gcc Version (Name der .exe)
CC = gcc
# Wahl des Archivierungsprogramms
AR = ar
# Namen der .o Dateien aus den .c-Dateien erzeugen
COBJ = $(subst .c,.o,$(CSRC))

# inc-Verzeichnisse um -I Option ergänzen
INC_PARAMS = $(foreach d, $(INCDIRS), -I$d)
# erzeuge Archiv
all: $(TARGET)

# packe alle .o-Dateien ins Archiv im lib-Ordner und kopiere die Header-Datei 
# in den include-Ordner, falls sie neuer als die vorhandene Version ist
$(TARGET): $(COBJ)
	mkdir -p $(TARGET_DIR)
	$(AR) rvs $(TARGET_DIR)/$@ $^  
	cp -uf $(HEADER) $(TARGET_DIR)/$(HEADER)	
# erzeuge alle .o Dateien
%.o: %.c
	$(CC) $(CFLAGS) $(INC_PARAMS) $(LIBDIR_PARAMS) $(LIB_PARAMS) $< -c -o $@

# all und clean sind keine Datei-Ziele
.PHONY: all clean

# alles wieder aufräumen
clean:
	rm -rf $(COBJ) $(TARGET_DIR)/$(TARGET) $(TARGET_DIR)/$(HEADER) 