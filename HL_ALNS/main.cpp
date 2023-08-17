#include <iostream>
#include <ctime>
#include "Instance.hpp"
#include "Sol.hpp"
#include "Heuristic.hpp"
#include <iterator>

using namespace std;

int main(){
	
	
	//// Problem data
	
	std::string file_name = "INST_MA_RE_20_cpp.txt";
	
	int number_of_periods = 20;
	
	// Randomness parameter
	double p {4};
	
	// Proximity weight
	double Gamma1 {0.5};
	
	// Met demand weight
	double Gamma2 {0.5};
	
	Instance inst(file_name, number_of_periods);
	
	Sol S(inst, p, Gamma1, Gamma2);
	
	S.printSol();
	
	WorstRemoval wr(1,1,1);
	
	PartialRandomRemoval prr {};
	
	BasicGreedyInsertion bgi(2,1,1);
	
	ConcentricRemoval crr(300);
	
	Sol BKS = S;
	
	Sol S_it = S;
	
	int it_BKS = 0;
	
	// srand(time(NULL));
	
	for (auto i {0}; i < 500; i++){
		
		std::cout << "Iteration: " << i << std::endl;
		
		std::string a;
		
		S = S_it;
		
		// if (i%15 == 0){
		
		// Gamma1 = ((double) rand() / (RAND_MAX));
		// Gamma2 = 1 - Gamma1;
		
		// std::cout << Gamma1 << " " << Gamma2 << std::endl;
		
		// Sol newS(inst, p, Gamma1, Gamma2);
		// 
		// S = newS;
		
		//}
		
		
		double traveling_costs = {};
		
		for (int route_index {0}; route_index < S.R.size(); route_index++){
			
			// double route_traveling_time = 0;
			
			for (int node_index {0}; node_index < S.R.at(route_index).size() - 1; node_index++){
				
				int first_arc_node = S.R.at(route_index).at(node_index);
				
				int second_arc_node = S.R.at(route_index).at(node_index  + 1);
				
				double arc_costs = S.inst.c.at(first_arc_node).at(second_arc_node).at(route_index);
				
				double arc_time = S.inst.t.at(first_arc_node).at(second_arc_node).at(route_index);
				
				traveling_costs += arc_costs;
				//route_traveling_time += arc_time;
				
			}
			
			// std::cout << "Route " << route_index << ": " << route_traveling_time << std::endl;
			
		}
		
		
		
		if ((i % 2 == 0) and (i % 3 != 0)){
			
			wr.apply(S);
			
			
		} else if (i % 3 == 0){
			
			prr.apply(S);
			
		} else {
			
			crr.apply(S);
			// prr.apply(S);
		}
		
		// S.printSol();
		
		//std::cin >> a;
		
		bgi.apply(S);
		
		// S.printSol();
		
		// printDouble(S.G);
		
		
		// std::cin >> a;
		
		double FO = S.totalZ - 0.001*traveling_costs;
		
		double target_epsilon = 0.1;
		
		double max_epsilon = *std::max_element(S.epsilon.begin(), S.epsilon.end());
		
		//int pen_unmet_demand = 1200;
		
		//for (auto  &met_demand: S.Z){
			
		// 	if (met_demand == 0){
				
		// 		FO -= pen_unmet_demand;
				
		// 	}
			
		//}
		
		std::cout << "FO: " << FO << std::endl;
		
		
		if ((FO > BKS.totalZ) and max_epsilon <= target_epsilon){
			
		 	BKS = S;
			
		 	S_it = S;
			
			it_BKS = i;
			
		}
		
		
		
	}
	
	
	BKS.printSol();
	
	std::cout << "It: " << it_BKS << std::endl;
	
	// S.printSol();
	
	// BKS.toTXT(file_name);
	
	// printInt(S.unmet_demand_clients);
	
	
	
	 
	return 0;
	
}