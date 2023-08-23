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
	
	int number_of_periods = 10;
	
	// Randomness parameter
	double p {4};
	
	// Proximity weight
	double Gamma1 {0.3};
	
	// Met demand weight
	double Gamma2 {0.7};
	
	Instance inst(file_name, number_of_periods);
	
	Sol S(inst, p, Gamma1, Gamma2);
	
	S.printSol();
	
	// wr.apply(S_i);
	
	// S_i.printSol();
	
	// S_i.toTXT(file_name);
	
	
	PartialRandomRemoval prr {};
	
	ConcentricRemoval cr(300);
	
	BasicGreedyInsertion bgi_1(1,1,1);
	
	BasicGreedyInsertion bgi_2(0.8,1,1);
	
	double target_epsilon = 0.16;
	
	double epsilon_range = 0.01;
	
	// Incumbent and best solution receives constructed S at beginning
	
	Sol S_best = S;
	Sol S_star = S;
	
	// Epsilons for solutions
	double max_epsilon_S_best = 9999;
	double max_epsilon_S_star = 9999;
	
	// Objective functions for solutions
	double FO_S_best = 0;
	double FO_S_star = 0;
	
	for (auto i {0}; i < 300; i++){
		
		std::cout << "Iteration: " << i << std::endl;
		
		std::string a;
		
		S = S_star;
		
		// Alternately applying concentric and random removal
		if (i%2==0){
			
			prr.apply(S);
			bgi_1.apply(S);
			
		}else{
			
			cr.apply(S);
			bgi_2.apply(S);
		}
		
		
		//S.printSol();
		
		//std::cin >> a;
		
		// Calculating traveling costs
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
		
		double FO = S.totalZ - 0.01*traveling_costs;
		
		double max_epsilon = *std::max_element(S.epsilon.begin(), S.epsilon.end());
		
		int pen_unmet_demand = 1000;
		
		for (auto  &met_demand: S.Z){
			
		 	if (met_demand == 0){
				
		 		FO -= pen_unmet_demand;
				
		 	}
		}
		
		// Incumbent solution update
		if (((max_epsilon < max_epsilon_S_star)) or ((FO > FO_S_star) and (max_epsilon <= target_epsilon))){
			
			std::cout << "Incumbent solution has been updated!" << std::endl;
			
			S_star = S;
			max_epsilon_S_star = max_epsilon;
			FO_S_star = FO;
		}
		
		// Best known solution update
		if ((FO > FO_S_best) and (max_epsilon <= target_epsilon)){
			
			std::cout << "Best found solution has been updated!" << std::endl;
			
			S_star = S;
			S_best = S;
			max_epsilon_S_best = max_epsilon;
			FO_S_best = FO;
			
		}
	}
	
	S_best.printSol();
	
	
	// srand(time(NULL));
	
	/*
	
	for (auto i {0}; i < 100; i++){
		
		std::cout << "Iteration: " << i << std::endl;
		
		std::string a;
		
		S = S_it;
		
		//if (i%15 == 0){
		
		//	Gamma1 = ((double) rand() / (RAND_MAX));
		//	Gamma2 = 1 - Gamma1;
			
		//	std::cout << Gamma1 << " " << Gamma2 << std::endl;
			
		//	Sol newS(inst, p, Gamma1, Gamma2);
			
		//	S = newS;
		
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
		
		// std::cin >> a;
		
		bgi.apply(S);
		
		// S.printSol();
		
		// printDouble(S.G);
		
		// std::cin >> a;
		
		double FO = S.totalZ - 0.01*traveling_costs;
		
		double target_epsilon = 0.1;
		
		double max_epsilon = *std::max_element(S.epsilon.begin(), S.epsilon.end());
		
		// int pen_unmet_demand = 1750;
		
		//for (auto  &met_demand: S.Z){
			
		// 	if (met_demand == 0){
				
		// 		FO -= pen_unmet_demand;
				
		// 	}
		//	
		//}
		
		std::cout << "FO: " << FO << std::endl;
		
		
		if ((FO > max_FO)){
			
			max_FO = FO;
			
			if (max_epsilon <= target_epsilon){
				
				BKS = S;
				
				it_BKS = i;
				
			}
			
		 	
		 	S_it = S;
			
			
			
		}
		
		
		
	}
	
	
	BKS.printSol();
	
	std::cout << "It: " << it_BKS << std::endl;
	
	// S.printSol();
	
	BKS.toTXT(file_name);
	
	// printInt(S.unmet_demand_clients);
	
	*/
	 
	return 0;
	
}