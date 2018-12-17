#include "matrixmath.h"
#include <iir_filter_v1.h>

/** \mainpage
 *  Die iir_filter_lib implementiert einen iir-Filter. Dabei handelt es sich
 *  um Filter mit endlicher Impulsantwort (englisch: finite impulse response, iir).
 *
 *  \f$G(z) = \frac{Y(z)}{U(z)} =  a_0 + a_1\,z^{-1} + a_2\,z^{-2} + \ldots + a_n\,z^{-n} \f$
 *
 *  Zur Konfiguration des Filters muessen die Koeffizienten der Impulsantwort \f$a_i\f$
 *  uebergeben werden. Zur Laufzeit des Filters wird dann das Eingangssignal
 *  mit der Impulsantwort gefaltet, und so die Filterausgabe berechnet.
 *
 *  Das Filter ist intern mit einem Zustandsraummodell implementiert, bei dem
 *  die Zustaende den zeitverzoegerten Eingangssignalen entsprechen:
 *
 *  \f$x(k+1) = A\,x(k) + B\,u(k)\f$
 *
 *  \f$ y(k) = C\,x(k) + D\,u(k) \f$
 *
 *  mit \f$A = \begin{pmatrix} 0 & 1 & 0 & \ldots & 0 \\ & & \ddots &  & \\  & & & 0 & 1 \\ 0 & & \ldots & & 0 \end{pmatrix} \f$,
 *      \f$B = \begin{pmatrix} 0 \\ \vdots \\ 0 \\ 1 \end{pmatrix} \f$
 *
 *  und \f$C = \begin{pmatrix} a_n & \ldots & a_2 & a_1 \end{pmatrix} \f$,
 *      \f$D = \begin{pmatrix} a_0 \end{pmatrix} \f$.
 *
 *  Der Benutzer muss eine Instanz von `struct iir_filter_variablen` anlegen und
 *  den Bibliotheksfunktionen jeweils einen Pointer darauf uebergeben.
 *
 *  Es muessen vier Funktionen aufgerufen werden:
 *  1. Zunaechst muss mit der Funktion `alloc_iir_filter_variablen_struct()`
 *     die interne Datenstruktur initialisiert werden.
 *  2. Danach koennen mit der Funktion `setze_koeffizienten()` die Filterkoeffizienten
 *     (d.h. die Impulsantwort) uebergeben werden.
 *  3. Anschliessend wird das Eingangssignal mit der Funktion `filterausgabe_berechnen()`
 *     schrittweise uebernommen und der jeweilige Ausgang berechnet. Diese Funktion
 *     kann beliebig oft ausgefuehrt werden.
 *  4.  Zuletzt kann ueber `free_iir_filter_variablen_struct()` der vorher reservierte Speicher
 *     wieder freigegeben werden.
 *
 */

/** \brief Diese Funktion initialisiert eine Instanz von `struct iir_filter_variablen`.
 *
 * Dabei wird Speicher fuer die internen
 * Matrizen und Vektoren definiert. Anschliessend werden die konstanten Matrizen \f$A\f$ und \f$B\f$
 * gefuellt und der Anfangszustand genullt.
 *
 * \param[in, out] piir_filter_variablen Pointer auf die zu initialisierende Datenstruktur
 * \param[in] iDimension steuert, wieviel Speicher reserviert werden muss. `iDimension` = Koeffizientenanzahl - 1.
 * \return RET_SUCCESS bei erfolgreicher Ausfuehrung.
 * \return RET_NULLPOINTER falls ein unzulaessiger Nullpointer uebergeben wurde.
 * \return RET_MEMORYERROR bei fehlgeschlagener Speicherreservierung.
 */
