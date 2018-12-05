/*
 * matrixA3.c
 *
 *  Created on: 05.12.2018
 *      Author: Alessio Gaggiono, Marco Schlinger, Truc-Quynh Doan
 */
#include <stdio.h>
#include <stdlib.h>
#include "matrixmath.h"


void buildMatrixA3(struct matrix *pmA3_res, struct vector *pvX3_res){


//Erstelle Matrizen mA3 und Vektor vX3:
	struct matrix mA3;
	struct vector vX3;

	struct matrix *pmA3;
	struct vector *pvX3;

//Zeiger initialisieren:

		pmA3 = &mA3;
		pvX3 = &vX3;

//Initialisiere Matrizen und Vektoren:
		init_mat(pmA3,4,4);

		init_vec(pvX3,3);
//Matrix mA1
		matrix_setval(pmA3,0,0,0);
		matrix_setval(pmA3,0,1,1);
		matrix_setval(pmA3,0,2,0);
		matrix_setval(pmA3,0,3,0);
		matrix_setval(pmA3,1,0,0);
		matrix_setval(pmA3,1,1,0);
		matrix_setval(pmA3,1,2,1);
		matrix_setval(pmA3,1,3,0);
		matrix_setval(pmA3,2,0,0);
		matrix_setval(pmA3,2,1,0);
		matrix_setval(pmA3,2,2,0);
		matrix_setval(pmA3,2,3,1);
		matrix_setval(pmA3,3,0,-24);
		matrix_setval(pmA3,3,1,-50);
		matrix_setval(pmA3,3,2,-35);
		matrix_setval(pmA3,3,3,-10);

//Vektor vX3
		vector_setval(pvX3,0,1);
		vector_setval(pvX3,1,0);
		vector_setval(pvX3,2,1);

//Uebergebe fertig gebaute Matrix und Anfangsbedingung der main.c
		pmA3_res = &pmA3;
		pvX3_res = &pvX3;
}