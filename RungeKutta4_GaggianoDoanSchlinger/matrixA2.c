/*
 * matrixA2.c
 *
 *  Created on: 05.12.2018
 *      Author: Alessio Gaggiono, Marco Schlinger, Truc-Quynh Doan
 */
#include <stdio.h>
#include <stdlib.h>
#include "matrixmath.h"


void buildMatrixA2(struct matrix *pmA2_res, struct vector *pvX2_res){


//Erstelle Matrizen mA2 und Vektor vX2:
	struct matrix mA2;
	struct vector vX2;

	struct matrix *pmA2;
	struct vector *pvX2;


//Zeiger initialisieren:

		pmA2 = &mA2;
		pvX2 = &vX2;
//Initialisiere Matrizen und Vektoren:
		init_mat(pmA2,2,2);

		init_vec(pvX2,2);

//Matrix mA1
		matrix_setval(pmA2,0,0,-1);
		matrix_setval(pmA2,0,1,0);
		matrix_setval(pmA2,1,0,0);
		matrix_setval(pmA2,1,1,1);

//Vektor vX2
		vector_setval(pvX2,0,1);
		vector_setval(pvX2,1,1);

//Uebergebe fertig gebaute Matrix und Anfangsbedingung der main.c
		pmA2_res = &pmA2;
		pvX2_res = &pvX2;
}