int alloc_iir_filter_variablen_struct(struct iir_filter_variablen* piir_filter_variablen, int iDimension)
{
	/*  Pruefung, ob gueltiger Pointer uebergeben wurde.
	 */
	if (!piir_filter_variablen)
		return RET_NULLPOINTER;

	/*  Aufbauen der internen Datenstruktur
	 */
	piir_filter_variablen->n = iDimension;
	piir_filter_variablen->A =       get_mat(iDimension, iDimension);
	piir_filter_variablen->B =       get_mat(iDimension, 1         );
	piir_filter_variablen->C =       get_mat(1,          iDimension);
	piir_filter_variablen->D =       get_mat(1,          1         );
	piir_filter_variablen->x =       get_vec(iDimension);
	piir_filter_variablen->x_temp1 = get_vec(iDimension);
	piir_filter_variablen->x_temp2 = get_vec(iDimension);
	piir_filter_variablen->u =       get_vec(1);
	piir_filter_variablen->y_temp =  get_vec(1);

	/* Pruefung, ob Fehler aufgetreten ist.
	 * In dem Fall Speicher wieder freigeben
	 */
	if (      (!piir_filter_variablen->A)
		  ||  (!piir_filter_variablen->B)
		  ||  (!piir_filter_variablen->C)
		  ||  (!piir_filter_variablen->D)
		  ||  (!piir_filter_variablen->x)
		  ||  (!piir_filter_variablen->x_temp1)
		  ||  (!piir_filter_variablen->x_temp2)
		  ||  (!piir_filter_variablen->u)
		  ||  (!piir_filter_variablen->y_temp)     )
	{
		free_iir_filter_variablen_struct(piir_filter_variablen);
		return RET_MEMORYERROR;
	}

	/* Die Matrizen A und B sind konstant.
	 * Sie werden in der folgenden Output-Funktion fuer ein Zustandsupdate
	 * benuzt, d.h. fuer die Auswertung von \f$x(k+1) = A\,x(k) + B\,u(k)\f$.
	 * An dieser Stelle werden sie beschrieben.
	 */
	{
		int i;
		zero_matrix(piir_filter_variablen->B);
		zero_matrix(piir_filter_variablen->A);

		/* Der Eingangswert wird immer in den letzten Zustand geschrieben:
		 */
		matrix_setval(piir_filter_variablen->B, 0, 0, 1.0);

		/* Alle Zustaende bis auf den ersten werden ein Element weiter
		 * nach vorne kopiert:
		 */
		for (i=1; i<iDimension; i++)
		{
			matrix_setval(piir_filter_variablen->A, i, i-1, 1.0);
		}

	}

	/*  Der Zustand wird mit Null initialisiert.
	 */
	zero_vector(piir_filter_variablen->x);

	return RET_SUCCESS;
}


/** \brief Diese Funktion gibt den intern verwendeten Speicher wieder frei.
  * \param[in, out] piir_filter_variablen Pointer auf die freizugebende Datenstruktur
 */
void free_iir_filter_variablen_struct(struct iir_filter_variablen* piir_filter_variablen)
{
	free_mat(piir_filter_variablen->A);
	free_mat(piir_filter_variablen->B);
	free_mat(piir_filter_variablen->C);
	free_mat(piir_filter_variablen->D);
	free_vec(piir_filter_variablen->x);
	free_vec(piir_filter_variablen->x_temp1);
	free_vec(piir_filter_variablen->x_temp2);
	free_vec(piir_filter_variablen->u);
	free_vec(piir_filter_variablen->y_temp);
}


/** \brief Diese Funktion uebernimmt die Filterkoeffizienten (\f$a_0,\,a_1,\,\ldots,\,a_n\f$) in die internen Datenstrukturen
 * \details Bei einer Filterlaenge von \f$n\f$ muessen \f$n+1\f$ Koeffizienten eingelesen werden.
 * Der erste Koeffizient entspricht dabei der Gewichtung des aktuellen (=juengsten) Eingangssignals.
 * \param[in, out] piir_filter_variablen Pointer auf die zu beschreibende Datenstruktur
 * \param[in] aKoeffs Array, aus dem die Filterkoeffizienten eingelesen werden sollen
 * \return RET_SUCCESS bei erfolgreicher Ausfuehrung.
 */
