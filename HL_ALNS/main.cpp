#include <iostream>
#include "Instance.hpp"
#include "Sol.hpp"
#include "Heuristic.hpp"
#include <iterator>

using namespace std;

int main(){
	
	
	//// Problem data
	
	std::string file_name = "INST_JO_NA_9_cpp.txt";
	
	int number_of_periods = 2;
	
	// Randomness parameter
	double p {4};
	
	// Proximity weight
	double Gamma1 {0.5};
	
	// Met demand weight
	double Gamma2 {0.5};
	
	Instance inst(file_name, number_of_periods);
	
	Sol S(inst, p, Gamma1, Gamma2);
	
	S.printSol();
	
	WorstRemoval wr {};
	wr.apply(S);
	
	// PartialRandomRemoval prr {};
	
	// prr.apply(S);
	
	//S.printSol(); 
	
	// BasicGreedyInsertion bgi {};
	
	// bgi.apply(S);
	
	// S.printSol();
	
	
	// printInt(S.unmet_demand_clients);
	
	return 0;
	
}