/// \file mdlSim.c
/// \brief Implementierung von mdlSim
///
///	 Stellt Funktionen zur Simulation des QuadCopter Beispiels bereit.
///
/// \author Karl Lukas Knierim (ISYS/KLK)
/// \date 19.10.2012

#include "mdlSim.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 4 					//!< Nennerordnung
#define M 4 					//!< Zählerordnung
#define DT 0.01 				//!< Zeitschrittweite
#define K_f   2.2 				//!< Verstärkung v->F
#define K_v   (1.0/(2.0*M_PI))  //!< Verstärkung w->v_l
#define m     1.5  				//!< Masse des Quad-Copters
#define g     9.81 				//!< Erdbeschleunigung

//! Zaehlerpolynom
static double adA[N] = {                   0e-6,   0.227265102996257e-6,  0.454530205992514e-6,	   0.227265102996257e-6};
//! Nennerpolynom
static double adB[M] = {      1.000000000000000,  -2.946229804136443,     2.893030499727596,  -0.946800695591153};


/*! Struktur zum Speicher des Zustands */
typedef struct
{
	double * pdU; 		//!< Speicher fuer Eingang
	double * pdY;		//!< Speicher fuer Ausgang
	unsigned uCycCntU;  //!< Zaehler fuer Index in zyklischem Eingangsspeicher
	unsigned uCycCntY;  //!< Zaehler fuer Index in zyklischem Eingangsspeicher
}Sys_T;


void* initSys(void)
{
	Sys_T *ptSys=NULL;
	unsigned ui=0;
	// Initialize random number
	srand ( time(NULL) );

	ptSys=(Sys_T*)malloc(sizeof(Sys_T));
	if(ptSys==NULL)
	{
		return NULL;
	}
	// Initialize pointer to zero
	ptSys->pdU=NULL;
	ptSys->pdY=NULL;
	// allocate memory for system values
	ptSys->pdU=(double*)malloc(N*sizeof(double));
	if(ptSys->pdU==NULL)
	{
		// delete allocated memory
		freeSys(ptSys);
		return NULL;
	}

	ptSys->pdY=(double*)malloc(M*sizeof(double));
	if(ptSys->pdY==NULL)
	{
		freeSys(ptSys);
		return NULL;
	}

	// reset cycle counters
	ptSys->uCycCntU=N;
	ptSys->uCycCntY=M;
	for(ui=0;ui<M;ui++)
	{
		ptSys->pdY[ui]=0.0;
	}
	for(ui=0;ui<N;ui++)
	{
		ptSys->pdU[ui]=0.0;
	}

	return (void*)ptSys;
}


int freeSys(void *pvSys)
{
	Sys_T* ptSys=(Sys_T*)pvSys;
	if(ptSys==NULL)
	{
		return EXIT_FAILURE;
	}
	if(ptSys->pdU!=NULL)
	{
		free(ptSys->pdU);
		ptSys->pdU=NULL;
	}
	if(ptSys->pdY!=NULL)
	{
		free(ptSys->pdY);
		ptSys->pdY=NULL;
	}

	free(ptSys);
	return EXIT_SUCCESS;
}

int stepSys(void *pvSys,const double dU,double *pdY)
{
	double dA=0.0;
	double dB=0.0;
    unsigned ui=0;
    double dNoise=0.0;
    Sys_T *ptSys=(Sys_T*)pvSys;

	// set current adU Value
  	ptSys->pdU[ptSys->uCycCntU%N]=dU-(m*g)/(K_f*K_v);
   	   // build sum a'*U
   	for(ui=0;ui<N;ui++)
   	{
   		dA+=adA[ui]* ptSys->pdU[(ptSys->uCycCntU-ui)%N];
   	}
   	// build sum b'*Y
   	for(ui=1;ui<M;ui++)
   	{
   		dB+=adB[ui]* ptSys->pdY[(ptSys->uCycCntY-ui)%M];
   	}
   	// set current adY Value
   	ptSys->pdY[ptSys->uCycCntY%M] = (dA-dB)/adB[0];

   	dNoise = 0.25*(2.0*(double)rand()/(double)RAND_MAX-1.0);
   	*pdY=ptSys->pdY[ptSys->uCycCntY%M]+dNoise;
    // Increase Cycle Counter
   	ptSys->uCycCntU=(ptSys->uCycCntU+1)%N+N;
   	ptSys->uCycCntY=(ptSys->uCycCntY+1)%M+M;
	return EXIT_SUCCESS;
}


int simSys(const double t0, const double tf, double (*cntrlFun)(CntrlFlag_E, double, double, double,void *), double(*wFun)(double),void* pUserData)
{
	double dCurT=t0;
	double dY    =  0.0;
	double dW    =  0.0;
	double dU    =  0.0;
	void*  pvSys = NULL;

	// Init System
	pvSys=initSys();
	// ersten Sollwert auslesen (für Initialisierung)
	dW=wFun(dCurT);
	// Initialiesierungs Aufruf für CntrlFun
	cntrlFun(CntrlFlag_Init,dCurT,dY,dW,pUserData);
	// simulate system
	for(dCurT=t0;dCurT<=tf;dCurT+=DT)
	{
		// sollwert berechen
		dW=wFun(dCurT);
		// Reglerfunktion aufrufen
		dU=cntrlFun(CntrlFlag_Run,dCurT,dY,dW,pUserData);
		// Zeitschritt Rechnen
		stepSys(pvSys,dU,&dY);
	}
	// Aufruf von cntrlFun mit CntrlFlag_Finish
	cntrlFun(CntrlFlag_Finish,tf,dY,dW,pUserData);
	// freigeben des reseriverten Speichers
	freeSys(pvSys);
	return EXIT_SUCCESS;
}


double unitStep(const double dCurTime)
{
	// check time value
	if(dCurTime>=0)
	{
		return 1.0; // t>=0
	}
	else
	{
		return 0.0; // t<0
	}
}

