/*
 * matrixA1.c
 *
 *  Created on: 05.12.2018
 *      Author: Alessio Gaggiono, Marco Schlinger, Truc-Quynh Doan
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "matrixmath.h"



void buildMatrixA1(struct matrix *pmA1_res, struct vector *pvX1_res){


//Erstelle Matrizen mA1 und Vektor vX1:
	struct matrix mA1;
	struct vector vX1;

	struct matrix *pmA1;
	struct vector *pvX1;

//Zeiger initialisieren:

	pmA1 = &mA1;
	pvX1 = &vX1;

//Initialisiere Matrizen und Vektoren:
	init_mat(pmA1,3,3);

	init_vec(pvX1,3);

//Matrix mA1
	matrix_setval(pmA1,0,0,0);
	matrix_setval(pmA1,0,1,1);
	matrix_setval(pmA1,0,2,0);
	matrix_setval(pmA1,1,0,0);
	matrix_setval(pmA1,1,1,0);
	matrix_setval(pmA1,1,2,1);
	matrix_setval(pmA1,2,0,-6);
	matrix_setval(pmA1,2,1,-11);
	matrix_setval(pmA1,2,2,-6);

//Vektor vX1
	vector_setval(pvX1,0,5);
	vector_setval(pvX1,1,2);
	vector_setval(pvX1,2,0);

//Uebergebe fertig gebaute Matrix und Anfangsbedingung der main.c
	pmA1_res = &pmA1;
	pvX1_res = &pvX1;
}
