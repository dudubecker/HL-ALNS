#include <iostream>
#include "Instance.hpp"
#include "Sol.hpp"
#include "Heuristic.hpp"
#include <iterator>

using namespace std;

int main(){
	
	
	//// Problem data
	
	std::string file_name = "INST_MA_RE_20_cpp.txt";
	
	int number_of_periods = 7;
	
	// Randomness parameter
	double p {4};
	
	// Proximity weight
	double Gamma1 {0.3};
	
	// Met demand weight
	double Gamma2 {0.7};
	
	Instance inst(file_name, number_of_periods);
	
	Sol S(inst, p, Gamma1, Gamma2);
	
	S.printSol();
	
	// S.printSol();
	
	// printDouble(S.W);
	
	BasicGreedyInsertion bgi {};
	
	bgi.apply(S);
	
	// PartialRandomRemoval prr {};
	
	// prr.apply(S);
	
	// S.printSol();
	
	// bgi.apply(S);
	
	// S.printSol();
	
	// std::cout << "\n";
	
	S.printSol();
	
	/*
	int node_index = 2;
	int route_index = 2;
	int insertion_index = 11;
	double demand = 600;
	
	S.insertNodeAt(node_index, route_index, insertion_index, demand);
	
	S.printSol();
	
	node_index = 22;
	route_index = 2;
	insertion_index = 12;
	demand = 450;
	
	
	S.insertNodeAt(node_index, route_index, insertion_index, demand);
	
	S.printSol();
	*/
	
	
	int cont = 0;
	for (auto &epsilon: S.epsilon){
		
		if (epsilon > -9999){
			
			std::cout << cont << ": " << epsilon*100 << "%\n";
			
		}
		cont += 1;
	}
	
	//std::cout << *std::max_element(S.epsilon.begin(), S.epsilon.end()) << std::endl;
	
	
	return 0;
	
}