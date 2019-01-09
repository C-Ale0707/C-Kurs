#include <stdlib.h>
#include <stdio.h>

double dE;
double dEold;
double dIntegral = 0;
double dDerivative;
double dpUr;
double dpUr2;
double diff = 0;

int pid_regler(double* pdU, double dtStep, double dSoll, double dIst, double dKp, double dKi, double dKd){
	if (dKp<0){
		return EXIT_FAILURE;
	}
	if (dKi<0){
		return EXIT_FAILURE;
	}
	if (dKd<0){
		return EXIT_FAILURE;
	}
	if (dtStep<=0){
		return EXIT_FAILURE;
	}

	dE = dSoll - dIst;                                      // Error
	dIntegral = dIntegral + (dE + (diff/dKi)) * dtStep;     // Update Integral
	dDerivative = (dE - dEold) / dtStep;                    // Calculate Derivative
	dpUr = dKp * dE + dKi * dIntegral + dKd * dDerivative;        // Sum up the output

	dpUr2 = dpUr;
	if (dpUr>250){
		dpUr2 = 250;
	}
	if (dpUr<0){
		dpUr2 = 0;
	}

	diff = dpUr2 - dpUr;

	*pdU = dpUr2;
	// printf("%f",*pdU);
	dEold = dE;                                         // Update old Error
	// dU = 1;
	return EXIT_SUCCESS;
}
