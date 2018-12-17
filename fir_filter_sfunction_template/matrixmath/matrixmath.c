/*! \file matrixmath.c
 * mathmatrix Matrix Algebra Library
 *	David Knies
 * v1.0 - 15.05.12
 * v1.1 - 24.05.12
 * 	- Dokumentation verbessert
 * 	- return values für Fehler
 *	- Flicken von Speicherloch in mat_free() und mat_malloc()
 *	- matrix/vector_getval() matrix/vector_setval(): Überprüfen, ob Matrix-/Vektordefinitionsbereich verlassen wird.
 **/

#include <stdio.h>
#include <stdlib.h>
#include "matrixmath.h"

//! Matrixausgabefunktion
/*! \param pmA Pointer auf auszugebende Matrixstruktur */
int mprint(struct matrix * pmA){

	int i, j;
	if(pmA!=NULL){
	for(i=0;i<pmA->irow;i++){
		for(j=0;j<pmA->icol;j++){
			printf("%f\t",pmA->ppdData[i][j]);
		}
		printf("\n");
	}
	return SUCCESS;
	}
	else return NULL_POINTER_ARGUMENT;
}

//! Vektorausgabefunktion
/*! \param pvv Pointer auf auszugebenden Vektor */
int vprint(struct vector * pvv){
	int i;
	if(pvv!=NULL){
		for(i=0;i<pvv->irow;i++){
			printf("%f\n",pvv->pdData[i]);
		}
		return SUCCESS;
	}
	else return NULL_POINTER_ARGUMENT;
}

//! Matrizenmultiplikation AxB
/*! \param pmA Pointer auf linke Matrixstruktur
 *  \param pmB Pointer auf rechte Matrixstruktur
 *  \param pmres Pointer auf Ergebnis-Matrixstruktur
 */
int mm_mult(struct matrix * pmA, struct matrix * pmB, struct matrix* pmres){
	int i,j,k;
	if(pmA==NULL||pmB==NULL){
		return NULL_POINTER_ARGUMENT;
	}
	if(pmA->icol!=pmB->irow||pmA->irow!=pmB->icol){
		return DIMENSION_MISSMATCH;
	}
	for(i=0;i<pmA->irow;i++){
		for(j=0;j<pmB->icol;j++){
			pmres->ppdData[i][j]=0.0;
			for(k=0;k<pmA->irow;k++){
				pmres->ppdData[i][j]+=pmA->ppdData[i][k]*pmB->ppdData[k][j];
			}
		}
	}
	return SUCCESS;
}

//! Matrix-Vektor-Multiplikation
/*! \param pmA Pointer auf zu multiplizierende Matrixstruktur
 *  \param pvv Pointer auf zu multiplizierdenden Vektorstruktur
 *  \param pvres Pointer auf Ergebnis-Vektorstruktur
 */
int mv_mult(struct matrix * pmA, struct vector * pvv, struct vector * pvres){

	int i, k;
	if(pmA==NULL||pvv==NULL){
		return NULL_POINTER_ARGUMENT;
	}
	if(pmA->icol!=pvv->irow){
		return DIMENSION_MISSMATCH;
	}
	for(i=0;i<pmA->irow;i++){
		// Ergebnisvektorelement mit 0 initialisieren
		pvres->pdData[i]=0.0;
		//zeilenweise MV-Multiplikation
		for(k=0;k<pmA->icol;k++){
			pvres->pdData[i]+=pmA->ppdData[i][k]*pvv->pdData[k];
		}
	}
	return SUCCESS;
}

//!  Multiplikation Matrix mit Skalar
/*! \param pmA Pointer auf zu multiplizierende Matrixstruktur
*  \param dscal zu multiplizierender Skalar
*  \param pmres Pointer auf Ergebnismatrixstruktur
*/
int m_scal(struct matrix* pmA, double dscal, struct matrix * pmres){
	int i, j;
	if(pmA==NULL){
		return NULL_POINTER_ARGUMENT;
	}

	for(i=0;i<pmA->irow;i++){
		for(j=0;j<pmA->icol;j++){
			pmres->ppdData[i][j]=pmA->ppdData[i][j]*dscal;
		}
	}
	return SUCCESS;
}

