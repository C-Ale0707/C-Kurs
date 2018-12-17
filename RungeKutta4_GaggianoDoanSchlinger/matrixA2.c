/*
 * matrixA2.c
 *
 *  Created on: 05.12.2018
 *      Author: Alessio Gaggiono, Marco Schlinger, Truc-Quynh Doan
 */
#include <stdio.h>
#include <stdlib.h>
#include "matrixmath.h"


void buildSystemA2(struct matrix *pmA2, struct vector *pvX2){

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

}
