#include "GMKP_CPX.h"

#define VARNAMES //use variable names
#define CONSNAMES //use constraint names
#define DEBUG //print cplex run on screen
#define WRITELP //write lp problem to file
#define WRITELOG //write log

int solveGMKP_CPX(int n, int m, int r, int * b, int * weights, int * profits, int * capacities, int * setups, int * classes, int * indexes, char * modelFilename, char * logFilename, int TL, bool intflag) {

	/*******************************************/
	/*     set CPLEX environment and lp        */
	/*******************************************/
	CPXENVptr env;
	CPXLPptr lp;
	int status;
	double objval;

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

#ifdef DEBUG
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
	/* order of the variables (x_ij) i=1...m (knapsack index), j = 1...n (object index) in CPLEX lp
	 *
	 * e.g., m=2, n=3
	 *
	 * [x_11, x_12, x_13, x_21, x_22, x_23]
	 *
	 * */

	/* order of the variables (y_ik) i=1...m (knapsack index), k = 1...r (classes index) in CPLEX lp
	 *
	 * e.g., r=2, k=2
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

#ifdef VARNAMES
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
			if (intflag)
				ctype[col] = 'B';

#ifdef VARNAMES
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
			if (intflag)
				ctype[col] = 'B';

#ifdef VARNAMES
			sprintf(vnames[col], "y_%d_%d", i + 1, k + 1);
#endif
			col++;
		} // k (classes)
	} // i (knapsacks)

	/* add columns to CPLEX LP
	 * */
	if (intflag)
		status = CPXnewcols(env, lp, ccnt, obj, lb, ub, ctype, vnames);
	else
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
#ifdef VARNAMES
	for (int i = 0; i < ccnt; i++)
		delete[] vnames[i];
	delete vnames;
#endif

	/*******************************************/
	/*   add CPLEX constraints                 */
	/*******************************************/

	/*	constraint (1):
		sum(j = 1 ... n) w_j * x_ij + sum(k = 1 ... r) s_k * y_ik <= C_i   for all i = 1 .. m
	 * */
	int rcnt = m; // number of constraints (rows)
	int nzcnt = ccnt; // number of total variables (columns)

	// allocate memory for constraint
	int *rmatbeg = new int[rcnt];
	double *rhs = new double[rcnt];
	char *sense = new char[rcnt];
	int *rmatind = new int[nzcnt];
	double *rmatval = new double[nzcnt];

#ifdef CONSNAMES
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

		// sum(j = 1 ... n) w_j * x_ij
		for (int j = 0; j < n; j++)
		{
			rmatind[cc] = i * n + j; // variable number
			rmatval[cc] = weights[j];
			cc++;
		}

		// sum(k = 1 ... r) s_k * y_ik 
		for (int k = 0; k < r; k++)
		{
			rmatind[cc] = n*m + i * r + k;
			rmatval[cc] = setups[k];
			cc++;
		}

#ifdef CONSNAMES
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
#ifdef CONSNAMES
	for (int i = 0; i < rcnt; i++)
		delete[] cnames[i];
	delete cnames;
#endif

	/*	constraint (2):
		sum(i = 1 ... m) x_ij <= 1       for all j = 1 .. n
	 * */

	rcnt = n; // number of constraints (rows)
	nzcnt = m * n; // number of total variables (columns)

	// allocate memory for constraint
	rmatbeg = new int[rcnt];
	rhs = new double[rcnt];
	sense = new char[rcnt];
	rmatind = new int[nzcnt];
	rmatval = new double[nzcnt];

#ifdef CONSNAMES
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

#ifdef CONSNAMES
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
#ifdef CONSNAMES
	for (int i = 0; i < rcnt; i++)
		delete[] cnames[i];
	delete cnames;
#endif

	/*	constraint (3):
		sum(i = 1 ... m) y_ik <= 1       for all k = 1 .. r
	 * */

	rcnt = r; // number of constraints (rows)
	nzcnt = m*r; // number of total variables (columns)

	// allocate memory for constraint
	rmatbeg = new int[rcnt];
	rhs = new double[rcnt];
	sense = new char[rcnt];
	rmatind = new int[nzcnt];
	rmatval = new double[nzcnt];

#ifdef CONSNAMES
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

#ifdef CONSNAMES
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
#ifdef CONSNAMES
	for (int i = 0; i < rcnt; i++)
		delete[] cnames[i];
	delete cnames;
#endif

	/*	constraint (4):
		sum(j belongs Rk) x_ij <= n * y_ik       for all k = 1 .. r
		sum(j belongs Rk) x_ij - n * y_ik <= 0   for all k = 1 .. r
	 * */

	rcnt = r*m; // number of constraints (rows)
	nzcnt = n*m + r*m; // number of total variables (columns)

	// allocate memory for constraint
	rmatbeg = new int[rcnt];
	rhs = new double[rcnt];
	sense = new char[rcnt];
	rmatind = new int[nzcnt];
	rmatval = new double[nzcnt];

#ifdef CONSNAMES
	cnames = new char*[rcnt];
	for (int i = 0; i < rcnt; i++)
		cnames[i] = new char[100];
#endif

	// init counter
	cc = 0;
	// fill in rows for multiple knapsack constraints
	for (int i = 0; i < m; i++) {
		for (int k = 0; k < r; k++) {

			// - n * y_ik       for all k = 1 .. r
			rmatbeg[i * r + k] = cc; // starting index of the n-th constraint
			sense[i * r + k] = 'L';
			rhs[i * r + k] = 0.0;

			rmatind[cc] = n * m + i * r + k; // variable number
			rmatval[cc] = -n;
			cc++;

			// sum(j belongs Rk) x_ij      for all k = 1 .. r
			for (int j = 0; j < n; j++) {

				int indexes_prev = k > 0 ? indexes[k - 1] : 0;
				for (int z = 0; z < indexes[k] - indexes_prev; z++) {

					if (classes[z + indexes_prev] == j) {
						rmatind[cc] = i * n + j; // variable number
						rmatval[cc] = 1.0;
						cc++;
					}
				}

			} // j (items)

#ifdef CONSNAMES
			sprintf(cnames[i * r + k], "dependent_decision_%d", i * r + k + 1);
#endif
		} // k (classes)
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
#ifdef CONSNAMES
	for (int i = 0; i < rcnt; i++)
		delete[] cnames[i];
	delete cnames;
#endif

#ifdef WRITELP
	CPXwriteprob(env, lp, modelFilename, NULL);
#endif

#ifdef WRITELOG
	status = CPXsetlogfilename(env, logFilename, "w");
	if (status) {
		std::cout << "error: GMKP failed to set LOG file" << std::endl;
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

	/* solve with CPLEX "mipopt" (if mixed-integer)
	 * */
	if (intflag)
		status = CPXmipopt(env, lp);
	else
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

	/* CUT OFF
	 * access the the MIP cutoff value being used during mixed integer optimization
	 * the cutoff is updated with the objective function value,
	 * each time an integer solution is found during branch and cut
	 *  */
	/*double cutoff_p;

	status = CPXgetcutoff(env, lp, &cutoff_p);
	if (status) {
		std::cout << "error: GMKP failed to obtain MIP cutoff value...exiting" << std::endl;
		exit(1);
	}*/

	/* RELATIVE GAP
	 * access the relative objective gap for a MIP optimization
	 * for a minimization problem, this value is computed by
	 * (bestinteger - bestobjective) / (1e-10 + |bestinteger|)
	 * where bestinteger is the value returned by CPXgetobjval
	 * and bestobjective is the value returned by CPXgetbestobjval
	 * for a maximization problem, the value is computed by
	 * (bestobjective - bestinteger) / (1e-10 + |bestinteger|)
	 * */

	/*double gap_p;

	status = CPXgetmiprelgap(env, lp, &gap_p);
	if (status) {
		std::cout << "error: GMKP failed to obtain relative objective gap...exiting" << std::endl;
		exit(1);
	}*/

	/* NUMBER OF NODES
	 * access the number of nodes used to solve a mixed integer problem
	 * */
	int nnodes = CPXgetnodecnt(env, lp);

	/*******************************************/
	/*  write output                           */
	/*******************************************/

	if (intflag == false)
		std::cout << "Root UB: " << objval << std::endl;
	std::cout << "best UB: " << objval_p << std::endl;
	if (intflag == true)
		std::cout << "cut off: " << objval << std::endl;
	std::cout << "opt: " << solstat << std::endl;
	std::cout << "BB-node: " << nnodes << std::endl;

	/*
	* GET VECTOR X
	* access the vector x to find solution
	*/

	if (intflag) {
		double *x;
		x = new double[ccnt];

		status = CPXsolution(env, lp, &solstat, &objval_p, x, NULL, NULL, NULL);
		if (status) {
			std::cout << "error: GMKP failed to check contraints of solution...exiting" << std::endl;
			exit(1);
		}

		int statusCheck = checkSolution(x, objval, n, m, r, b, weights, profits, capacities, setups, classes, indexes);

		if (statusCheck == 0) {
			std::cout << "All constraints are ok" << std::endl;
		}
		else if (statusCheck == 1) {
			std::cout << "Constraint violated: weights of the items are greater than the capacity..." << std::endl;
		}
		else if (statusCheck == 2) {
			std::cout << "Constraint violated: item is assigned to more than one knapsack..." << std::endl;
		}
		else if (statusCheck == 3) {
			std::cout << "Constraint violated: class is assigned to more than one knapsack..." << std::endl;
		}
		else if (statusCheck == 4) {
			std::cout << "Constraint violated: items of class are not assigned to knapsack..." << std::endl;
		}
		else if (statusCheck == 5) {
			std::cout << "Optimal solution violeted..." << std::endl;
		}

		delete[] x;
	}

	/*free CPLEX
	 * */
	CPXfreeprob(env, &lp);

	CPXcloseCPLEX(&env);

	return status;
}