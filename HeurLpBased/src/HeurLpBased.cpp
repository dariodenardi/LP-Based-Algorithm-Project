#include <iostream>

#include <time.h>

#include "INSTANCE.h"
#include "LPBASED_CPX.h"

using namespace std;

int main(int argc, char **argv)
{
	if (argc < 3) {
		std::cout << "invalid parameters!\n";
		std::cout << "parameters: [nameInstance] [timeout]\n";
		return -1;
	}

	// input parameters
	char *instanceName = argv[1];
	int TL = atoi(argv[2]);
	int instanceNameLength = 0;

	bool ok = true;
	while (ok) {
		if (instanceName[instanceNameLength] == '.')
			ok = false;
		else
			instanceNameLength++;
	}

	std::cout << "Input parameters: " << std::endl;
	std::cout << "instance name: " << instanceName << std::endl;
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
	
	char modelFilename[200];
	char logFilename[200];

	// read file
	int status = readInstance(instanceName, n, m, r, weights, capacities, profits, classes, indexes, setups, b);
	if (status) {
		std::cout << "File not found or not read correctly" << std::endl;
		return -3;
	}

	// model into a .lp file
	strcpy(modelFilename, "models/");
	strncat(modelFilename, instanceName, instanceNameLength);
	strcat(modelFilename, ".lp");

	// log into a .txt file
	strcpy(logFilename, "logs/");
	strncat(logFilename, instanceName, instanceNameLength);
	strcat(logFilename, ".txt");

	printInstance(n, m, r, weights, capacities, profits, classes, indexes, setups, b);

	status = solve(n, m, r, b, weights, profits, capacities, setups, classes, indexes, modelFilename, logFilename, TL);

	// print output
	if (status)
		std::cout << "An error has occurred! Error number : " << status << std::endl;
	else
		std::cout << "The function was performed correctly!" << std::endl;

	// free memory
	free(b);
	free(profits);
	free(weights);
	free(capacities);
	free(setups);
	free(classes);
	free(indexes);

	return 0;
}