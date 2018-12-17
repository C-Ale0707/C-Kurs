#undef S_FUNCTION_NAME 
#define S_FUNCTION_NAME iir_filter_v1_sfunction        /* Name der S-Function festlegen */
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"       /* wegen SimStruct S */
#include <coeff_iir.h>
#include <iir_filter_v1.h>

#define NENNER_COEFF_VECTOR_A ssGetSFcnParam(S,0)
#define ZAEHLER_COEFF_VECTOR_B ssGetSFcnParam(S,1)

static void mdlInitializeSizes(SimStruct *S)
{
    /*  In der Funktion mdlInitializeSizes müssen wir Simulink
     *  die Schnittstellen unseres Blockes mitteilen.
     *  D.h. wir sagen wieviele Ein- und Ausgänge wir haben,
     *  welche Dimensionen diese Ports haben, welche Daten Simulink 
     *  für uns zwischen mehreren Zeitschritten 
     *  speichern muss, ob wir Parameter erwarten, usw.
     */
       
    ssSetNumSFcnParams(S, 2);  
    
    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);

	ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 0);
    ssSetNumPWork(S, 1); 
    
    if (!ssSetNumInputPorts(S, 1)) return;
    ssSetInputPortWidth(S, 0, 1);
    ssSetInputPortDirectFeedThrough(S, 0, 1);
     
    if (!ssSetNumOutputPorts(S, 1)) return;
    ssSetOutputPortWidth(S, 0, 1);

    ssSetInputPortDataType(S, 0, SS_DOUBLE);
    ssSetOutputPortDataType(S, 0, SS_DOUBLE); 
    
    ssSetNumSampleTimes(S, 1);
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);

}


static void mdlInitializeSampleTimes(SimStruct *S)
{
    /* Mit der Funktion mdlInitializeSampleTimes teilen wir
     * Simulink mit, mit welcher Abtastrate unser Block ausgeführt werden 
     * soll. 
     * Hier gehen wir davon aus, dass der Simulink-Programmierer
     * das von oben her im Griff hat, d.h. wir erben einfach eine uns
     * zugewiesene Abtastrate (INHERITED_SAMPLE_TIME).
     */
    ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
    ssSetOffsetTime(S, 0, 0.0);
}


#define MDL_START 
#if defined(MDL_START) 
static void mdlStart(SimStruct *S)
{
   /* Die mdlStart wird einmalig zu Simulationsbeginn ausgeführt.
    * Dies ist ein sinnvoller Ort, um dynamisch Speicher zu reservieren.
    */
   int n; //Filterordnung
   real_T* koeffizienten;
   
   struct iir_filter_variablen * piir_filter_variablen;
   
   koeffizienten = (real_T*)mxGetData(NENNER_COEFF_VECTOR_A);
   n = mxGetNumberOfElements(NENNER_COEFF_VECTOR_A)-1;
   
   piir_filter_variablen =(struct iir_filter_vaariablen *)malloc(sizeof(struct iir_filter_variablen));
   if(!piir_filter_variablen){
   
       ssSetErrorStatus(S,"Speicherreservierung fehlgeschlagen.");
       return;
   }
   
   if(alloc_iir_filter_variablen_struct(piir_filter_variablen,n) != RET_SUCCESS){
       /*Bei einem Rückgabewert !=0 ist ein Fehler aufgetreten.*/
       
       free(piir_filter_variablen);
       ssSetErrorStatus(S,"Speicherrreservierung/Initialisierung fehlgeschlagen.");
       return;
      
   }
   
   setze_koeffizienten(piir_filter_variablen,(double *)koeffizienten);
   ssSetPWorkValue(S,0,piir_filter_variablen);
   
   
}
#endif

static void mdlOutputs(SimStruct *S, int_T tid)
{
   /* Die mdlOutputs wird in jedem Simulink-Zeitschritt ausgeführt.
    * Hier müssen aus den Eingangsdaten des S-Function-Blocks neue 
    * Ausgangsdaten berechnet werden.
    */
    
    real_T * y;
    InputREalPtrsType u;
    struct iir_filter_variablen * piir_filter_variablen;
    
    y = ssGetOUtputPortRealSignal(S,0);
    u = ssGetInputPortRealSignalPtrs(S,0);
    aKoeffizienten = (real_T *)mxGetData(NENNER_COEFF_VECTOR_A);
    bKoeffizienten = (real_T *)mxGetData(ZAEHLER_COEFF_VECTOR_B);
    
    
    //Pointer auf vorab reservierten Speicher auslesen:
    
    piir_filter_variablen = (struct iir_filter_variablen *) ssGetPWorkValue(S,0);
    
    //Einlesen der aktuellen Koeffizienten:
    
    setze_koeffizienten(piir_filter_variablen,(double*) aKoeffizienten, (double *) bKoeffizienten);
    
    //Filterberechnung:
    
    filterausgabe_berechnen(pfir_filter_variablen, *u[0],y);
   
   
}
     
static void mdlTerminate(SimStruct *S)
{
   /* Die Funktion mdlTerminate wird einmalig beim Beenden einer Simulation
    * ausgeführt. Dies ist ein sinnvoller Ort um den in der mdlStart 
    * reservierten Speicher wieder freizugeben.
    */
    
    
    
    int * Nullpointer;
    
    struct iir_filter_variablen * piir_filter_variablen;
    piir_filter_variablen = (struct iir_filter_variablen *) ssGetPWorkValue(S,0);
    
    if(piir_filter_variablen){
        //Gebe nicht mehr benötigten reservierten Speicher wieder frei
        
        free_iir_filter_variablen_struct(piir_filter_variablen);
        free(piir_filter_variablen);
    
    }
    
    Nullpointer = (int *) 0;
    ssSetPWorkValue(S, 0, Nullpointer);
    

    
}

/* S-Function trailer
 */
#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else                  
#include "cg_sfun.h"       /* Code generation registration function */
#endif                 
