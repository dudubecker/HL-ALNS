#include <iostream>
#include "Instance.hpp"
#include "Sol.hpp"
#include "Heuristic.hpp"
#include <iterator>

using namespace std;

int main(){
	
	
	//// Problem data
	
	std::string file_name = "INST_JO_NA_9_cpp.txt";
	
	
	int number_of_periods = 4;
	
	// Randomness parameter
	double p {4};
	
	// Proximity weight
	double Gamma1 {0.6};
	
	// Met demand weight
	double Gamma2 {0.4};
	
	Instance inst(file_name, number_of_periods);
	
	Sol S(inst, p, Gamma1, Gamma2);
	
	S.printSol();
	
	/*
	
	WorstRemoval wr {};
	
	PartialRandomRemoval prr {};
	
	BasicGreedyInsertion bgi {};
	
	Sol BKS = S;
	
	for (auto i {0}; i < 5; i++){
		
		
		prr.apply(S);
		
		bgi.apply(S);
		
		// wr.apply(S);
		
		// bgi.apply(S);
		
		if (S.totalZ > BKS.totalZ){
			
			BKS = S;
			
		}
		
			
		
		
	}
	
	
	// BKS.printSol();
	
	S.printSol();
	
	// S.toTXT(file_name);
	
	// printInt(S.unmet_demand_clients);
	
	 * 
	 * */
	 
	return 0;
	
}