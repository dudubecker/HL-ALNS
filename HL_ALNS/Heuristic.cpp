#include "Heuristic.hpp"
#include <ctime>
#include <random>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <utility>

//// Initializations

//// Heuristic object implementation

Sol Heuristic::apply(Sol &S){
    std::cout << "Comportamento base de Heuristic\n";
    // Coloque aqui o código base que deve ser comum a todas as classes derivadas

    // Chamada para o método "specificApply" específico da classe derivada
    // Isso garantirá que o comportamento específico da classe derivada seja chamado após o código base.
	specificApply(S);
	
	return S;
	
}

double Heuristic::deltaInsertion(std::string delta_type, Sol &S, int &node_index, int &route_index, int &insertion_index){
	
	double delta {};
	
	bool last_position = false;
	
	if (insertion_index == (S.RSize.at(route_index))){
		
		last_position = true;
		
	}
	
	
	if (delta_type == "time"){
		
		if (last_position){
		
			int last_node_index = S.R.at(route_index).back();
			
			delta = S.inst.t.at(last_node_index).at(node_index).at(route_index);
			
		
		// If it's not last position, arcs difference needs to be taken into account
		} else {
			
			int old_arc_first_node = S.R.at(route_index).at(insertion_index - 1);
			
			int old_arc_second_node = S.R.at(route_index).at(insertion_index);
			
			double old_arc_time = S.inst.t.at(old_arc_first_node).at(old_arc_second_node).at(route_index);
			
			double first_new_arc_time = S.inst.t.at(old_arc_first_node).at(node_index).at(route_index);
			
			double second_new_arc_time = S.inst.t.at(node_index).at(old_arc_second_node).at(route_index);
			
			delta = (first_new_arc_time + second_new_arc_time) - old_arc_time;
			
		}
		
		
	} else if (delta_type == "cost"){
		
		if (last_position){
			
			int last_node_index = S.R.at(route_index).back();
			
			delta = S.inst.c.at(last_node_index).at(node_index).at(route_index);
			
		
		// If it's not last position, arcs difference needs to be taken into account
		} else {
			
			int old_arc_first_node = S.R.at(route_index).at(insertion_index - 1);
			
			int old_arc_second_node = S.R.at(route_index).at(insertion_index);
			
			double old_arc_cost = S.inst.c.at(old_arc_first_node).at(old_arc_second_node).at(route_index);
			
			double first_new_arc_cost = S.inst.c.at(old_arc_first_node).at(node_index).at(route_index);
			
			double second_new_arc_cost = S.inst.c.at(node_index).at(old_arc_second_node).at(route_index);
			
			delta = (first_new_arc_cost + second_new_arc_cost) - old_arc_cost;
			
		}
		
	} else {
		
		std::cout << "Not a valid type for 'deltaInsertion' function" << std::endl;
		
	}
	
	// In this case, delta is a positive number!
	return delta;
	
}

double Heuristic::deltaRemoval(std::string delta_type, Sol &S, int &node_index, int &route_index, int &removal_index){
	
	double delta {};
	
	// Treating case when removal position is the last in the route - arc difference calculation is not the same!
	
	bool last_node = false;
	
	if (removal_index == (S.RSize.at(route_index) - 1)){
		
		last_node = true;
		
	}
	
	if (delta_type == "time"){
		
		// If removal position is the last one
		if (last_node){
			
			int last_but_one_node = S.R.at(route_index).at(removal_index - 1);
			
			delta = S.inst.t.at(last_but_one_node).at(node_index).at(route_index);
		
		// If removal position is in the middle of the route
		} else {
			
			// Arc time travel difference
			
			int first_node = S.R.at(route_index).at(removal_index - 1);
			
			int second_node = S.R.at(route_index).at(removal_index + 1);
			
			double first_old_arc_time = S.inst.t.at(first_node).at(node_index).at(route_index);
			
			double second_old_arc_time = S.inst.t.at(node_index).at(second_node).at(route_index);
			
			double new_arc_time = S.inst.t.at(first_node).at(second_node).at(route_index);
			
			std::cout << first_node << " " << second_node << std::endl;
			
			std::cout << first_old_arc_time << " " << second_old_arc_time << " " << new_arc_time << std::endl;
			
			delta = new_arc_time - (first_old_arc_time + second_old_arc_time);
			
		}
		
		
	} else if (delta_type == "cost"){
		
		
		// If removal position is the last one
		if (last_node){
			
			int last_but_one_node = S.R.at(route_index).at(removal_index - 1);
			
			delta = S.inst.c.at(last_but_one_node).at(node_index).at(route_index);
		
		// If removal position is in the middle of the route
		} else {
			
			// Arc time travel difference
			
			int first_node = S.R.at(route_index).at(removal_index - 1);
			
			int second_node = S.R.at(route_index).at(removal_index + 1);
			
			double first_old_arc_cost = S.inst.c.at(first_node).at(node_index).at(route_index);
			
			double second_old_arc_cost = S.inst.c.at(node_index).at(second_node).at(route_index);
			
			double new_arc_cost = S.inst.c.at(first_node).at(second_node).at(route_index);
			
			delta = new_arc_cost - (first_old_arc_cost + second_old_arc_cost);
			
		}
		
		
	} else {
		
		std::cout << "Not a valid type for 'deltaRemoval' function" << std::endl;
		
	}
	
	// Obs: delta will be a negative value!
	return delta;
	
}

