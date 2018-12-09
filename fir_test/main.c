#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fir_filter_v1.h>

#define IN_FILE "csv_data.txt"
#define OUT_FILE "csv_result.txt"

typedef struct{
	double t; //Zeit-Wert
	double x; //X-Wert

}point_t;

int main(){

	//double aFilterkoeffizienten[] = {0.2,0.2,0.2,0.2,0.2};
	//Neue Coeffs aus der coeffs.txt
	double aFilterkoeffizienten[] = {0.000000,0.001327,0.004650,0.011750,0.023779,0.040723,0.061157,0.082404,0.101082,0.113897,0.118460,0.113897,0.101082,0.082404,0.061157,0.040723,0.023779,0.011750,0.004650,0.001327};
	int n;

	struct fir_filter_variablen * pfir_filter_variablen;

	FILE *fp;
	unsigned int i, point_count;
	point_t *points;
	char line[80];
	char *cptr;

	//Filterordnung
	n= (sizeof(aFilterkoeffizienten)/sizeof(aFilterkoeffizienten[0]))-1;

	pfir_filter_variablen = (struct fir_filter_variablen *)malloc(sizeof(struct fir_filter_variablen));

	if(!pfir_filter_variablen){

		perror("Error allocating pfir_filter_variablen: \n");
		exit(EXIT_FAILURE);

	}

	//Struktur initialisiert
	if(alloc_fir_filter_variablen_struct(pfir_filter_variablen,n) != RET_SUCCESS){

		//Speicher wieder freigeben
		free(pfir_filter_variablen);
		perror("Error allocating pfir_filter_variablen struct. \n");
		exit(EXIT_FAILURE);
	}

	// Koeffizienten setzen

	if(setze_koeffizienten(pfir_filter_variablen, aFilterkoeffizienten)!= RET_SUCCESS){

		free(pfir_filter_variablen);
		perror("Error initializing coefficients. \n");
		exit(EXIT_FAILURE);
	}

	//Öffne Datei (csv_data.txt)

	fp = fopen(IN_FILE, "r"); //read-only
	if(fp == NULL){
		//Notiz: In den meisten Fällen fehlt diese Datei daher verweißt der Pointer auf eine NULL
		perror("Error while opening the input file. \n");
		exit(EXIT_FAILURE);
	}

	point_count = 0;
	while(fgets(line, 80,fp)!= NULL){
		point_count++;
	}
	rewind(fp); //Zeiger zurücksetzen

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
			free_fir_filter_variablen_struct(pfir_filter_variablen);
			free(pfir_filter_variablen);
			perror("Error while opening the output file. \n");
			exit(EXIT_FAILURE);
		}

		fprintf(fp,"k\tu\ty\n");

		for(k=0; k<iSignallaenge;k++){

			//Eingang = 0 für k<10, Eingang=1 für k>=10
			//dEingangssignal = (k<10) ? 0.0 : 1.0;
			dEingangssignal = points[k].x; //Momentane Eingangswert
			// filtern
			filterausgabe_berechnen(pfir_filter_variablen, dEingangssignal,&dAusgangssignal);

			fprintf(fp,"%f,\t%f,\t%3.2f\n",points[k].t,dEingangssignal,dAusgangssignal);
			fflush(stdout);
		}

		//Datei schließen
		fclose(fp);



	}





	free_fir_filter_variablen_struct(pfir_filter_variablen);
	free(pfir_filter_variablen);









}
















