#include <iostream>
#include "Instance.hpp"
#include "Sol.hpp"
#include "Heuristic.hpp"
#include <iterator>

using namespace std;

int main(){
	
	
	//// Problem data
	
	std::string file_name = "INST_CR_IM_IR_JO_MA_MA_NA_RE_TE_82_cpp.txt";
	
	int number_of_periods = 15;
	
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
	
	PartialRandomRemoval prr {};
	
	BasicGreedyInsertion bgi {};
	
	
	/*
	for (auto i {0}; i < 1; i++){
		
		wr.apply(S);
		
		bgi.apply(S);
		
		prr.apply(S);
		
		bgi.apply(S);
		
		
	}
	*/
	
	// printInt(S.unmet_demand_clients);
	
	return 0;
	
}