//! Multiplikation Vektor mit Skalar
/*! \param pvv Pointer auf zu multiplizierdende Vektorstruktur
 * \param  dscal zu multiplizierender Skalar
 * \param pvres Pointer auf Ergebnisvektorstruktur
 */
int v_scal(struct vector * pvv, double dscal, struct vector * pvres){
	int i;
	if(pvv==NULL){
		return NULL_POINTER_ARGUMENT;
	}
	for(i=0;i<pvv->irow;i++){
		pvres->pdData[i]=pvv->pdData[i]*dscal;
	}
	return SUCCESS;
}

//! Matrixaddition
/*! \param pmA Pointer auf erste zu addierende Matrixstruktur
 *  \param pmB Pointer auf zweite zu addierende Matrixstruktur
 *  \param pmres Pointer auf Ergebnismatrixstruktur
 */
int mm_add(struct matrix * pmA, struct matrix * pmB, struct matrix* pmres){
	if(pmA==NULL||pmB==NULL){
		return NULL_POINTER_ARGUMENT;
	}
	if(pmA->icol!=pmB->icol||pmA->irow!=pmB->irow){
		return DIMENSION_MISSMATCH;
	}
  int i, j;
	for(i=0;i<pmA->irow;i++){
		for(j=0;j<pmA->icol;j++){
			pmres->ppdData[i][j]=pmA->ppdData[i][j]+pmB->ppdData[i][j];
		}
	}
	return SUCCESS;
}

//! Vektoraddition
/*! \param pvv Pointer auf erste zu addierende Vektorstruktur
 *  \param pvx Pointer auf zweite zu addierende Vektorstruktur
 *  \param pvres Pointer auf Ergebnisvektorstruktur
 */
int vv_add(struct vector * pvv, struct vector * pvx, struct vector * pvres){
  int i;
	if(pvv==NULL||pvx==NULL){
		return NULL_POINTER_ARGUMENT;
	}
	if(pvv->irow!=pvx->irow){
		return DIMENSION_MISSMATCH;
	}
	for(i=0;i<pvv->irow;i++){
		pvres->pdData[i]=pvv->pdData[i]+pvx->pdData[i];
	}
	return SUCCESS;
}

//! Reservieren von Speicher für eine double-Matrix der Dimension iz X is und Rückgabe eines Pointers auf diesen Speicherbereich
/*! \param iz Anzahl der Zeilen
 * 	\param is Anzahl der Spalten
 *
 * 	Wenn nicht genügend Speicher für die Matrix reserviert werden kann, wird ein Nullpointer zurückgegeben
 */
double** mat_malloc(int iz, int is){
	int i;
	double** ppdMat;
	ppdMat = (double**) malloc(iz*sizeof(double*));
	if(ppdMat!=NULL){
		for(i=0;i<iz;i++){
			ppdMat[i]=(double*) malloc(is*sizeof(double));
			if(ppdMat[i]==NULL)
			{
				// bisher reservierten Speicher wieder freigeben, wenn Fehler bei Speicherreservierung
				while(i>0){
					free(ppdMat[i-1]);
					i--;
				}
				free(ppdMat);
				return NULL;
			}
		}
	}
	return ppdMat;
}

//! Reservieren von Speicher für einen double-Vektor der Dimension iz und Rückgabe eines Pointers auf diesen Speicherbereich
/*! \param iz Anzahl der Zeilen
 *
 * 	Wenn nicht genügend Speicher für die Matrix reserviert werden kann, wird ein Nullpointer zurückgegeben
 */
double* vec_malloc(int iz){
	return (double*) malloc(iz*sizeof(double));
}

//! Kopieren einer Vektorstuktur
/*! \param pvx Pointer auf zu kopierende Vektorstruktur
 *  \param pvy Pointer auf Kopievektorstruktur
 */
