#include <stdio.h>
#include <stdlib.h>
#include "matrixmath.h"

void buildSystemA1(struct matrix* pmA1_res, struct vector* pvX1_res);
void buildSystemA2(struct matrix* pmA2_res, struct vector* pvX2_res);
void buildSystemA3(struct matrix* pmA3_res, struct vector* pvX3_res);

int rungeKutta4(struct matrix* pmA, struct vector* pvIC, const int ciT_sim, const double cdH);


int main (){

	//Erstelle n�tige Strukturen f�r die Matrizen A1, A2, A3
	struct matrix mA1;
	struct matrix mA2;
	struct matrix mA3;


	//Erstelle n�tige Strukturen f�r die Initial Condition Vektoren (AB's)
	struct vector vIC1;
	struct vector vIC2;
	struct vector vIC3;

	//Funktionsaufrufe um Arrays und ICs zu erstellen und sie den bisher ziellosen Pointern zuzuweisen
	buildSystemA1(&mA1, &vIC1);
	buildSystemA2(&mA2, &vIC2);
	buildSystemA3(&mA3, &vIC3);


	//Wichtige Konstanten f�r die Berechnung mit der Fkt. RungeKuttta4 (Schrittweite h & Sim.Zeit)
	const double cdH=0.05;
	const int ciT_sim=10;



	//---Funktionsaufruf: RungeKutta 4:--//

	// rungeKutta4(&mA1,&vIC1,ciT_sim,cdH);
	rungeKutta4(&mA2,&vIC2,ciT_sim,cdH);
	// rungeKutta4(&mA3,&vIC3,ciT_sim,cdH);


	//Gebe nicht mehr ben�tigten Arbeitspeicherfrei:
	free_mat(&mA1);
	free_mat(&mA2);
	free_mat(&mA3);
	free_vec(&vIC1);
	free_vec(&vIC2);
	free_vec(&vIC3);



	return EXIT_SUCCESS;
}
