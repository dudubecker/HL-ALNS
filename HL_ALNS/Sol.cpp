#include "Sol.hpp"
#include <ctime>
#include <random>
#include <chrono>

Sol::Sol(Instance &inst_val){
	
	// Replicable data
	srand(123);
	
	// True random data
	// srand(time(NULL);
	
	inst = inst_val;
	
	// Constructive heuristic
	
	// Starting routes with empty values
	
	for (size_t location = 0; location < inst.Q_loc.size(); location++){
		
		for (const& vehicle: inst.Q_loc.at(location)){
			
			// Creating empty route
			R.push_back({location});
			
			// Empty variable "z" for the first visit
			z.push_back({0});
			
		}
		
	}
	
	// Z vector with 0 met demands:
	Z.resize(inst.counties.size());
	std::fill(Z.begin(), Z.end(), 0);
	
	// Filling depot met demands with very large number
	std::vector<int> depots = inst.S_0;
	depots.insert(depots.end(), inst.S_f.begin(), inst.S_f.end());
	
	for (auto &i: depots){
		
		Z.at(i) = 9999;
		
	}
	
	// W vector, with routes lengths
	W.resize(inst.m);
	std::fill(W.begin(), W.end(), 0);
	
	
	//// Construction heuristic main loop
	
	double routes_max_length = inst.T*inst.w_b.at(0);
	
	bool unfinished_routes = true;
	
	while (unfinished_routes){
		
		
		// Choosing random route
		int route_index = rand()%inst.m;
		
		// Route needs to be unfinished to be selected
		while (W.at(route_index) > routes_max_length){
			
			route_index = rand()%inst.m;
			
		}
		
		// Inserting nearest pickup node
		// List of proximity (instance data)
		
		// While the capacity can be assigned to node
			// Choosing delivery node
			
			
			// Assigning all current capacity to node
		
		
		
		// Checking if all routes are finished (constructive heuristic breakpoint)
		
		unfinished_routes = false;
		
		for (auto route_length: W){
			
			if (route_length < routes_max_length){
				
				unfinished_routes = true;
				
			}
			
			
		}
		
		
		
		// Turns loop off:
		unfinished_routes = false;
		
	}
};

Sol::~Sol()
{
}


void Sol::printSol(){
	
	std::cout << "S: \n" << std::endl;
	
	for (auto &route: R){
		
		std::cout << "[ ";
		
		for (auto &node: route){
			
			std::cout << node << " ";
		}
		
		std::cout << "]\n";
		
	}
	
	
}
