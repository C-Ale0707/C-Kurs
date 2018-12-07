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



void buildSystemA1(struct matrix* pmA1, struct vector* pvIC1){

//Initialisiere Matrizen und Vektoren:
	init_mat(pmA1,3,3);

	init_vec(pvIC1,3);

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
	vector_setval(pvIC1,0,5);
	vector_setval(pvIC1,1,2);
	vector_setval(pvIC1,2,0);

}
