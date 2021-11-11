#ifndef LPBASED_CPX_H_
#define LPBASED_CPX_H_

#include <ilcplex/cplex.h>
#include <math.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "CHECK_CONS_V2.h"

int solve(int n, int m, int r, int * b, int * weights, int * profits, int * capacities, int * setups, int * classes, int * indexes, char * modelFilename, char * logFilename, int TL);

#endif /* LPBASED_CPX_H_ */