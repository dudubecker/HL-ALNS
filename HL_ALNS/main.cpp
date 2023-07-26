#include <iostream>
#include "Instance.hpp"
#include "Sol.hpp"
#include "Heuristic.hpp"
#include <iterator>

using namespace std;

int main(){
	
	
	
	/*
	//// Problem data
	
	std::string file_name = "INST_JO_NA_9_cpp.txt";
	
	int number_of_periods = 3;
	
	// Randomness parameter
	double p {6};
	
	// Proximity weight
	double Gamma1 {0.5};
	
	// Met demand weight
	double Gamma2 {0.5};
	
	Instance inst(file_name, number_of_periods);
	
	Sol S(inst, p, Gamma1, Gamma2);
	
	// S.printSol();
	
	/*
	// For printing nodesPositions attribute:
	for (auto node_index {0}; node_index < S.nodesPositions.size(); node_index++){
		
		std::cout << node_index << ": \n\n";
		
		for (auto route_index {0}; route_index < S.inst.m; route_index++){
			
			if (S.nodesPositions.at(node_index).at(route_index).size() > 0){
				
				std::cout << "Route " << route_index << ": ";
				
				printInt(S.nodesPositions.at(node_index).at(route_index));
				
			}
			
			
		}
		
		std::cout << "\n";
		
	}

	
	// S.toTXT(file_name);
	
	
	// S.printSol();
	
	// // S.removeNodeCase(node_index);
	// S.removeNodeCase(node_index);
	
	// int route_index = 0;
	// int removal_index = 1;
	
	int node_index = 2;
	
	S.removeNodeCases(node_index);
	
	S.printSol();
	
	printDouble(S.Z);
	*/
	
	
	return 0;
	
}