//// RemovalHeuristic objects implementation

// Sobrescrita do método "specificApply" para a RemovalHeuristic
int RemovalHeuristic::initializeMethod(Sol &S) {
	
	std::cout << "Removal heuristics base code\n";
	
	// Replicable data
	srand(121);
	
	// True random data
	// srand(time(NULL));
	
	// Number of nodes in solution
	int number_of_nodes = std::accumulate(S.RSize.begin(), S.RSize.end(), 0);
	
	// Valor mínimo de mi (10% da quantidade de pedidos)
	int low_mi = round(number_of_nodes*0.2);
	
	// Valor máximo de mi (40% da quantidade de pedidos)
	int high_mi = round(number_of_nodes*0.3);
	
	int mi = low_mi + rand()%(high_mi - low_mi + 1);
	
	return mi;
	
}

// Sobrescrita do método "specificApply" para a PartialRandomRemoval
Sol PartialRandomRemoval::specificApply(Sol &S) {
	// Chamada para o método "specificApply" da classe base "RemovalHeuristic"
	
	std::cout << "Partial Random Removal: \n";
	
	int mi = RemovalHeuristic::initializeMethod(S);
	
	// Initial number of nodes in solution
	int initial_number_of_nodes = std::accumulate(S.RSize.begin(), S.RSize.end(), 0);
	
	int current_number_of_nodes = initial_number_of_nodes;
	
	// Vector with valid nodes to be removed (P U D)
	std::vector<int> nodes = S.inst.P;
	nodes.insert(nodes.end(), S.inst.D.begin(), S.inst.D.end());
	
	int amount_of_valid_nodes = nodes.size();
	
	while ((initial_number_of_nodes - current_number_of_nodes) < mi){
		
		// Choosing random node
		int random_index = rand()%amount_of_valid_nodes;
		
		int random_node = nodes.at(random_index);
		
		// Removing node case
		S.removeNodeCase(random_node);
		
		// Updating number of nodes in solution
		current_number_of_nodes = std::accumulate(S.RSize.begin(), S.RSize.end(), 0);
		
	}
	
	
	return S;
}

// Sobrescrita do método "specificApply" para a PartialRandomRemoval
Sol ConcentricRemoval::specificApply(Sol &S) {
	
	std::cout << "Concentric Removal: \n";
	
	int mi = RemovalHeuristic::initializeMethod(S);
	
	// Initial number of nodes in solution
	int initial_number_of_nodes = std::accumulate(S.RSize.begin(), S.RSize.end(), 0);
	
	int current_number_of_nodes = initial_number_of_nodes;
	
	// Choosing random node in P U D to be considered the center node
	// Vector with valid nodes to be removed (P U D)
	std::vector<int> nodes = S.inst.P;
	nodes.insert(nodes.end(), S.inst.D.begin(), S.inst.D.end());
	
	int amount_of_valid_nodes = nodes.size();
	
	// Node of reference
	int center_node = nodes.at(rand()%amount_of_valid_nodes);
	
	// Choosing subset of nodes in neighborhood of "center_node"
	
	// Constructing neighborhood
	std::vector<int> neighborhood_nodes = {};
	
	for (auto node: nodes){
		
		if (S.inst.dist.at(center_node).at(node) <= radius){
			
			neighborhood_nodes.push_back(node);
			
		}
		
	}
	
	int amount_of_neighborhood_nodes = neighborhood_nodes.size();
	
	// std::cout << center_node << std::endl;
	
	printInt(neighborhood_nodes);
	
	// While "mi" nodes haven't been removed or while there's still cases of nodes to be removed in neighborhood
	
	while (((initial_number_of_nodes - current_number_of_nodes) < mi) and (S.containsAny(neighborhood_nodes))){
		
		// // Choosing random node
		int random_index = rand()%amount_of_neighborhood_nodes;
		
		int random_node = neighborhood_nodes.at(random_index);
		
		// Removing node case
		S.removeNodeCase(random_node);
		
		// Updating number of nodes in solution
		current_number_of_nodes = std::accumulate(S.RSize.begin(), S.RSize.end(), 0);
		
	}
	
	// std::cout << "mi = " << mi << std::endl;
	
	// std::cout << "initial_number_of_nodes = " << initial_number_of_nodes << std::endl;
	
	// std::cout << "current_number_of_nodes = " << current_number_of_nodes << std::endl;
	
	
	return S;
}