int setze_koeffizienten(struct iir_filter_variablen* piir_filter_variablen, double * aKoeffs, double * bKoeffs)
{
	int i;

	if (!piir_filter_variablen)
		return RET_NULLPOINTER;
	if (!aKoeffs)
		return RET_NULLPOINTER;
	if (!aKoeffs)
		return RET_NULLPOINTER;

	matrix_setval(piir_filter_variablen->D, 0, 0, bKoeffs[0]);

	for (i=1; i<=piir_filter_variablen->n; i++)
	{
	   matrix_setval(piir_filter_variablen->C, 0, i-1, bKoeffs[i]-bKoeffs[0]*aKoeffs[i]);
	}

	for (i=1; i<=piir_filter_variablen->n; i++)
	{
	   matrix_setval(piir_filter_variablen->A, 0, i-1, -aKoeffs[i]);
	}

	return RET_SUCCESS;
}

/** \brief Die Funktion filterausgabe_berechnen() uebernimmt ein neues Eingangssignal und fuehrt einen Filterschritt durch.
 * \details Dazu wird die Ausgangsgleichung berechnet und anschliessend ein Zustandsupdate ausgefuehrt.
 * \param[in, out] piir_filter_variablen Pointer auf die zu verwendende Datenstruktur
 * \param[in] eingang neuer Wert des Eingabesignals
 * \param[out] ausgang Pointer auf die Speicherstelle, auf das Filterergebnis geschrieben werden soll.
 * \return RET_SUCCESS bei erfolgreicher Ausfuehrung.
 * \return RET_NULLPOINTER falls ein unzulaessiger Nullpointer uebergeben wurde.
 */
int filterausgabe_berechnen(struct iir_filter_variablen* piir_filter_variablen, double eingang, double * ausgang)
{
	/* Es wird geprueft, ob eine gueltige `struct iir_filter_variablen` uebergeben wurde.
	 */
	if (!piir_filter_variablen)
		return RET_NULLPOINTER;

	/* Anschliessend wird der aktuelle Filtereingang eingelesen.
	 */
	vector_setval(piir_filter_variablen->u, 0, eingang);

	/* Dann wird die aktuelle Ausgangsgleichung y = C*x + D*u berechnet
	 * und das Ergebnis nach *ausgang geschrieben.
	 */
	{
		double dtemp_ausgang1;
		double dtemp_ausgang2;

		/* Berechnung von C*x
		 */
		mv_mult(piir_filter_variablen->C, piir_filter_variablen->x, piir_filter_variablen->y_temp);
		vector_getval(piir_filter_variablen->y_temp, 0, &dtemp_ausgang1);

		/* Berechnung von D*u
		 */
		mv_mult(piir_filter_variablen->D, piir_filter_variablen->u, piir_filter_variablen->y_temp);
		vector_getval(piir_filter_variablen->y_temp, 0, &dtemp_ausgang2);

		/* Summation y = C*x + D*u
		 */
		*ausgang = dtemp_ausgang1 + dtemp_ausgang2;
	}

	/* Zuletzt wird die Zustandsgleichung durchgerechnet.
	 * Sie hat die Form x(k+1) = A*x(k) + B*u(k), d.h. wir berechnen
	 * den Zustand fuer den naechsten Zeitschritt.
	 * Die Gleichung wird in zwei Schritten berechnet.
	 * Anschliessend wird der neue Zustand x_{neu}, d.h. x(k+1), auf den alten
	 * Zustandsspeicher `x` kopiert, damit er beim naechsten Aufruf
	 * dann als alter Zustand zur Verfuegung steht.
	 */
	{
		mv_mult(piir_filter_variablen->A, piir_filter_variablen->x, piir_filter_variablen->x_temp1); /* x_temp1 = A*x */
		mv_mult(piir_filter_variablen->B, piir_filter_variablen->u, piir_filter_variablen->x_temp2); /* x_temp2 = B*u */
		vv_add(piir_filter_variablen->x_temp1, piir_filter_variablen->x_temp2, piir_filter_variablen->x);  /* x = x_temp1 + x_temp2 */
	}

	return RET_SUCCESS;

}
