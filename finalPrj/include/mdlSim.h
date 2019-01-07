/// \file mdlSim.h
/// \brief Header File für mdlSim Library
///
///	 Stellt Funktionen zur Simulation des QuadCopter Beispiels bereit.
///
/// \author Karl Lukas Knierim (ISYS/KLK)
/// \date 19.10.2012
#ifndef MDLSIM_H_
#define MDLSIM_H_

/*! Wird an cntrlFun übergeben um den Aufruf zu spezifizieren */
typedef enum CntrlFlag_Enum
{
	CntrlFlag_Init,		/*!< Wird beim Initialisierungs-Aufruf übergeben */
	CntrlFlag_Run,  	/*!< Wird beim Run-Aufruf übergeben */
	CntrlFlag_Finish,	/*!< Wird beim Finish-Aufruf übergeben */
} CntrlFlag_E;

/// \brief Simulation des Systems G
///
/// \param[in] t0 Start Zeitpunkt der Simulation
/// \param[in] tf End Zeitpunkt der Simulation
/// \param[in] cntrlFun Funktionspointer auf Regler Funktion
/// \param[in] wFun Funktionspointer auf Sollwert Generator Funktion
/// \param[in] pUserData Pointer auf User Data Storage
///
/// \return EXIT_SUCCESS
///
/// \author Karl Lukas Knierim (ISYS/KLK)
/// \date 19.10.2012
int simSys(const double t0, const double tf, double (*cntrlFun)(CntrlFlag_E, double, double, double,void *), double(*wFun)(double),void* pUserData);


/// \brief Fuehrt einen Zeitschritt des Systems G durch
/**
\param[in]  pvSys Zeiger auf Zustandsvariablen des Systems
\param[in]  dU    Eingang fuer Zeitschritt
\param[out] pdY   Messwert am Ende des Zeitschritts
\return EXIT_SUCCESS
\author Karl Lukas Knierim (ISYS/KLK)
\date 19.10.2012
*/
int stepSys(void *pvSys,const double dU,double *pdY);

/// \brief Gibt den lokalen Speicher in Sys_T
///
/// \param[in] pvSys Zeiger auf Zustandsvariablen des Systems
///
/// \return EXIT_SUCCESS bei Erfolg
///         EXIT_FAILURE wenn pvSys==NULL
///
/// \author Karl Lukas Knierim (ISYS/KLK)
/// \date 19.10.2012
int freeSys(void* pvSys);

/// \brief Reserviert lokalen Speicher und initialiesiert die Übertragungsfunktion
///
///
/// \return NULL bei fehler während der Initialisierung
///			Pointer auf Systemzustand
///
/// \author Karl Lukas Knierim (ISYS/KLK)
/// \date 19.10.2012
void* initSys(void);

/// \brief Beispielfunktion für Sollwert, springt bei t=0 von w=0 auf w=1
///
/// \return EXIT_SUCCESS
///
/// \author Karl Lukas Knierim (ISYS/KLK)
/// \date 19.10.2012
double unitStep(const double dCurTime);


#endif /* MDLSIM_H_ */