int v_copy(struct vector * pvx, struct vector * pvy){
	int i;
	if(pvx==NULL){
		return NULL_POINTER_ARGUMENT;
	}
	for(i=0;i<pvx->irow;i++){
		pvy->pdData[i]=pvx->pdData[i];
	}
	return SUCCESS;
}			

//! Kopieren einer Matrixstruktur
/*! \param pmA Pointer auf zu kopierende Matrixstruktur
 *  \param pmB Pointer auf Kopiematrixstruktur
 */
int m_copy(struct matrix * pmA, struct matrix * pmB){
	if(pmA==NULL){
		return NULL_POINTER_ARGUMENT;
	}
	int i,j;
	for(i=0;i<pmA->irow;i++){
		for(j=0;j<pmA->icol;j++){
			pmB->ppdData[i][j]=pmA->ppdData[i][j];
		}
	}
	return SUCCESS;
}

//! Initialisieren einer Matrixstruktur, Reservieren des benötigten Speichers
/*! \param pmA Pointer auf zu initialisierende Matrixstruktur
 *  \param iz Anzahl Zeilen der Matrix
 *  \param is Anzahl Spalten der Matrix
 */
int init_mat(struct matrix * pmA,int iz, int is){
	pmA->ppdData=mat_malloc(iz,is);
	if(pmA->ppdData==NULL){
		return OUTOFMEMORY;
	}
	pmA->irow=iz;
	pmA->icol=is; 
	return SUCCESS;
}

//! Initialisieren einer Vektorstruktur, Reservieren des benötigten Speichers
/*! \param pvv Pointer auf zu initialisierende Vetorstruktur
 *  \param iz Anzahl Zeilen des Vektors
 */
int init_vec(struct vector * pvv, int iz){
	pvv->pdData=vec_malloc(iz);
	if(pvv->pdData==NULL){
		return OUTOFMEMORY;
	}
	pvv->irow=iz;
	return SUCCESS;
}


//! Freigabe sämtlichen reservierten Speichers einer Matrixstruktur
/*! \param pmA Pointer auf freizugebende Matrixstruktur */
int free_mat(struct matrix * pmA){
	int i=0;
	if(pmA==NULL){
		return NULL_POINTER_ARGUMENT;
	}
	for(i=0;i<pmA->irow;i++){
		free(pmA->ppdData[i]);
	}
	free(pmA->ppdData);
	return SUCCESS;
}

//! Freigabe sämtlichen reservierten Speichers einer Matrixstruktur
/*! \param pvv Pointer auf freizugebende Vektorstruktur */
int free_vec(struct vector * pvv){
	if(pvv==NULL){
		return NULL_POINTER_ARGUMENT;
	}
	free(pvv->pdData);
	return SUCCESS;
}


/*  Ulf Schaper, 07.05.2012:
 *  Folgende Funktionen eingefügt.
 *
 *  David Knies, 24.05.2012:
 *  - Fehlerabfrage und Kommentierung.
 *  - matrix/vector_getval() matrix/vector_setval(): Überprüfen, ob Matrix-/Vektordefinitionsbereich verlassen wird.
 */


//! Reservieren eines Speicherbereichs für eine Vektorstruktur
/* \param iz Zeilenanzahl des zu speichernden Vektors
 */
struct vector * get_vec(int iz)
{
	struct vector * temp;
	temp = malloc(sizeof(struct vector));
	if (temp) 
	{
		int fehler;
		fehler = init_vec(temp, iz);
		if (fehler)
		{
			free(temp);
			temp = (struct vector *) 0;
		}
	}
	return temp;
}

//! Reservieren eines Speicherbereichs für eine Vektorstruktur
/* \param iz Zeilenanzahl der zu speichernden Matrix
 * \param is Spaltenanzahl der zu speichernden Matrix
 */
struct matrix * get_mat(int iz, int is)
{
	struct matrix * temp;
	temp = malloc(sizeof(struct matrix));
	if (temp) 
	{
		int fehler;
		fehler = init_mat(temp, iz, is);
		if (fehler)
		{
			free(temp);
			temp = (struct matrix *) 0;
		}
	}
	return temp;
}

