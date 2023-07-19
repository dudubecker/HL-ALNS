#include "Sol.hpp"
#include <ctime>
#include <random>
#include <chrono>
#include <algorithm>




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
	std::fill(Z.begin(), Z.end(), 9999);
	
	// Filling depot met demands with very large number
	
	for (auto &i: inst.D){
		
		Z.at(i) = 0;
		
	}
	
	// Amount of goods, at the beginning only defined in pickup nodes
	
	G.resize(inst.counties.size());
	for (auto &i: inst.P){
		
		G.at(i) = inst.d.at(i);
		
	}
	
	// W vector, with routes lengths
	W.resize(inst.m);
	std::fill(W.begin(), W.end(), 0);
	
	
	//// Construction heuristic main loop
	
	double routes_max_length = inst.T*inst.w_b.at(0);
	
	bool unfinished_routes = true;
	
	bool unmet_demand = true;
	
	while (unfinished_routes && unmet_demand){
		
		// Choosing random route
		int route_index = rand()%inst.m;
		
		// Route needs to be unfinished to be selected
		while (W.at(route_index) > routes_max_length){
			
			route_index = rand()%inst.m;
			
		}
		
		// Inserting nearest pickup node on route
		
		int last_node = R.at(route_index).back();
		
		int next_pickup_node = 9999;
		
		for (auto &node: inst.proximitiesN.at(last_node)){
			
			if ((count(inst.P.begin(), inst.P.end(), node)) && (G.at(node) > 0)){
				
				next_pickup_node = node;
				
				break;
				
			}
			
		}
		
		// Inserting node
		R.at(route_index).push_back(next_pickup_node);
		
		// Amount of load picked up - minimum between vehicle capacity and amount of goods in pickup node
		double picked_up_load = std::min(inst.Q.at(route_index), G.at(next_pickup_node));
		
		// Adding amount collected to attribute z
		z.at(route_index).push_back(picked_up_load);
		
		// Subtracting capacity in attribute G
		G.at(next_pickup_node) -= picked_up_load;
		
		// Adding travel times to attribute w
		W.at(route_index) += inst.t.at(last_node).at(next_pickup_node).at(route_index);
		
		
		// Inserting all possible delivery nodes
		
		double available_load = picked_up_load;
		
		
		// While the capacity can be assigned to node
		while (available_load > 0){
			
			int last_node = R.at(route_index).back();
			
			// Choosing delivery node
			
			// Possible improvement would be to sort only delivery nodes!
			
			std::vector<int> sorted_proximities = inst.proximitiesN.at(last_node);
			
			std::vector<int> sorted_met_demands = sortIndexes(Z);
			
			std::vector<double> scores {};
			
			// Proximity weight
			double Gamma1 {0.8};
			
			// Met demand weight
			double Gamma2 {0.2};
			
			// Randomness parameter
			double p {8};
			
			
			// Calculating score for each possible node
			
			for (auto &node: inst.N){
				
				double score {};
				
				double index_proximity = std::find(sorted_proximities.begin(),sorted_proximities.end(), node) - sorted_proximities.begin();
				
				// double index_met_demand = std::find(sorted_met_demands.begin(),sorted_met_demands.end(), node) - sorted_met_demands.begin();
				
				// If node is not depot or pickup - this enables checking that statement very quickly, instead of looking for node in setP/setS
				if (Z.at(node) != 9999){
					
					//score = Gamma1*index_proximity + Gamma2*index_met_demand;
					score = (1/Gamma1)*(index_proximity/inst.N.size()) + (1/Gamma2)*(Z.at(node));
					
					
				} else {
					
					score = 9999;
					
				}
				
				scores.push_back(score);
				
			}
			
			// Variable with sorted scores - It's good to remind that, the lower the score, most probably is for the node to be chosen
			std::vector<int> sorted_scores = sortIndexes(scores);
			
			// printInt(sorted_scores);
			
			// Removing non delivery nodes from vector
			for (int i = 0; i < inst.N.size() - inst.D.size(); i++){
				
				sorted_scores.pop_back();
				
			}
			
			
			// Removing last chosen delivery from sorted_scores
			sorted_scores.erase(std::remove_if(sorted_scores.begin(), sorted_scores.end(), [&last_node](int value) -> bool { return value == last_node; }), sorted_scores.end());
			
			
			// printInt(sorted_scores);
			
			// Choosing node with similar approach as ALNS
			
			double epsilon = {};
			epsilon = ((double) rand() / (RAND_MAX));
			
			// Index of chosen node in sorted scores list
			
			int index_delivery_node = {};
			
			int next_delivery_node = {};
			
			// If demand of node is already met, other node needs to be chosen
			
			bool unvalid_node = true;
			
			while (unvalid_node){
				
				index_delivery_node = round(pow(epsilon, p)*((sorted_scores).size()-1));
				
				next_delivery_node = sorted_scores.at(index_delivery_node);
				
				if (Z.at(next_delivery_node) < 0.999){
					
					unvalid_node = false;
					
				}
				
				
			}
			
			
			
			// Adding node to route
			
			// Inserting node
			R.at(route_index).push_back(next_delivery_node);
			
			// Amount of delivered load - minimum between available load and amount of goods demanded
			double delivered_load = std::min(available_load, (std::abs(inst.d.at(next_delivery_node)) - G.at(next_delivery_node)));
			
			// Adding amount collected to attribute z
			z.at(route_index).push_back(-delivered_load);
			
			// Adding relative covered demand to attribute Z
			Z.at(next_delivery_node) += std::abs(delivered_load/inst.d.at(next_delivery_node));
			
			// Adding delivery in attribute G
			G.at(next_delivery_node) += delivered_load;
			
			// Adding travel times to attribute w
			W.at(route_index) += inst.t.at(last_node).at(next_delivery_node).at(route_index);
			
			// Subtracting delivered load from available load variable
			available_load -= delivered_load;
			
			
			
			
		}
		
		
		
		
		
		// Checking if all routes are finished (regarding time)
		// or demands are fully met (constructive heuristic breakpoint) !!!!!!
		
		// Checking met demands
		
		unmet_demand = false;
		
		for (auto &value: Z){
			
			if (value < 1){
				
				unmet_demand = true;
				
			}
			
		}
		
		unfinished_routes = false;
		
		// Checking route lenghts
		for (auto route_length: W){
			
			if (route_length < routes_max_length){
				
				unfinished_routes = true;
				
			}
			
			
		}
		
		
		
		// Turns loop off:
		// unfinished_routes = false;
		
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
