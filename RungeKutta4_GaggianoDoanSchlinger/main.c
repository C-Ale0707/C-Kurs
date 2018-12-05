#include <stdio.h>
#include <stdlib.h>
#include "matrixmath.h"

int main (){

	//Erstelle Pointer auf Matrizen A1, A2, A3

	struct matrix *pmA1;
	struct matrix *pmA2;
	struct matrix *pmA3;


	//Erstelle Pointer auf Initial Condition Vektoren
	struct vector *pvIC1;
	struct vector *pvIC2;
	struct vector *pvIC3;


	//Funktionsaufrufe um Arrays und ICs zu erstellen und sie den bisher ziellosen Pointern zuzuweisen
	buildMatrixA1(pmA1, pvIC1);
	buildMatrixA2(pmA2, pvIC2);
	buildMatrixA3(pmA3, pvIC3);


	// Runge Kutta


	//Gebe nicht mehr benötigten Arbeitspeicherfrei:
	free_mat(pmA1);
	free_mat(pmA2);
	free_mat(pmA3);
	free_vec(pvIC1);
	free_vec(pvIC2);
	free_vec(pvIC3);



	return EXIT_SUCCESS;
}
