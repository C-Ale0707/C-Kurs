#include <stdlib.h>

double dE;
double dEold = 0;
double dU;
double dIntegral = 0;
double dDerivative;

double pid_regler(double* pdU, double dtStep, double dSoll, double dIst, double dKp, double dKi, double dKd){
	if (dKp<0 || dKi<0 || dKd<0){
		return EXIT_FAILURE;
	}
	dE = dSoll - dIst;                                    // Error
	dIntegral = dIntegral + dE * dtStep;                  // Update Integral
	dDerivative = (dE - dEold) / dtStep;                  // Calculate Derivative
	dU = dKp * dE + dKi * dIntegral + dKd * dDerivative;  // Sum up the output
	dEold = dE;                                           // Update old Error
	*pdU = dU;
	return EXIT_SUCCESS;
}
