#include "LPBASED_CPX.h"

#define VARNAMES //use variable names
#define CONSNAMES //use constraint names
#define DEBUG //print cplex run on screen
#define WRITELP //write lp problem to file
#define WRITELOG //write log

int solve(int n, int m, int r, int * b, int * weights, int * profits, int * capacities, int * setups, int * classes, int * indexes, char * modelFilename, char * logFilename, int TL) {

	/*******************************************/
	/*     set CPLEX environment and lp        */
	/*******************************************/
	CPXENVptr env;
	CPXLPptr lp;
	int status;
	double objval;
	clock_t start, end;
	double time;

	/* open CPLEX environment
	 * */
	env = CPXopenCPLEX(&status);
	if (status) {
		std::cout << "error: GMKP CPXopenCPLEX failed...exiting" << std::endl;
		exit(1);
	}

	/* set CPLEX data checking ON
	 * */
	status = CPXsetintparam(env, CPX_PARAM_DATACHECK, CPX_ON);
	if (status) {
		std::cout << "error: GMKP set CPLEX data checking ON failed...exiting" << std::endl;
		exit(1);
	}

#ifndef NDEBUG
	/* set CPLEX output ON
	 * */
	status = CPXsetintparam(env, CPX_PARAM_SCRIND, CPX_ON);
	if (status) {
		std::cout << "error: GMKP set CPLEX output ON failed...exiting" << std::endl;
		exit(1);
	}
#endif

	/* create CPLEX lp
	 * */
	lp = CPXcreateprob(env, &status, "GMKP - Callable Library");
	if (status) {
		std::cout << "error: GMKP CPXcreateprob failed...exiting" << std::endl;
		exit(1);
	}

	/*******************************************/
	/*     add CPLEX columns                   */
	/*******************************************/
	/* order of the variables (x_ij) i = 1...m (knapsack index), j = 1...n (object index) in CPLEX lp
	 *
	 * e.g., m = 2, n = 3
	 *
	 * [x_11, x_12, x_13, x_21, x_22, x_23]
	 *
	 * */

	/* order of the variables (y_ik) i = 1...m (knapsack index), k = 1...r (classes index) in CPLEX lp
	 *
	 * e.g., r = 2, k = 2
	 *
	 * [y_11, y_12, y_21, y_22]
	 *
	 * */

	double *obj, *lb, *ub;
	char *ctype;
	char **vnames = 0;
	char **cnames = 0;
	int ccnt = n*m + m*r; // number of columns

	int col; //column counter

	/* set objective function sense
	 * */
	CPXchgobjsen(env, lp, CPX_MAX);
	if (status) {
		std::cout << "error: GMKP CPXchgobjsen failed...exiting" << std::endl;
		exit(1);
	}

	// allocate memory for column description
	obj = new double[ccnt];
	lb =  new double[ccnt];
	ub =  new double[ccnt];
	ctype = new char[ccnt];

#ifndef NDEBUG
	vnames = new char*[ccnt];
	for (int i = 0; i < ccnt; i++)
		vnames[i] = new char[100];
#endif

	col = 0; // init column counter

	// fill column vectors
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {

			obj[col] = profits[i*n + j];
			lb[col] = 0.0;
			ub[col] = 1.0;

#ifndef NDEBUG
			sprintf(vnames[col], "x_%d_%d", i + 1, j + 1);
#endif
			col++;

		} // j (items)
	} // i (knapsacks)

	for (int i = 0; i < m; i++) {
		for (int k = 0; k < r; k++) {

			obj[col] = 0;
			lb[col] = 0.0;
			ub[col] = 1.0;

#ifndef NDEBUG
			sprintf(vnames[col], "y_%d_%d", i + 1, k + 1);
#endif
			col++;
		} // k (classes)
	} // i (knapsacks)

	/* add columns to CPLEX LP
	 * */
	status = CPXnewcols(env, lp, ccnt, obj, lb, ub, NULL, vnames);
	
	if (status) {
		std::cout << "error: GMKP CPXnewcols failed...exiting" << std::endl;
		exit(1);
	}

	// free columns stuff
	delete [] lb;
	delete [] ub;
	delete [] obj;
	delete [] ctype;
#ifndef NDEBUG
	for (int i = 0; i < ccnt; i++)
		delete[] vnames[i];
	delete vnames;