//! Matrixstruktur mit Nullen füllen
/*! \param pmA Pointer auf zu nullende Matrixstruktur */
int zero_matrix(struct matrix * pmA)
{
	int i, j;

	if (pmA==NULL)
	{
	  return NULL_POINTER_ARGUMENT;
	}

	for (i=0; i<pmA->irow; i++)
	{
		for (j=0; j<pmA->icol; j++)
		{
			pmA->ppdData[i][j] = 0.0;
		}
	}

	return SUCCESS;
}


//! Vektorstruktur mit Nullen füllen
/*! \param pvv Pointer auf zu nullende Vektorstruktur */
int zero_vector(struct vector * pvv)
{
	int i;

	if (!pvv)
	{
	  return 1;
	}

	for (i=0; i<pvv->irow; i++)
	{
		pvv->pdData[i] = 0.0;
	}

	return SUCCESS;
}


//! Setzen des Wertes eines einzelnen Matrixelementes
/*! \param pmA Matrix mit zu veränderndem Element
 * \param izeile Zeilennummer des zu setzenden Elements (C-Notation, [0;n-1])
 * \param ispalte Spaltennummer des zu setzenden Elements (C-Notation, [0;n-1])
 * \param neuer_wert zuzuweisender Wert
 */
int matrix_setval(struct matrix * pmA, int izeile, int ispalte, double neuer_wert)
{
	if (pmA==NULL)
	{
	  return NULL_POINTER_ARGUMENT;
	}
	// falls izeile oder ispalte größer sind als die Matrixdimensionen
	if(izeile>=pmA->irow||ispalte>=pmA->icol){
		return OUTOFRANGE;
	}

	pmA->ppdData[izeile][ispalte] = neuer_wert;

	return SUCCESS;
}

//! Setzen des Wertes eines einzelnen Matrixelementes
/*! \param pvv Matrix mit zu veränderndem Element
 *  \param izeile Zeilennummer des zu setzenden Elements (C-Notation, [0;n-1])
 *  \param neuer_wert zuzuweisender Wert
 */
int vector_setval(struct vector * pvv, int izeile, double neuer_wert)
{
	if (pvv==NULL)
	{
	  return NULL_POINTER_ARGUMENT;
	}
	if (izeile>=pvv->irow){
		return OUTOFRANGE;
	}
	pvv->pdData[izeile] = neuer_wert;

	return SUCCESS;
}

//! Auslesen eines einzelnen Matrixelementes
/*! \param pmA Pointer auf auszulesende Matrixstruktur
 * \param izeile Zeile des auszulesenden Elements (C-Notation, [0;n-1])
 * \param ispalte Spalte des auszulesenden Elements (C-Notation, [0;n-1])
 * \param ausgelesener_wert Pointer auf Rückgabewert
 */
int matrix_getval(struct matrix * pmA, int izeile, int ispalte, double *ausgelesener_wert)
{
	if (pmA==NULL)
	{
	  return NULL_POINTER_ARGUMENT;
	}
	if(izeile>=pmA->irow||ispalte>=pmA->icol){
		return OUTOFRANGE;
	}
	*ausgelesener_wert = pmA->ppdData[izeile][ispalte];
	return SUCCESS;
}

//! Auslesen eines einzelnen Vektorelementes
/*! \param pvv Pointer auf auszulesende Vektorstruktur
 * \param izeile Zeile des auszulesenden Elements (C-Notation, [0;n-1])
 * \param ausgelesener_wert Pointer auf Rückgabewert
 */
int vector_getval(struct vector * pvv, int izeile, double *ausgelesener_wert)
{
	if (pvv==NULL)
	{
		return NULL_POINTER_ARGUMENT;
	}
	if (izeile>=pvv->irow){
		return OUTOFRANGE;
	}
	*ausgelesener_wert = pvv->pdData[izeile];
	return SUCCESS;
}
