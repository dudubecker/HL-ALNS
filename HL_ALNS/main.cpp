#include <iostream>
#include "Instance.hpp"
#include "Sol.hpp"
#include <iterator>

using namespace std;

int main(){
	
	//// Problem data
	
	std::string file_name = "INST_JO_NA_9_cpp.txt";
	
	int number_of_periods = ;
	
	// Randomness parameter
	double p {6};
	
	// Proximity weight
	double Gamma1 {0.5};
	
	// Met demand weight
	double Gamma2 {0.5};
	
	Instance inst(file_name, number_of_periods);
	
	Sol S(inst, p, Gamma1, Gamma2);
	
	S.printSol();
	
	printDouble(S.G);
	
	// int route_index = 1; 
	// int removal_index = 7;
	
	
	// S.removeNodeAt(route_index, removal_index);
	
	// S.printSol();
	
	
	S.toTXT(file_name);
	
	
	return 0;
	
}