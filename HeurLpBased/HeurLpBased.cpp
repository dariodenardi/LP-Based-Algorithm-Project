#include <iostream>

#include <time.h>

#include "pugixml.hpp"

#include "INSTANCE.h"
#include "GMKP_CPX.h"
#include "CHECK_CONS.h"

using namespace std;

int main(int argc, char **argv)
{
	if (argc < 2) {
		std::cout << "invalid parameters!\n";
		std::cout << "parameters: [nameInstance]\n";
		return -1;
	}

	// input parameters
	char *instance_name = argv[1];
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

	int num = 25;
	// data for GMKP instance
	int n; // number of objects
	int m; // number of knapsacks
	int r; // number of subsets
	int *b = &num; // item can be assign at most to bk knapsacks // num points to somewhere random
	int *profits = &num; // array for linear profit term
	int *weights = &num; // array of weights
	int *capacities = &num; // array of knapsack capacities
	int *setups = &num; // array of setup
	int *classes = &num; // array of classes
	int *indexes = &num; // array of indexes

	clock_t start, end;
	double time;
	
	char modelFilename[200];
	char logFilename[200];

	// read configuration
	pugi::xml_document doc;

	pugi::xml_parse_result result = doc.load_file("config.xml");
	if (result) {
		std::cout << "config.xml loaded correctly!\n";
	}
	else {
		std::cout << "config.xml not found or not written correctly!\n";
		return -2;
	}

	std::cout << "Parameters" << std::endl;

	// library
	std::string library = doc.child("Config").child("library").attribute("name").as_string();
	std::cout << "library used: " << library << std::endl;

	// timeout
	int TL = doc.child("Config").child("timeout").attribute("value").as_int();
	std::cout << "timeout: " << TL << "s" << std::endl;

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

	status = solveGMKP_CPX(n, m, r, b, weights, profits, capacities, setups, classes, indexes, modelFilename, logFilename, TL, false);

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