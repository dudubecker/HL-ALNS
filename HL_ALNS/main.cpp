#include <iostream>
#include "Instance.hpp"
#include "Sol.hpp"
#include "Heuristic.hpp"
#include <iterator>

using namespace std;

int main(){
	
	
	//// Problem data
	
	std::string file_name = "INST_MA_RE_20_cpp.txt";
	
	int number_of_periods = 5;
	
	// Randomness parameter
	double p {4};
	
	// Proximity weight
	double Gamma1 {0.3};
	
	// Met demand weight
	double Gamma2 {0.7};
	
	Instance inst(file_name, number_of_periods);
	
	Sol S(inst, p, Gamma1, Gamma2);
	
	S.printSol();
	// printDouble(S.W);
	
	// PartialRandomRemoval prr {};
	
	// prr.apply(S);
	
	// S.printSol();
	
	ConcentricRemoval cr(45);
	
	cr.apply(S);
	
	S.printSol();
	
	BasicGreedyInsertion bgi {};
	
	bgi.apply(S);
	
	// std::cout << "\n\n\n\n\n";
	
	// std::vector<int> nodes_vector {11, 8};
	
	// std::cout << S.containsAll(nodes_vector) << std::endl;
	
	// printDouble(S.W);
	
	
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