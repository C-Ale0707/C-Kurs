/** \file fir_filter_v1.h
 *
 *  \brief Headerfile mit Datenstruktur und Zugriffsfunktionen fuer FIR-Filter
 *
 *  \details In diesem Header wird die Datenstruktur `struct fir_filter_variablen` definiert,
 *  welche interne Groessen des FIR-Filters aufnimmt. Ausserdem werden Funktionsprototypen
 *  bereitgestellt fuer den Konstruktor, den Destruktor, eine Funktion zur Eingabe der Filterkoeffizienten
 *  sowie eine Funktion zur Berechnung des Filters.
 *
 *  \author Ulf Schaper
 *  \date 2012-05-11
 */
#include "matrixmath.h"

/** \brief Das Enum `fiterlib_return_values` listet die moeglichen Rueckgabewerte
 *  der Funktionen der fir_filter_lib auf.
 */
enum fiterlib_return_values {
   RET_SUCCESS,   /**< `RET_SUCCESS` signalisiert einen korrektes Funktionsverhalten */
   RET_NULLPOINTER, /**< `RET_NULLPOINTER` deutet darauf hin, dass einer Funktion ein Nullpointer uebergeben wurde. */
   RET_MEMORYERROR, /**< `RET_MEMORYERROR` weisst auf einen Speicherfehler hin, z.B. bei der Speicheralloziierung. */
   RET_GENERALERROR  /**< `RET_GENERALERROR` deutet einen nicht weiter spezifizierten Fehler an. */
  };


/** \brief Die Struktur `struct fir_filter_variablen` enthaelt die internen Variablen
 *  eines FIR-Filters.
 */
struct fir_filter_variablen {
   int n; /**< `n` speichert die Ordnung der internen Matrizen/Vektoren. Achtung, die Filterlaenge ist immer n+1. */
   struct matrix * A; /**< `A` speichert die interne Systemmatrix */
   struct matrix * B; /**< `B` speichert den internen Eingangsvektor */
   struct matrix * C; /**< `C` speichert den internen Ausgangsvektor */
   struct matrix * D; /**< `D` speichert die interne Durchgriffsmatrix */
   struct vector * x; /**< `x` speichert den Filterzustand in Form von zeitverzoegerten Eingangsgroessen */
   struct vector * x_temp1; /**< `x_temp1` wird bei der Berechnung der Filtergleichung verwendet. */
   struct vector * x_temp2; /**< `x_temp2` wird bei der Berechnung der Filtergleichung verwendet. */
   struct vector * u; /**< nach `u` wird der Filtereingang geschrieben */
   struct vector * y_temp; /**< Die Variable `y_temp` wird bei der Auswertung der Filterausgangsgleichung benutzt. */
};


int alloc_fir_filter_variablen_struct(struct fir_filter_variablen* pfir_filter_variablen, int iDimension);
void free_fir_filter_variablen_struct(struct fir_filter_variablen* pfir_filter_variablen);
int setze_koeffizienten(struct fir_filter_variablen* pfir_filter_variablen, double * aKoeffs);
int filterausgabe_berechnen(struct fir_filter_variablen* pfir_filter_variablen, double eingang, double * ausgang);
