#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <coeff_iir.h>
#include <iir_filter_v1.h>

#define IN_FILE "csv_test_data.txt"
#define OUT_FILE "csv_result.txt"

typedef struct{
	double t; //Zeit-Wert
	double x; //X-Wert

}point_t;

int main(){

	int n;
	double aFilterkoeffizienten[] = {NENNER_COEFF_VECTOR};
	double bFilterkoeffizienten[] = {ZAEHLER_COEFF_VECTOR};

	struct iir_filter_variablen * piir_filter_variablen;

	FILE *fp;
	unsigned int i, point_count;
	point_t *points;
	char line[80];
	char *cptr;

	//Filterordnung
	n = (sizeof(aFilterkoeffizienten)/sizeof(aFilterkoeffizienten[0]))-1;

	piir_filter_variablen = (struct iir_filter_variablen *)malloc(sizeof(struct iir_filter_variablen));

	if(!piir_filter_variablen){

		perror("Error allocating piir_filter_variablen: \n");
		exit(EXIT_FAILURE);

	}

	//Struktur initialisiert
	if(alloc_iir_filter_variablen_struct(piir_filter_variablen,n) != RET_SUCCESS){

		//Speicher wieder freigeben
		free(piir_filter_variablen);
		perror("Error allocating piir_filter_variablen struct. \n");
		exit(EXIT_FAILURE);
	}

	// Koeffizienten setzen

	if(setze_koeffizienten(piir_filter_variablen, aFilterkoeffizienten, bFilterkoeffizienten)!= RET_SUCCESS){

		free(piir_filter_variablen);
		perror("Error initializing coefficients. \n");
		exit(EXIT_FAILURE);
	}

	//�ffne Datei (csv_data.txt)

	fp = fopen(IN_FILE, "r"); //read-only
	if(fp == NULL){
		//Notiz: In den meisten F�llen fehlt diese Datei daher verwei�t der Pointer auf eine NULL
		perror("Error while opening the input file. \n");
		exit(EXIT_FAILURE);
	}

	point_count = 0;
	while(fgets(line, 80,fp)!= NULL){
		point_count++;
	}
	rewind(fp); //Zeiger zur�cksetzen

	points = malloc(point_count*sizeof(point_t));

	i = 0;
	while(fgets(line,80,fp) != NULL){
		cptr = strtok(line,","); // Wollen nur den Wert vor dem Komma lesen, daher ists so zu machen
		points[i].t = strtod(cptr,NULL);
		cptr = strtok(NULL,"\n");
		points[i].x = strtod(cptr,NULL);
		i++;

	}

	fclose(fp);

	//Abfrage obs funktioniert hat noch implementieren...:

	{ //Ist eine Unterumgebung worin hier def. Vars nur in diesem Scope sichtbar sind

		double dEingangssignal;
		double dAusgangssignal;
		int iSignallaenge = point_count;
		int k;

		fp = fopen(OUT_FILE, "w");
		if(fp == NULL){
			free_iir_filter_variablen_struct(piir_filter_variablen);
			free(piir_filter_variablen);
			perror("Error while opening the output file. \n");
			exit(EXIT_FAILURE);
		}

		//fprintf(fp,"k\tu\ty\n");

		for(k=0; k<iSignallaenge;k++){

			//Eingang = 0 f�r k<10, Eingang=1 f�r k>=10
			//dEingangssignal = (k<10) ? 0.0 : 1.0;
			dEingangssignal = points[k].x; //Momentane Eingangswert
			// filtern
			filterausgabe_berechnen(piir_filter_variablen, dEingangssignal,&dAusgangssignal);

			fprintf(fp,"%f,\t%f,\t%3.2f\n",points[k].t,dEingangssignal,dAusgangssignal);
			fflush(stdout);
		}

		//Datei schlie�en
		fclose(fp);

	}


	free_iir_filter_variablen_struct(piir_filter_variablen);
	free(piir_filter_variablen);




}
