#endif

	/*******************************************/
	/*   add CPLEX constraints                 */
	/*******************************************/

	/*	constraint (1):
		\sum_{j = 1 ... n} w_j * x_ij + \sum_{k = 1 ... r} s_k * y_ik <= C_i		\forall i \in M
	 * */
	int rcnt = m; // number of constraints (rows)
	int nzcnt = ccnt; // number of total variables (columns)

	// allocate memory for constraint
	int *rmatbeg = new int[rcnt];
	double *rhs = new double[rcnt];
	char *sense = new char[rcnt];
	int *rmatind = new int[nzcnt];
	double *rmatval = new double[nzcnt];

#ifndef NDEBUG
	cnames = new char*[rcnt];
	for (int i = 0; i < rcnt; i++)
		cnames[i] = new char[100];
#endif

	// init counter
	int cc = 0;
	for (int i = 0; i < m; i++)
	{
		rmatbeg[i] = cc; // starting index of the n-th constraint
		sense[i] = 'L';
		rhs[i] = capacities[i];

		// \sum_{j = 1 ... n} w_j * x_ij
		for (int j = 0; j < n; j++)
		{
			rmatind[cc] = i * n + j; // variable number
			rmatval[cc] = weights[j];
			cc++;
		}

		// \sum_{k = 1 ... r} s_k * y_ik
		for (int k = 0; k < r; k++)
		{
			rmatind[cc] = n*m + i * r + k;
			rmatval[cc] = setups[k];
			cc++;
		}

#ifndef NDEBUG
		sprintf(cnames[i], "capacity_%d", i + 1);
#endif
	}

	/* add rows for capacity constraints
	 * */
	status = CPXaddrows(env, lp, 0, rcnt, nzcnt, rhs, sense, rmatbeg, rmatind, rmatval, NULL, cnames);
	if (status) {
		std::cout << "error: GMKP CPXaddrows (1-th constraint) failed...exiting" << std::endl;
		exit(1);
	}

	// free rows stuff
	delete[] rmatbeg;
	delete[] sense;
	delete[] rhs;
	delete[] rmatind;
	delete[] rmatval;
#ifndef NDEBUG
	for (int i = 0; i < rcnt; i++)
		delete[] cnames[i];
	delete cnames;
#endif

	/*	constraint (2):
		\sum_{i = 1 ... m} x_ij <= 1       \forall j \in N
	 * */

	rcnt = n; // number of constraints (rows)
	nzcnt = m * n; // number of total variables (columns)

	// allocate memory for constraint
	rmatbeg = new int[rcnt];
	rhs = new double[rcnt];
	sense = new char[rcnt];
	rmatind = new int[nzcnt];
	rmatval = new double[nzcnt];

#ifndef NDEBUG
	cnames = new char*[rcnt];
	for (int i = 0; i < rcnt; i++)
		cnames[i] = new char[100];
#endif

	// init counter
	cc = 0;
	// fill in rows for multiple knapsack constraints
	for (int j = 0; j < rcnt; j++)
	{
		rmatbeg[j] = cc; // starting index of the n-th constraint
		sense[j] = 'L';
		rhs[j] = 1.0;

		for (int i = 0; i < m; i++)
		{
			rmatind[cc] = i * n + j; // variable number
			rmatval[cc] = 1.0;
			cc++;
		}

#ifndef NDEBUG
		sprintf(cnames[j], "max_one_bin_x_%d", j + 1);
#endif
	}

	/* add rows for multiple knapsack constraints
	 * */
	status = CPXaddrows(env, lp, 0, rcnt, nzcnt, rhs, sense, rmatbeg, rmatind, rmatval, NULL, cnames);
	if (status) {
		std::cout << "error: GMKP CPXaddrows (2-nd constraint) failed...exiting" << std::endl;
		exit(1);
	}

	// free rows stuff
	delete[] rmatbeg;
	delete[] sense;
	delete[] rhs;
	delete[] rmatind;
	delete[] rmatval;
#ifndef NDEBUG
	for (int i = 0; i < rcnt; i++)
		delete[] cnames[i];
	delete cnames;
#endif

	/*	constraint (3):
		\sum_{i = 1 ... m} y_ik <= b_k		\forall k \in K
	 * */

	rcnt = r; // number of constraints (rows)
	nzcnt = m*r; // number of total variables (columns)

	// allocate memory for constraint
	rmatbeg = new int[rcnt];
	rhs = new double[rcnt];
	sense = new char[rcnt];
	rmatind = new int[nzcnt];
	rmatval = new double[nzcnt];

