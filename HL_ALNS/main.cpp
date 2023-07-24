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
	
	
	// For printing nodesPositions attribute:
	for (auto node_index {0}; node_index < S.nodesPositions.size(); node_index++){
		
		std::cout << node_index << ": ";
		
		for(auto position_pair: S.nodesPositions.at(node_index)){
			
			
			std::cout << position_pair.first << " " << position_pair.second << "; ";
			
			
		}
		
		std::cout << "\n\n";
		
	}
	
	
	int route_index = 1; 
	int removal_index = 7;
	
	
	S.removeNodeAt(route_index, removal_index);
	
	S.printSol();
	
	// For printing nodesPositions attribute:
	for (auto node_index {0}; node_index < S.nodesPositions.size(); node_index++){
		
		std::cout << node_index << ": ";
		
		for(auto position_pair: S.nodesPositions.at(node_index)){
			
			
			std::cout << position_pair.first << " " << position_pair.second << "; ";
			
			
		}
		
		std::cout << "\n\n";
		
	}
	
	// S.toTXT(file_name);
	
	
	
	return 0;
	
}