#include <iostream>

#include <time.h>

#include "INSTANCE.h"
#include "LPBASED_CPX.h"
#include "CHECK_CONS.h"

using namespace std;

int main(int argc, char **argv)
{
	if (argc < 3) {
		std::cout << "invalid parameters!\n";
		std::cout << "parameters: [nameInstance] [timeout]\n";
		return -1;
	}

	// input parameters
	char *instance_name = argv[1];
	int TL = atoi(argv[2]);
	int stringLength = 0;

	bool ok = true;
	while (ok) {
		if (instance_name[stringLength] == '.')
			ok = false;
		else
			stringLength++;
	}

	std::cout << "Input parameters: " << std::endl;
	std::cout << "instance name: " << instance_name << std::endl;
	std::cout << std::endl;

	// data for GMKP instance
	int n; // number of objects
	int m; // number of knapsacks
	int r; // number of subsets
	int *b = NULL; // item can be assign at most to bk knapsacks
	int *profits = NULL; // array for linear profit term
	int *weights = NULL; // array of weights
	int *capacities = NULL; // array of knapsack capacities
	int *setups = NULL; // array of setup
	int *classes = NULL; // array of classes
	int *indexes = NULL; // array of indexes

	clock_t start, end;
	double time;
	double objval;
	double *x = NULL;
	
	char modelFilename[200];
	char logFilename[200];

	// read file
	int status = readInstance(instance_name, n, m, r, weights, capacities, profits, classes, indexes, setups, b);
	if (status) {
		std::cout << "File not found or not read correctly" << std::endl;
		return -3;
	}

	// model into a .lp file
	strcpy(modelFilename, "models/");
	strncat(modelFilename, instance_name, stringLength);
	strcat(modelFilename, ".lp");

	// log into a .txt file
	strcpy(logFilename, "logs/");
	strncat(logFilename, instance_name, stringLength);
	strcat(logFilename, ".txt");

	printInstance(n, m, r, weights, capacities, profits, classes, indexes, setups, b);

	status = solve(n, m, r, b, weights, profits, capacities, setups, classes, indexes, modelFilename, logFilename, TL, &objval, &x);

	// print output
	if (status)
		std::cout << "An error has occurred! Error number : " << status << std::endl;
	else
		std::cout << "The function was performed correctly!" << std::endl;

	int truncated = (int)objval;
	while (objval != truncated) {

		for (int i = 0; i < n*m + m*r; i++) {
			std::cout << x[i] << std::endl;
		}
		printf("\n\n");

		status = solve(n, m, r, b, weights, profits, capacities, setups, classes, indexes, modelFilename, logFilename, TL, &objval, &x);

		// print output
		if (status)
			std::cout << "An error has occurred! Error number : " << status << std::endl;
		else
			std::cout << "The function was performed correctly!" << std::endl;

		truncated = (int)objval;
	}

	std::cout << "Result: " << objval << std::endl;

	// free memory
	free(b);
	free(profits);
	free(weights);
	free(capacities);
	free(setups);
	free(classes);
	free(indexes);
	free(x);

	return 0;
}