#ifndef NDEBUG
	cnames = new char*[rcnt];
	for (int i = 0; i < rcnt; i++)
		cnames[i] = new char[100];
#endif

	// init counter
	cc = 0;
	// fill in rows for multiple knapsack constraints
	for (int k = 0; k < rcnt; k++)
	{
		rmatbeg[k] = cc; // starting index of the n-th constraint
		sense[k] = 'L';
		rhs[k] = b[k];

		for (int i = 0; i < m; i++)
		{
			rmatind[cc] = n*m + i * r + k; // variable number
			rmatval[cc] = 1.0;
			cc++;
		}

#ifndef NDEBUG
		sprintf(cnames[k], "max_b_bin_y_%d", k + 1);
#endif
	}

	/* add rows for multiple knapsack constraints
	 * */
	status = CPXaddrows(env, lp, 0, rcnt, nzcnt, rhs, sense, rmatbeg, rmatind, rmatval, NULL, cnames);
	if (status) {
		std::cout << "error: GMKP CPXaddrows (3-rd constraint) failed...exiting" << std::endl;
		exit(1);
	}

	// free rows stuff
	delete[] rmatbeg;
	delete[] sense;
	delete[] rhs;
	delete[] rmatind;
	delete[] rmatval;
#ifndef NDEBUG
	for (int i = 0; i < rcnt; i++)
		delete[] cnames[i];
	delete cnames;
#endif

	/*	constraint (4):
		x_ij <= y_ik		\forall i \in M, \forall k \in K, \forall j \in R_k
		x_ij - y_ik <= 0	\forall i \in M, \forall k \in K, \forall j \in R_k
	 * */

	rcnt = n*m; // number of constraints (rows)
	nzcnt = n * m + n * m; // number of total variables (columns)

	// allocate memory for constraint
	rmatbeg = new int[rcnt];
	rhs = new double[rcnt];
	sense = new char[rcnt];
	rmatind = new int[nzcnt];
	rmatval = new double[nzcnt];

#ifndef NDEBUG
	cnames = new char*[rcnt];
	for (int i = 0; i < rcnt; i++)
		cnames[i] = new char[100];
#endif

	// init counter
	cc = 0;
	int prov2 = 0;
	// fill in rows for multiple knapsack constraints
	for (int k = 0; k < r; k++) {
		int indexes_prev = k > 0 ? indexes[k - 1] : 0;
		for (int z = 0; z < indexes[k] - indexes_prev; z++) {

			for (int i = 0; i < m; i++) {

				rmatbeg[prov2] = cc; // starting index of the n-th constraint
				sense[prov2] = 'L';
				rhs[prov2] = 0.0;

				rmatind[cc] = n * m + i * r + k; // variable number
				rmatval[cc] = -1;
				cc++;

				rmatind[cc] = n * i + classes[z + indexes_prev]; // variable number
				rmatval[cc] = 1;
				cc++;

#ifndef NDEBUG
				sprintf(cnames[prov2], "dependent_decision_%d", prov2 + 1);
#endif

				prov2++;
			} // k (classes)
		} // n (items)

	} // i (knapsacks)

	/* add rows for multiple knapsack constraints
	 * */
	status = CPXaddrows(env, lp, 0, rcnt, nzcnt, rhs, sense, rmatbeg, rmatind, rmatval, NULL, cnames);
	if (status) {
		std::cout << "error: GMKP CPXaddrows (4-th constraint) failed...exiting" << std::endl;
		exit(1);
	}

	// free rows stuff
	delete[] rmatbeg;
	delete[] sense;
	delete[] rhs;
	delete[] rmatind;
	delete[] rmatval;
#ifndef NDEBUG
	for (int i = 0; i < rcnt; i++)
		delete[] cnames[i];
	delete cnames;
#endif

#ifndef NDEBUG
	status = CPXwriteprob(env, lp, modelFilename, NULL);
	if (status) {
		std::cout << "error: GMKP failed to write MODEL file" << std::endl;
	}
#endif

#ifndef NDEBUG
	status = CPXsetlogfilename(env, logFilename, "w");
	if (status) {
		std::cout << "error: GMKP failed to write LOG file" << std::endl;
	}
