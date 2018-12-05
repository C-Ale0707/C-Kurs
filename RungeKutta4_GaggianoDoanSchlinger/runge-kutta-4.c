#include <stdio.h>
#include <stdlib.h>
#include "matrixmath.h"

int rungeKutta4(struct matrix* pmA, struct vector* pvIC, const int ciT_sim, const double cdH){

	// Anzahl der Schritte errechnen
	const int ciSteps = ciT_sim/cdH;

	// int ciSteps = 200;

	// Dimension der Matrix erfassen
	int iDimA = pmA->icol;


	// Runge-Kutta-Variablen initialiseren
	struct vector vK1;
	struct vector vK2;
	struct vector vK3;
	struct vector vK4;

	init_vec(&vK1,iDimA);
	init_vec(&vK2,iDimA);
	init_vec(&vK3,iDimA);
	init_vec(&vK4,iDimA);


	// Return Vector initialisieren
	//struct vector vResult;
	//init_vec(vResult,ciSteps);


	// Approximations-Vektor initialiseren
	struct vector vX;
	init_vec(&vX,iDimA);
	// Anfangsbedingung vX zuweisen
	vX = *pvIC;


	//Anfang Runge-Kutta-Verfahren
	for(int i=0; i<ciSteps; i++){

		// Runge-Kutta-Variablen K1 errechnen
		mv_mult(pmA,&vX,&vK1);

		// Runge-Kutta-Variablen K2 errechnen
		v_scal(&vK1,cdH/2.0,&vK2);
		vv_add(&vX,&vK2,&vK2);
		mv_mult(pmA,&vK2,&vK2);

		// Runge-Kutta-Variablen K3 errechnen
		v_scal(&vK2,cdH/2.0,&vK3);
		vv_add(&vX,&vK3,&vK3);
		mv_mult(pmA,&vK3,&vK3);

		// Runge-Kutta-Variablen K4 errechnen
		v_scal(&vK3,cdH,&vK4);
		vv_add(&vX,&vK4,&vK4);
		mv_mult(pmA,&vK4,&vK4);

		// Den Ks ihre Gewichtungsfaktoren multiplizieren
		v_scal(&vK1,cdH/6.0,&vK1);
		v_scal(&vK2,cdH/3.0,&vK2);
		v_scal(&vK3,cdH/3.0,&vK3);
		v_scal(&vK4,cdH/6.0,&vK4);

		// Ks zusammenaddieren
		vv_add(&vK1,&vK2,&vK1);
		vv_add(&vK3,&vK4,&vK3);

		vv_add(&vK1,&vK3,&vK1);

		// Ks * h mit ursprünglichem Wert addieren
		vv_add(&vX,&vK1,&vX);

		//Ergebnis ausgeben
		printf("%i\n",i);
		vprint(&vX);
	}

	free_vec(&vK1);
	free_vec(&vK2);
	free_vec(&vK3);
	free_vec(&vK4);
	free_vec(&vX);

	return 1;
}
