#include <iostream>
#include "Instance.hpp"
#include "Sol.hpp"
#include <iterator>

using namespace std;

int main(){
	
	//// Problem data
	
	std::string file_name = "INST_JO_NA_9_cpp.txt";
	
	int number_of_periods = 2;
	
	// Randomness parameter
	double p {6};
	
	// Proximity weight
	double Gamma1 {0.5};
	
	// Met demand weight
	double Gamma2 {0.5};
	
	Instance inst(file_name, number_of_periods);
	
	Sol S(inst, p, Gamma1, Gamma2);
	
	S.printSol();
	
	printDouble(S.Z);
	
	std::cout << "\n\n";
	
	printDouble(S.W);
	
	std::cout << "\n\n\n\n\n\n\n\n\n";
	
	
	int node_index = 11; 
	int route_index = 0; 
	int removal_index = 16;
	
	S.removeNode(node_index, route_index, removal_index);
	
	S.printSol();
	
	printDouble(S.Z);
	
	std::cout << "\n\n";
	
	printDouble(S.W);
	
	std::cout << "\n\n";
	
	
	
	// S.toTXT(file_name);
	
	
	
	return 0;
	
}