#endif

	/*******************************************/
	/*   solve program with CPLEX    */
	/*******************************************/

	/* set CPLEX number of threads
	 * */
	status = CPXsetintparam(env, CPX_PARAM_THREADS, 1);
	if (status) {
		std::cout << "error: GMKP failed to set CPX threads parameter...exiting" << std::endl;
		exit(1);
	}

	status = CPXsetintparam(env, CPX_PARAM_SCRIND, 0);
	if (status) {
		std::cout << "error: GMKP failed to disable CPX output parameter...exiting" << std::endl;
		exit(1);
	}

	/* set CPLEX time limit (in seconds)
	 * */
	if (TL > 0)
		status = CPXsetdblparam(env, CPX_PARAM_TILIM, TL);
	if (status) {
		std::cout << "error: GMKP failed to set CPX time limit parameter...exiting" << std::endl;
		exit(1);
	}

	/* solve with CPLEX "lpopt"
	 * */
	start = clock();
	status = CPXlpopt(env, lp);
	end = clock();
	time = ((double)(end - start)) / CLOCKS_PER_SEC;


	if (status) {
		std::cout << "error: GMKP failed to optimize...exiting" << std::endl;
		exit(1);
	}

	/*******************************************/
	/*  access CPLEX results                   */
	/*******************************************/

	/* SOLUTION STATUS
	 * access solution status
	 * */
	int solstat;

	solstat = CPXgetstat(env, lp);

	/* OBJECTIVE VALUE
	 * access objective function value
	 * */
	status = CPXgetobjval(env, lp, &objval);
	if (status) {
		std::cout << "error: GMKP failed to obtain objective value...exiting" << std::endl;
		exit(1);
	}

	/* BEST BOUND
	 * access the currently best known bound of all the remaining open nodes in a branch-and-bound tree
	 * */
	 double objval_p;

	CPXgetbestobjval(env, lp, &objval_p);
	if (status) {
		std::cout << "error: GMKP failed to obtain best known bound...exiting" << std::endl;
		exit(1);
	}

	/*
	* GET VECTOR X
	* access the vector x to find solution
	*/

	double *x = new double[ccnt];

	status = CPXsolution(env, lp, &solstat, &objval_p, x, NULL, NULL, NULL);
	if (status) {
		std::cout << "error: GMKP failed to check contraints of solution...exiting" << std::endl;
		exit(1);
	}

	int statusCheck = checkSolution(x, objval, n, m, r, b, weights, profits, capacities, setups, classes, indexes);

	if (statusCheck == 0)
		std::cout << "Iteration 1: all constraints are ok" << std::endl;
	else if (statusCheck == 1)
		std::cout << "Iteration 1: constraint violated: weights of the items are greater than the capacity..." << std::endl;
	else if (statusCheck == 2)
		std::cout << "Iteration 1: constraint violated: item is assigned to more than one knapsack..." << std::endl;
	else if (statusCheck == 3)
		std::cout << "Iteration 1: constraint violated: class is assigned to more than one knapsack..." << std::endl;
	else if (statusCheck == 4)
		std::cout << "Iteration 1: constraint violated: items of class are not assigned to knapsack..." << std::endl;
	else if (statusCheck == 5)
		std::cout << "Iteration 1: optimal solution violeted..." << std::endl;

	/*******************************************/
	/*   change Upper/Lower bown with CPLEX    */
	/*******************************************/

	bool allInt = true;
	int indexBestValue = 0;
	double bestValue = -1;
	int *indices = new int[1];
	double *bd = new double[1];
	int iteration = 2;

	int truncated = (int)objval;
	while (objval != truncated) {

		allInt = true;
		indexBestValue = 0;
		bestValue = -1;

		/*for (int i = 0; i < n*m + m * r; i++) {
			std::cout << x[i] << std::endl;
		}
		printf("\n\n");*/

		// check y*
		for (int i = 0; i < m*r; i++) {
			double value = x[m * n + i];
			int truncatedValue = (int)value;

			// if y* is 1
			if (value == 1) {
				bd[0] = 1;
				indices[0] = m * n + i;
				status = CPXchgbds(env, lp, 1, indices, "L", bd);
				if (status) {
					std::cout << "error: GMKP failed to change CPX bounds...exiting" << std::endl;
					exit(1);
				}
			}
			// find best value
			else if (truncatedValue != value) {
				allInt = false;
				if (x[m * n + i] > bestValue) {
					bestValue = x[m * n + i];
					indexBestValue = m * n + i;
				}

			} // if truncated

		} // for y*

		// all y* are integers
		// check x*
		if (allInt) {

			for (int i = 0; i < n*m; i++) {
				double value = x[i];
				int truncatedValue = (int)value;

				// if x* is 1
				if (value == 1) {
					bd[0] = 1;
					indices[0] = i;
					status = CPXchgbds(env, lp, 1, indices, "L", bd);
					if (status) {
						std::cout << "error: GMKP failed to change CPX bounds...exiting" << std::endl;
						exit(1);
					}
				}
				// find best value
				else if (truncatedValue != value) {
					allInt = false;
					if (x[i] > bestValue) {
						bestValue = x[i];
						indexBestValue = i;
					}

				} // if truncated

			} // for x*

		}

		// there is a fractional value
		if (!allInt) {
			x[indexBestValue] = 1;

			// check contraint 1
			int statusCheck = checkSolution(x, objval, n, m, r, b, weights, profits, capacities, setups, classes, indexes);

			if (statusCheck == 1) {
				bd[0] = 0;
				indices[0] = indexBestValue;
				status = CPXchgbds(env, lp, 1, indices, "U", bd);
				if (status) {
					std::cout << "error: GMKP failed to change CPX bounds...exiting" << std::endl;
					exit(1);
				}
			}
		}

		//

#ifndef NDEBUG
		status = CPXwriteprob(env, lp, modelFilename, NULL);
		if (status) {
			std::cout << "error: GMKP failed to write MODEL file" << std::endl;
		}
#endif

#ifndef NDEBUG
		status = CPXsetlogfilename(env, logFilename, "w");
		if (status) {
			std::cout << "error: GMKP failed to write LOG file" << std::endl;
		}
#endif

		/* solve with CPLEX "lpopt"
		* */
		status = CPXlpopt(env, lp);

		if (status) {
			std::cout << "error: GMKP failed to optimize...exiting" << std::endl;
			exit(1);
		}

		/*******************************************/
		/*  access CPLEX results                   */
		/*******************************************/

		/* SOLUTION STATUS
		* access solution status
		* */

		solstat = CPXgetstat(env, lp);

		/* OBJECTIVE VALUE
		 * access objective function value
		 * */
		status = CPXgetobjval(env, lp, &objval);
		if (status) {
			std::cout << "error: GMKP failed to obtain objective value...exiting" << std::endl;
			exit(1);
		}

		/* BEST BOUND
		 * access the currently best known bound of all the remaining open nodes in a branch-and-bound tree
		 * */

		CPXgetbestobjval(env, lp, &objval_p);
		if (status) {
			std::cout << "error: GMKP failed to obtain best known bound...exiting" << std::endl;
			exit(1);
		}

		status = CPXsolution(env, lp, &solstat, &objval_p, x, NULL, NULL, NULL);
		if (status) {
			std::cout << "error: GMKP failed to check contraints of solution...exiting" << std::endl;
			exit(1);
		}

		statusCheck = checkSolution(x, objval, n, m, r, b, weights, profits, capacities, setups, classes, indexes);

		if (statusCheck == 0)
			std::cout << "Iteration " << iteration << ": all constraints are ok" << std::endl;
		else if (statusCheck == 1)
			std::cout << "Iteration " << iteration << ": constraint violated: weights of the items are greater than the capacity..." << std::endl;
		else if (statusCheck == 2)
			std::cout << "Iteration " << iteration << ": constraint violated: item is assigned to more than one knapsack..." << std::endl;
		else if (statusCheck == 3)
			std::cout << "Iteration " << iteration << ": constraint violated: class is assigned to more than one knapsack..." << std::endl;
		else if (statusCheck == 4)
			std::cout << "Iteration " << iteration << ": constraint violated: items of class are not assigned to knapsack..." << std::endl;
		else if (statusCheck == 5)
			std::cout << "Iteration " << iteration << ": optimal solution violeted..." << std::endl;

		iteration++;

		truncated = (int)objval;
	}

	// print output
	std::cout << "Result: " << objval << std::endl;
	std::cout << "Elapsed time: " << time << std::endl;

	delete[] x;
	delete[] indices;
	delete[] bd;

	//

	/*free CPLEX
	 * */
	CPXfreeprob(env, &lp);

	CPXcloseCPLEX(&env);

	return status;
}
