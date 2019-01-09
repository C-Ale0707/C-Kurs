
#include "mdlSim.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <coeff_iir.h>
#include <iir_filter_v1.h>

#define IN_FILE "sollverlauf.csv"
#define OUT_FILE "ergebnis2.csv"

//Definiere das Struct für die einzulesenden Daten:
typedef struct{
	double t; //Zeit-Wert
	double x; //X-Wert (Zustandswert des Systems)
}point_t;

int pid_regler(double* pdU, double dtStep, double dSoll, double dIst, double dKp, double dKi, double dKd);

int main(void)
{
	/**********************************************/
	/* Schrittweise Simulation                    */
	/**********************************************/

	void * pvSys=NULL;
	double 		 dY   	= 0.0;
	double 		 dU   	= 0.0;
	const double cdtEnd = 10;		// Endzeit der Simulation
	double		 dt     = 0;		// aktuelle Simulationszeit in Sekunden
	double		 dtStep = 0;		// Schrittweite des letzten Schrittes in Sekunden

	
	// double dKp = 950;
	// double dKi = 1500;
	// double dKd = 1100;

	//double dKp = 2320;
	//double dKi = 0.01;
	//double dKd = 1000;

	double dKp = 355.0;
	double dKi = 555.0;
	double dKd = 155.0;


	//Wichtige Variablen für die Verarbeitung der CSV-Dateien:

	FILE *fp;						// Pointer auf eine Datei
	unsigned int i, point_count;
	point_t *points;
	char line[500];
	char *cptr;



	// Initialisiert das Modell
	pvSys=initSys();
	
	// Lese Soll-Daten ein:

	fp = fopen(IN_FILE, "r");

	//Fehlermeldung und terminieren der Prozesse bei Problemen mit der Datei
	if(fp == NULL){
		perror("Error while opening the input file.\n");
		exit(EXIT_FAILURE);
	}

	//Verarbeite die CSV-Datei:

	point_count = 0;
	while(fgets(line, 500,fp)!= NULL){
		point_count++;
	}
	rewind(fp); //Zeiger zurücksetzen

	points = malloc(point_count*sizeof(point_t));

	i = 0;
	while(fgets(line,500,fp) != NULL){
		cptr = strtok(line,","); // Wollen nur den Wert vor dem Komma lesen, daher ists so zu machen
		points[i].t = strtod(cptr,NULL);
		cptr = strtok(NULL,"\n");
		points[i].x = strtod(cptr,NULL);
		i++;

	}

	fclose(fp);


	double aFilterkoeffizienten[] = { NENNER_COEFF_VECTOR };
	double bFilterkoeffizienten[] = { ZAEHLER_COEFF_VECTOR };

	// double aFilterkoeffizienten[] = { 0.000242904903433892, 0.000485809806867785, 0.000242904903433892, 1, -1.96810331125610, 0.969074930869833 };
	// double bFilterkoeffizienten[] = { 0.0154662914031034, 0.0154662914031034, 0, 1, -0.969067417193793, 0 };

	int n;
	struct iir_filter_variablen * piir_filter_variablen;

	/**  Einlesen der Testdaten aus einer CSV-Datei */
	/**  Anschliessend wird die Filterlaenge ausgelesen.
	 */
	n = (sizeof(aFilterkoeffizienten) / sizeof(aFilterkoeffizienten[0])) - 1;
	/** Es wird eine Instanz von der Datenstruktur des IIR-Filters angelegt
	 *  und ein Pointer darauf in `piir_filter_variablen` gespeichert.
	 */
	piir_filter_variablen = (struct iir_filter_variablen *) malloc(
			sizeof(struct iir_filter_variablen));
	if (!piir_filter_variablen) {
		perror("Error allocating piir_filter_variablen.\n");
		exit(EXIT_FAILURE);
	}
	/** Anschliessend wird die Struktur initialisiert.
	 */
	if (alloc_iir_filter_variablen_struct(piir_filter_variablen, n)
			!= RET_SUCCESS) {
		/** Wenn `alloc_iir_filter_variablen_struct` einen Rueckgabewert
		 *  ungleich 0 hat, dann ist ein Fehler aufgetreten.
		 *   In dem Fall muss der verwendete Speicher wieder freigegeben werden,
		 *   bevor das Programm beendet wird.
		 */
		free(piir_filter_variablen);
		perror("Error allocating piir_filter_variablen struct.\n");
		exit(EXIT_FAILURE);
	}
	/** Danach werden die Filterkoeffizienten aus unserem lokalen Array
	 *  an den Filter uebergeben.
	 */
	if (setze_koeffizienten(piir_filter_variablen, aFilterkoeffizienten,
			bFilterkoeffizienten) != RET_SUCCESS) {
		free(piir_filter_variablen);
		perror("Error initializing coefficients.\n");
		exit(EXIT_FAILURE);
	}



	fp = fopen(OUT_FILE,"w");
	//Fehlermeldung bei möglichen Problemen mit der Datei
	if(fp == NULL){
		perror("Error while opening the output file.\n");
		exit(EXIT_FAILURE);
	}

	int k = 0;
	double hilf;
	int syms = 0;
	double dSpeicher = 0;
	// simuliert das Modell für "cdtEnd" Sekunden
	while(dt < cdtEnd)
	{	
		// Regler aufrufen
		//----------------------------------------------------------------------
		// int success;
		// success = iPID(&dU,dY,dtStep,...,&datastruct);
		// ...
		
		// System simulieren:
		//----------------------------------------------------------------------

		int success;
		success = pid_regler(&dU, dtStep, points[k].x, dY, dKp, dKi, dKd);

		if (success == EXIT_FAILURE){
			dU = dSpeicher;
		}
		dSpeicher = dU;
		// ############ nachfolgenden Code nicht ändern ################
		dtStep = 0;	// aktuelle Schrittweite resetten
		do {
			// einen Schritt simulieren (Schrittweite ist hier 0.01s)
			stepSys(pvSys,dU,&dY);
			
			// Schrittweitenvariable aktualisieren
			dtStep = dtStep + 0.01;		
			
			// per Zufall schauen ob direkt noch ein Schritt simuliert wird
		} while( (double)rand()/(double)RAND_MAX < 0.05 );
		dt = dt + dtStep;
		// ############ voranstehenden Code nicht ändern ###############
		//
		// in realen Steuergeräten kann es sein, dass der Regler für einen
		// Zyklus nicht ausgeführt wird. Bei Simulink-Modellen gibt es eine 
		// Schrittweitensteuerung. Beides wird mit einem gewissen Zufall in der 
		// Schrittweite hier simuliert.
		//----------------------------------------------------------------------
		
		syms = dtStep/0.01;
		hilf = dY;
		filterausgabe_berechnen(piir_filter_variablen, hilf, &dY);
		// printf("\nt=%4.2f Y= %6.2f",dt,dY);
		fprintf(fp,"%f,\t%f,\t%3.2f\n",dt,points[k].x,dY);
		fflush(stdout);
		k = k + 1;
		printf("%f,", hilf-dY);
	}
			
	// Gibt Speicher frei

	free_iir_filter_variablen_struct(piir_filter_variablen);
	free(piir_filter_variablen);

	freeSys(pvSys);

	fclose(fp);

	//////////////////Rest freen
	return EXIT_SUCCESS;
}



