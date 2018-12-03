#include <stdio.h>
#include <stdlib.h>
#include "matrixmath.h"

int main (){


	//Beispiel-Test mit der matrixmath Bibliothek:


	//Erstelle Matrizen A,B und Vektor v:

	struct matrix A,B;
	struct vector v, v_A, v_B;


	struct matrix *pmA;
	struct matrix *pmB;
	struct vector *pvV;
    struct vector *pvV_A;
	struct vector *pvV_B;

	//Zeiger initialisieren:

	pmA = &A;
	pmB = &B;

	pvV = &v;
	pvV_A = &v_A;
	pvV_B = &v_B;

	//Initialisiere Matrizen und Vektoren:
	init_mat(pmA,2,2);
	init_mat(pmB,2,2);

	init_vec(pvV,2);
	init_vec(pvV_A,2);
	init_vec(pvV_B,2);

	//Setze Einträge der Matrizen und Vektoren:

	//Matrix A

	zero_matrix(pmA); // Matrix A = Nullmatrix

	//Matrix B
	matrix_setval(pmB,0,0,1); //Matrix B = Einheitsmatrix
	matrix_setval(pmB,0,1,0);
	matrix_setval(pmB,1,0,0);
	matrix_setval(pmB,1,1,1);

	//Vektor v
	vector_setval(pvV,0,1);
	vector_setval(pvV,1,1);

	//Printe A,B und v auf die Console:

	 /* mprint(pmA);
		printf("\n");
		mprint(pmB);
		printf("\n");
		vprint(pvV); */

	//MatrixVektor-Multiplikationen:

	mv_mult(pmA,pvV,pvV_A); // A * v = v_A
	mv_mult(pmB,pvV,pvV_B); // B * v = v_B


	//Printe die neuen Vektoren v_A und v_B aus:

	printf("Vektor V_A:\n");
	vprint(pvV_A);
	printf("\n");
	printf("Vektor V_B:\n");
	vprint(pvV_B);
	printf("\n");


	//Gebe nicht mehr benötigten Arbeitspeicherfrei:
	free_mat(pmA);
	free_mat(pmB);
	free_vec(pvV);
	free_vec(pvV_A);
	free_vec(pvV_B);






	return EXIT_SUCCESS;
}
