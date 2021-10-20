#ifndef GMKP_CPX_H_
#define GMKP_CPX_H_

#include <ilcplex/cplex.h>
#include <math.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "CHECK_CONS.h"

int solveGMKP_CPX(int n, int m, int r, int * b, int * weights, int * profits, int * capacities, int * setups, int * classes, int * indexes, char * modelFilename, char * logFilename, int TL, bool intflag);

#endif /* GMKP_CPX_H_ */