//// InsertionHeuristic objects implementation

void InsertionHeuristic::initializeMethod() {
	
	std::cout << "Initializing Insertion" << std::endl;
	
	// Replicable data
	srand(135);
	
	// True random data
	// srand(time(NULL));
	
	
}

Sol BasicGreedyInsertion::specificApply(Sol &S) {
	
	// Initialization of insertion:
	
	InsertionHeuristic::initializeMethod();
	
	std::cout << "Basic Greedy Insertion" << std::endl;
	
	bool idle_segments {true};
	
	// Available nodes to be inserted: starts with all nodes in D
	std::vector<int> available_nodes = S.inst.D;
	
	// While there are idle segments in solution:
	while ((idle_segments) and (available_nodes.size() > 0)){
		
		
		// Selecting client to be inserted
		
		// This is done by iterating in met demands vector only once
		// For each value, a random noise between -5% and 5% (or other number) is added
		// giving randomness to the search
		
		// It is faster than sorting vector everytime!
		
		double lowest_met_demand = 1;
		
		double lowest_met_demand_index = available_nodes.at(0);
		
		double noise_value = 0.02;
		
		double random_noise = -noise_value + static_cast<double>(rand()) / RAND_MAX * (2*noise_value);
		
		double noise_met_demand {};
		
		for (auto &node_index: available_nodes){
			
			random_noise = -noise_value + static_cast<double>(rand()) / RAND_MAX * (2*noise_value);
			
			// Applying random noise to evaluate met demand percentage
			noise_met_demand = S.Z.at(node_index) + random_noise;
			
			if (noise_met_demand < lowest_met_demand){
				
				lowest_met_demand = noise_met_demand;
				
				lowest_met_demand_index = node_index;
				
			}
			
			
		}
		
		// Variable with lowest_met_demand_index (just for readability)
		int insertion_node_index = lowest_met_demand_index;
		
		// The solution will now be segmented, and the "segments_map" variable
		// will store, for each route (key), all idle segments, by positions in route
		
		// This will make the code easier to understand, though a little bit less efficient
		std::vector<std::vector<std::vector<int>>> segments_vector(S.inst.m, std::vector<std::vector<int>>());
		
		// Variable that stores idle demand values in each segment
		std::vector<std::vector<double>> segments_idle_demands_vector(S.inst.m, std::vector<double>());
		
		
		// Splitting "S" into segments
		
		idle_segments = false;
		
		for (int route_index {0}; route_index < S.inst.m; route_index++){
			
			// Node position in route
			int node_position = {0};
			
			// Variable for storing idle demand
			double idle_demand = 0;
			
			// Segment nodes vector
			std::vector<int> segment {};
			
			for (auto &node: S.R.at(route_index)){
				
				if (((S.Z.at(node) == 9999) & (node_position > 0))){
					
					//std::cout << "\n";
					//std::cout << "Idle demand of segment: " << idle_demand << std::endl;
					
					if (idle_demand > 0){
						
						segments_vector.at(route_index).push_back(segment);
						segments_idle_demands_vector.at(route_index).push_back(idle_demand);
						
						idle_segments = true;
						
					}
					
					// Restarting idle demand counter
					idle_demand = 0;
					
					// Emptying segment
					segment = {};
					
				}
				
				// std::cout << node << " ";
				
				// Incrementing idle demand with position in "z"
				idle_demand += S.z.at(route_index).at(node_position);
				
				// Adding node to segment
				segment.push_back(node_position);
				
				// Incrementing node_position in route
				node_position++;
				
			}
			
			// Storing data for last segment - this is needed to do separetely 
			
			// std::cout << "\nIdle demand of segment: " << idle_demand << std::endl;
			
			if (idle_demand > 0){
						
				segments_vector.at(route_index).push_back(segment);
				segments_idle_demands_vector.at(route_index).push_back(idle_demand);
				
				idle_segments = true;
				
			}
			
			// std::cout << "\n\n\n";
			
		}
		
		// If no segment is idle, while loop breaks !
		
		if (!idle_segments){
			
			break;
			
		}
		
		
		
		// Checking for each idle segment the feasible insertions of client in node "lowest_met_demand_index"
		
		// Route max length - maybe there's a better way for doing that!
		double routes_max_length = S.inst.T*S.inst.w_b.at(0);
		
		// Delta for insertion
		double cost_delta = {};
		
		// Minimum insertion value
		double min_cost_delta = 100000;
		
		// Corresponding idle demand of segment of minimum insertion costs
		double min_cost_idle_demand = {};
		
		// Pair with positions for insertion - First value is route, second is position
		std::pair<int, int> min_cost_positions;
		
		// Boolean variable that controls if inserting the node in any position is feasible
		bool any_feasible_position = false;
		
		for (int route_index {0}; route_index < S.inst.m; route_index++){
			
			for (int segment_index {0}; segment_index < segments_vector.at(route_index).size(); segment_index++){
				
				// printInt(segments_vector.at(route_index).at(segment_index));
				
				// Checking all possible insertion positions at segment
				
				// The insertion positions will always start counting from the pickup node index
				// So, if segment is [1,2], possible insertions will be in 2 and 3 positions!
				
				for (auto &position: segments_vector.at(route_index).at(segment_index)){
					
					int insertion_position = position + 1;
					
					// std::cout << route_index << " " << insertion_position << std::endl;
					
					// Boolean to control feasibility of insertion
					bool feasible = false;
					
					// Time delta for insertion
					double time_delta  = deltaInsertion("time", S, insertion_node_index, route_index, insertion_position);
					
					// Checking feasibility of position regarding route length
					if ((S.W.at(route_index) + time_delta) < routes_max_length){
						
						feasible = true;
						
						std::cout << "Posicao factivel encontrada para cliente " << insertion_node_index << std::endl;
						
						any_feasible_position = true;
						
					}
					
					// If feasible, delta in position is checked
					if (feasible){
						
						double cost_delta = deltaInsertion("cost", S, insertion_node_index, route_index, insertion_position);
						
						// Checking if this is the position with lowest cost
						if (cost_delta < min_cost_delta){
							
							// If delta is the lowest, positions and delta are updated
							
							
							min_cost_delta = cost_delta;
							min_cost_positions.first = route_index;
							min_cost_positions.second = insertion_position;
							
							min_cost_idle_demand = segments_idle_demands_vector.at(route_index).at(segment_index);
							
							// std::cout << "\n" << cost_delta << std::endl;
							
							// std::cout << min_cost_positions.first << std::endl;
							
							// std::cout << min_cost_positions.second << "\n\n";
							
							
						}
						
					}
					
				}
				
			}
			
			// std::cout << "\n";
		}
		
		// for (auto vec: segments_idle_demands_vector){
			
			// printDouble(vec);
			
			// std::cout << "\n";
			
		// }
		
		
		
		// Calculating demand assigned to node
		
		// It's the minimum value between segment idle demand and client's current unmet demand (in absolute terms)
		double demand = std::min(min_cost_idle_demand, ( std::abs(S.inst.d.at(insertion_node_index)) - S.G.at(insertion_node_index)));
		
		
		
		// std::cout <<  std::abs(S.inst.d.at(insertion_node_index)) - S.G.at(insertion_node_index) << std::endl;
		
		std::cout << insertion_node_index << " " << min_cost_positions.first << " " << min_cost_positions.second << " " << demand << std::endl;
		
		if (any_feasible_position){
			
			// Inserting client in positions with lowest delta
			S.insertNodeAt(insertion_node_index, min_cost_positions.first, min_cost_positions.second, demand);
			
			
		// If no feasible positions in segment were found, node is not available anymore to be selected
		} else {
			
			available_nodes.erase(std::remove_if(available_nodes.begin(), available_nodes.end(), [&insertion_node_index](int value) -> bool { return value == insertion_node_index; }), available_nodes.end());
			
		}
		
	}
	
	
	return S;
	
}
