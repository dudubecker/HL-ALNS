#include "Heuristic.hpp"
#include <ctime>
#include <random>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <utility>
#include <cmath>
#include <utils.h>

//// Initializations

//// Heuristic object implementation

Sol Heuristic::apply(Sol &S){
	
    // std::cout << "Comportamento base de Heuristic\n";
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

double Heuristic::deltaReplacement(std::string delta_type, Sol &S, int &node_index, int &route_index, int &position_index){
	
	double delta {};
	
	// Treating case when removal position is the last in the route - arc difference calculation is not the same!
	
	bool last_node = false;
	
	if (position_index == (S.RSize.at(route_index) - 1)){
		
		last_node = true;
		
	}
	
	if (delta_type == "time"){
		
		// If replacement position is the last one
		if (last_node){
			
			int last_node_index = S.R.at(route_index).at(position_index);
			
			int last_but_one_node_index = S.R.at(route_index).at(position_index - 1);
			
			double old_arc_time = S.inst.t.at(last_but_one_node_index).at(last_node_index).at(route_index);
			
			double new_arc_time = S.inst.t.at(last_but_one_node_index).at(node_index).at(route_index);
			
			delta = new_arc_time - old_arc_time;
			
		// If replacement position is in the middle of the route
		} else {
			
			int node_replaced = S.R.at(route_index).at(position_index);
			
			int node_before = S.R.at(route_index).at(position_index - 1);
			
			int node_after = S.R.at(route_index).at(position_index + 1);
			
			double first_old_arc_time = S.inst.t.at(node_before).at(node_replaced).at(route_index);
			
			double second_old_arc_time = S.inst.t.at(node_replaced).at(node_after).at(route_index);
			
			double first_new_arc_time = S.inst.t.at(node_before).at(node_index).at(route_index);
			
			double second_new_arc_time = S.inst.t.at(node_index).at(node_after).at(route_index);
			
			delta =  (first_new_arc_time + second_new_arc_time) - (first_old_arc_time + second_old_arc_time);
			
		}
		
		
	} else if (delta_type == "cost"){
		
		
		// If replacement position is the last one
		if (last_node){
			
			int last_node_index = S.R.at(route_index).at(position_index);
			
			int last_but_one_node_index = S.R.at(route_index).at(position_index - 1);
			
			double old_arc_cost = S.inst.c.at(last_but_one_node_index).at(last_node_index).at(route_index);
			
			double new_arc_cost = S.inst.c.at(last_but_one_node_index).at(node_index).at(route_index);
			
			delta =  new_arc_cost - old_arc_cost;
			
			
		// If replacement position is in the middle of the route
		} else {
			
			int node_replaced = S.R.at(route_index).at(position_index);
			
			int node_before = S.R.at(route_index).at(position_index - 1);
			
			int node_after = S.R.at(route_index).at(position_index + 1);
			
			double first_old_arc_cost = S.inst.t.at(node_before).at(node_replaced).at(route_index);
			
			double second_old_arc_cost = S.inst.t.at(node_replaced).at(node_after).at(route_index);
			
			double first_new_arc_cost = S.inst.t.at(node_before).at(node_index).at(route_index);
			
			double second_new_arc_cost = S.inst.t.at(node_index).at(node_after).at(route_index);
			
			delta = (first_new_arc_cost + second_new_arc_cost) - (first_old_arc_cost + second_old_arc_cost);
			
		}
		
		
	} else {
		
		std::cout << "Not a valid type for 'deltaReplacement' function" << std::endl;
		
	}
	
	// Obs: delta can be either negative or positive!
	
	return delta;
	
}

std::pair<double, double> Heuristic::deltaEpsilon(Sol &S, int &source_node_index, int &receiver_node_index, int &route_index, int &position_index){

	std::pair<double, double> return_pair = {};
	
	
	
	// Available demand to be transferred
	double available_demand = std::abs( S.z.at(route_index).at(position_index));
	
	// Pace on which load is transferred from one client to other
	double pace = 1;
	
	// Met demand for source node
	double met_demand_source_node = S.G.at(source_node_index);
	
	// Met demand for receiver node
	double met_demand_receiver_node = S.G.at(receiver_node_index);
	
	// Initial source epsilon
	double epsilon_source_node = std::abs(     (met_demand_source_node/S.totalZ)  -  (std::abs(S.inst.d.at(source_node_index))/S.totalD)     );
	
	// Initial receiver epsilon
	double epsilon_receiver_node = std::abs(     (met_demand_receiver_node/S.totalZ)  -  (std::abs(S.inst.d.at(receiver_node_index))/S.totalD)     );
	
	// Initial epsilon sum
	double initial_epsilon_sum = epsilon_source_node + epsilon_receiver_node;
	initial_epsilon_sum = roundToDecimalPlaces(initial_epsilon_sum, 5);
	
	// Current epsilon sum
	double epsilon_sum = initial_epsilon_sum;
	
	// Minimum epsilon sum
	double min_epsilon_sum = initial_epsilon_sum;
	
	// Transferred demand
	double transferred_demand = 0;
	
	// Transferred demand of minimum epsilon
	double min_epsilon_transferred_demand = 0;
	
	// While there is demand to be transferred
	while (available_demand >= 0){
		
		if (epsilon_sum < min_epsilon_sum){
			
			// That means that both epsilons are getting better, so iterations should continue
			
			min_epsilon_sum = epsilon_sum;
			min_epsilon_transferred_demand = transferred_demand;
			
			// std::cout << "Both getting better" << std::endl;
			
			
		} else if ((epsilon_sum == min_epsilon_sum) and (transferred_demand > 0)){
			
			// That means one epsilon is rising and the other is falling - if after all iterations
			// the epsilons don't start getting worst, it can be a replacement if feasible and if it reduces costs!
			
			// std::cout << "One getting worst and the other getting better" << std::endl;
			
			// break;
			
			;
			
		} else if (epsilon_sum > min_epsilon_sum){
			
			// That means the epsilons are getting worst
			
			// std::cout << "Both getting worst!" << std::endl;
			
			break;
			
		}
		
		met_demand_source_node -= pace;
		
		met_demand_receiver_node += pace;
		
		epsilon_source_node = std::abs(     (met_demand_source_node/S.totalZ)  -  (std::abs(S.inst.d.at(source_node_index))/S.totalD)     );
		
		epsilon_receiver_node = std::abs(     (met_demand_receiver_node/S.totalZ)  -  (std::abs(S.inst.d.at(receiver_node_index))/S.totalD)     );
		
		epsilon_sum = epsilon_source_node + epsilon_receiver_node;
		epsilon_sum = roundToDecimalPlaces(epsilon_sum, 5);
		
		transferred_demand += pace;
		available_demand -= pace;
		
	}
	
	double delta_epsilon = min_epsilon_sum - initial_epsilon_sum;
	
	
	// Case when since first pace the epsilons get worst: this is forbidden!
	
	if (transferred_demand == pace){
		
		delta_epsilon = 9999;
		
	}
	
	// std::cout << "min_epsilon_sum : " << min_epsilon_sum << std::endl;
	
	// std::cout << "initial_epsilon_sum : " << initial_epsilon_sum << std::endl;
	
	// std::cout << "Delta epsilon: " << delta_epsilon << std::endl;
	// std::cout << "transferred demand from " << source_node_index << " to " << receiver_node_index << " for minimizing epsilon sum: " << min_epsilon_transferred_demand << std::endl;
	
	// std::cout << "transfering " <<  transferred_demand << " demand from " << source_node_index << " to " << receiver_node_index << " : ";
	
	// First element of pair - delta epsilon of insertion/replacement
	
	return_pair.first = delta_epsilon;
	
	// Second element of pair - transferred demand for minimizing delta epsilon
	
	// Case when it's possible to enhance both epsilons until a certain point
	if (delta_epsilon < 0){
		
		return_pair.second = min_epsilon_transferred_demand;
	
	// Case when it's possible to replace, if transfering all demand doesn't impact epsilon sum
	} else if (delta_epsilon == 0){
		
		return_pair.second = (transferred_demand - 1);
	
	// Case when, since first transfering, epsilons get worst
	} else if (delta_epsilon == 9999){
		
		return_pair.second = 0;
		
	}
	
	
	return return_pair;
	
}

//// RemovalHeuristic objects implementation

// Sobrescrita do método "specificApply" para a RemovalHeuristic
int RemovalHeuristic::initializeMethod(Sol &S) {
	
	// std::cout << "Removal heuristics base code\n";
	
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
	
	std::cout << "\n\nPartial Random Removal\n\n";
	
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
	
	std::cout << "\n\nConcentric Removal: \n";
	
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
	
	// printInt(neighborhood_nodes);
	
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
	
	// std::cout << "Initializing Insertion" << std::endl;
	
	// Replicable data
	srand(136);
	
	// True random data
	// srand(time(NULL));
	
	
}

Sol BasicGreedyInsertion::specificApply(Sol &S) {
	
	// Initialization of insertion:
	
	InsertionHeuristic::initializeMethod();
	
	std::cout << "\n\nBasic Greedy Insertion\n\n" << std::endl;
	

	

	// First part of insertion - Getting rid of idle segments
{
	bool idle_segments {true};
	
	std::vector<std::vector<std::vector<int>>> segments_vector(S.inst.m, std::vector<std::vector<int>>());
	
	// Available nodes to be inserted: starts with all nodes in D
	std::vector<int> available_nodes = S.inst.D;
	
	// Taking out fully served clients
	for (auto node: available_nodes){
		
		if (S.Z.at(node) == 1){
			
			available_nodes.erase(std::remove_if(available_nodes.begin(), available_nodes.end(), [&node](int value) -> bool { return value == node; }), available_nodes.end());
			
		}
		
	}
	
	// While there are idle segments in solution:
	while ((idle_segments) and (available_nodes.size() > 0)){
		
		// The solution will now be segmented, and the "segments_vector" variable
		// will store, for each route (key), all idle segments, by positions in route
		
		// This will make the code easier to understand, though a little bit less efficient
		
		// Clearing segments vector for iteration
		
		for (auto& segment : segments_vector){
			for (auto& inner_segment : segment){
				inner_segment.clear();
			}
			segment.clear();
		}
		
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
		
		
		// Checking for each idle segment the feasible insertions of each client
		
		// Route max length - maybe there's a better way for doing that!
		double routes_max_length = S.inst.T*S.inst.w_b.at(0);
		
		// Delta for insertion
		double cost_delta = {};
		
		// Minimum insertion value
		double min_cost_delta = 100000;
		
		// Corresponding idle demand of segment of minimum insertion costs
		double min_cost_idle_demand = {};
		
		// Min costs node
		int min_cost_node = {};
		
		// Pair with positions for insertion - First value is route, second is position
		std::pair<int, int> min_cost_positions;
		
		// Boolean variable that controls if inserting the node in any position is feasible
		bool any_feasible_position = false;
		
		// Iterating -> delivery nodes -> routes -> segments
		
		for (auto &insertion_node: available_nodes){
			
			// Boolean variable that controls if, for a specific node, there's been found a feasible insertion position
			bool feasible_position_node = false;
			
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
						double time_delta  = deltaInsertion("time", S, insertion_node, route_index, insertion_position);
						
						// Checking feasibility of position regarding route length
						if ((S.W.at(route_index) + time_delta) < routes_max_length){
							
							feasible = true;
							
							// std::cout << "Posicao factivel encontrada para cliente " << insertion_node_index << std::endl;
							
							any_feasible_position = true;
							
							feasible_position_node = true;
							
						}
						
						// If feasible, delta in position is checked
						if (feasible){
							
							// Here, other types of scores can be calculated!
							
							cost_delta = deltaInsertion("cost", S, insertion_node, route_index, insertion_position);
							
							// Checking if this is the position with lowest cost
							if (cost_delta < min_cost_delta){
								
								// If delta is the lowest, positions and delta are updated
								
								
								min_cost_delta = cost_delta;
								
								min_cost_node = insertion_node;
								
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
			
			if (!feasible_position_node){
				
				// std::cout << insertion_node << std::endl;
				
				available_nodes.erase(std::remove_if(available_nodes.begin(), available_nodes.end(), [&insertion_node](int value) -> bool { return value == insertion_node; }), available_nodes.end());
				
				// printInt(available_nodes);
				
			}
			
		}
		
		// Calculating demand assigned to node
		
		// It's the minimum value between segment idle demand and client's current unmet demand (in absolute terms)
		double demand = std::min(min_cost_idle_demand, ( std::abs(S.inst.d.at(min_cost_node)) - S.G.at(min_cost_node)));
		
		if ((any_feasible_position) and (demand > 0)){
			
			// Inserting client in positions with lowest delta
			S.insertNodeAt(min_cost_node, min_cost_positions.first, min_cost_positions.second, demand);
			
		// If no feasible positions in segment were found, node is not available anymore to be selected
		}
		
		
		// Node is no longer available if its demand is fully covered
		if (S.Z.at(min_cost_node) == 1){
			
			available_nodes.erase(std::remove_if(available_nodes.begin(), available_nodes.end(), [&min_cost_node](int value) -> bool { return value == min_cost_node; }), available_nodes.end());
			
		}
		
	}
	
	// Last part of initialization - If some segment could not have any feasible insertion, it is then removed
	// This is done because in the next part of the insertion, no segment can be idle!
	
	for (auto route_index {0}; route_index < segments_vector.size(); route_index++){
		
		for (auto segment_index {0}; segment_index < segments_vector.at(route_index).size(); segment_index++){
			
			for (auto &removal_index: segments_vector.at(route_index).at(segment_index)){
				
				// Initial route size
				int initial_route_size = S.RSize.at(route_index);
				
				// Excluding idle segment
				S.removeNodeAt(route_index, removal_index);
				
				// Route size after removal
				int final_route_size = S.RSize.at(route_index);
				
				// Segment size, which is equal to variation in route size
				int segment_size = initial_route_size - final_route_size;
				
				// Updating position in segments_vector object, based on delta in route size
				for (auto index_position {0}; index_position < segments_vector.at(route_index).at(segment_index).size(); index_position++){
					
					segments_vector.at(route_index).at(segment_index).at(index_position) -= segment_size;
					
				}
				
				
			}
			
			
			
		}
		
	}
}
	
	// Second part of insertion - Giving feasibility to solution regarding epsilon value
	
	
	// Available nodes to be inserted: starts with all nodes in D
	std::vector<int> available_nodes = {};
	
	double global_epsilon = 0.08;
	
	// Route max length - maybe there's a better way for doing that!
	double routes_max_length = S.inst.T*S.inst.w_b.at(0);
	
	// Max epsilon
	double max_epsilon = *std::max_element(S.epsilon.begin(), S.epsilon.end());
	
	while (max_epsilon > global_epsilon){
		
		S.printSol();
		int cont = 0;
		for (auto &epsilon: S.epsilon){
			
			if (epsilon > -9999){
				
				std::cout << cont << ": " << epsilon*100 << "%\n";
				
			}
			cont += 1;
		}
		
		printDouble(S.Z);
		
		
		// Available nodes to be inserted: starts with all nodes in D
		
		////// It would be more efficient if this was an attribute!
		available_nodes = S.inst.D;
	
		// Taking out fully served clients
		for (auto node: available_nodes){
			
			if (S.Z.at(node) == 1){
				
				available_nodes.erase(std::remove_if(available_nodes.begin(), available_nodes.end(), [&node](int value) -> bool { return value == node; }), available_nodes.end());
				
			}
			
		}
		
		// Minimum score found so far: great scores are negative scores!
		double min_score = 9999;
		
		// Corresponding node of minimum score
		double min_score_node = {};
		
		// Corresponding route of minimum score
		double min_score_route = {};
		
		// Corresponding position of minimum score
		double min_score_position = {};
		
		// Corresponding transferred demand of minimum score
		double min_score_transferred_demand = {};
		
		// Boolean variable, that controls if best score corresponds to a insertion before
		bool insertion_before = false;
		
		// Boolean variable, that controls if best score corresponds to a insertion after
		bool insertion_after = false;
		
		// Boolean variable, that controls if best score corresponds to a replacement
		bool replacement = false;
		
		// Iterating, for all available nodes for insertion, for all routes and positions
		
		for (auto &receiver_node_index : available_nodes){
			
			for (int route_index {0}; route_index < S.inst.m; route_index++){
				
				// Nodes can be inserted from position 2 of S, as position 0 is depot and position 1 is first pickup node
				for (int position_index {2}; position_index < S.RSize.at(route_index); position_index++){
					
					int source_node_index = S.R.at(route_index).at(position_index);
					
					// std::cout << "Route: " << route_index << std::endl;
					// std::cout << "Position: " << position_index << std::endl;
					// std::cout << "Receiver node: " << receiver_node_index << std::endl;
					// std::cout << "Source node: " << source_node_index << std::endl;
					
					// If source_node_index is not pickup node
					if ((S.Z.at(source_node_index) != 9999) and (source_node_index != receiver_node_index)){
						
						// Checking feasibilities and, if feasible, scores
						int position_before = position_index;
						int position_after = position_index + 1;
						
						double delta_epsilon, transferred_demand;
						
						// Feasibility for inserting node before source node
						if (S.W.at(route_index) + deltaInsertion("time", S, receiver_node_index, route_index, position_before) < routes_max_length){
							
							// std::cout << "Feasible insertion before found!" << std::endl;
							
							// Calculating scores
							
							std::tie(delta_epsilon, transferred_demand) = deltaEpsilon(S, source_node_index, receiver_node_index, route_index, position_index);
							
							double delta_costs_insertion_before = deltaInsertion("cost", S, receiver_node_index, route_index, position_before);
							double score_insertion_before = delta_epsilon*1000000 + delta_costs_insertion_before;
							
							if (score_insertion_before < min_score){
								
								// Updating values with corresponding data from iteration
								min_score = score_insertion_before;
								// Corresponding node of minimum score
								min_score_node = receiver_node_index;
								// Corresponding route of minimum score
								min_score_route = route_index;
								// Corresponding position of minimum score
								min_score_position = position_before;
								// Corresponding transferred demand of minimum score
								min_score_transferred_demand = transferred_demand;
								// Boolean variables
								replacement = false;
								insertion_after = false;
								insertion_before = true;
								
							}
							
							// std::cout << "Score: " << score_insertion_before << "\n\n";
						}
						
						// Feasibility for inserting node after source node
						if (S.W.at(route_index) + deltaInsertion("time", S, receiver_node_index, route_index, position_after) < routes_max_length){
							
							// std::cout << "Feasible insertion after found!" << std::endl;
							
							// Calculating scores
							
							std::tie(delta_epsilon, transferred_demand) = deltaEpsilon(S, source_node_index, receiver_node_index, route_index, position_index);
							
							double delta_costs_insertion_after = deltaInsertion("cost", S, receiver_node_index, route_index, position_after);
							
							double score_insertion_after = delta_epsilon*1000000 + delta_costs_insertion_after;
							
							if (score_insertion_after < min_score){
								
								// Updating values with corresponding data from iteration
								min_score = score_insertion_after;
								// Corresponding node of minimum score
								min_score_node = receiver_node_index;
								// Corresponding route of minimum score
								min_score_route = route_index;
								// Corresponding position of minimum score
								// min_score_position = position_after;
								
								// Corresponding transferred demand of minimum score
								min_score_transferred_demand = transferred_demand;
								// Boolean variables
								replacement = false;
								insertion_after = true;
								insertion_before = false;
								
							}
							
							// std::cout << "Score: " << score_insertion_after << "\n\n";
							
						}
						// Feasibility for replacing source node for receiving node
						if (S.W.at(route_index) + deltaReplacement("time", S, receiver_node_index, route_index, position_index) < routes_max_length){
							
							// std::cout << S.W.at(route_index) + deltaReplacement("time", S, receiver_node_index, route_index, position_index) << std::endl;
							// std::cout << "Feasible replacement found!" << std::endl;
							
							
							// Calculating scores
							
							std::tie(delta_epsilon, transferred_demand) = deltaEpsilon(S, source_node_index, receiver_node_index, route_index, position_index);
							
							double delta_costs_replacement = deltaReplacement("cost", S, receiver_node_index, route_index, position_index);
							double score_replacement = delta_epsilon*1000000 + delta_costs_replacement;
							
							// std::cout << "Score: " << score_replacement << "\n\n";
							
							// Replacement has an additional constraint: I can only replace if transferring all demand doesn't impact epsilon sums!
							if ((score_replacement < min_score) and (transferred_demand == std::abs(S.z.at(route_index).at(position_index)))){
								
								// std::cout << "\n\n\nREPLACEMENT POSITION HAS BECOME THE BEST ONE\n\n\n" << std::endl;
								
								// Updating values with corresponding data from iteration
								min_score = score_replacement;
								// Corresponding node of minimum score
								min_score_node = receiver_node_index;
								// Corresponding route of minimum score
								min_score_route = route_index;
								// Corresponding position of minimum score
								min_score_position = position_index;
								// Corresponding transferred demand of minimum score
								min_score_transferred_demand = transferred_demand;
								// Boolean variables
								replacement = true;
								insertion_after = false;
								insertion_before = false;
								
							}
						}
					}
				}
				
				// std::cout << "\n\n\n";
			}
		}
		
		
		
		//
		std::cout << "Minimum found score: " << min_score << std::endl;
		
		std::cout << "Corresponding node: " << min_score_node << std::endl;
		// Corresponding route of minimum score
		std::cout << "Corresponding route: " << min_score_route << std::endl;
		// Corresponding position of minimum score
		std::cout << "Corresponding position at route: " << min_score_position  << std::endl;
		// Corresponding source node
		// std::cout << "Corresponding source node: " << S.R.at(min_score_route).at(min_score_position)  << std::endl;
		// Corresponding transferred demand of minimum score
		std::cout << "Corresponding demand to be transferred: " << min_score_transferred_demand << std::endl;
		// Boolean variable, that controls if best score corresponds to a replacement
		std::cout << "Is it a replacement? -> " << replacement << std::endl;
		std::cout << "Is it a insertion_before? -> " << insertion_before << std::endl;
		std::cout << "Is it a insertion_after? -> " << insertion_after << std::endl;
		
		// Making changes in Solution object
		
		if (insertion_before){
			
			int insertion_before_node = min_score_node;
			int insertion_before_route = min_score_route;
			int insertion_before_position = min_score_position;
			double insertion_before_demand = min_score_transferred_demand;
			
			S.splitInsertion("before", insertion_before_node, insertion_before_route, insertion_before_position, insertion_before_demand);
			
		} else if (insertion_after){
			
			int insertion_after_node = min_score_node;
			int insertion_after_route = min_score_route;
			int insertion_after_position = min_score_position;
			double insertion_after_demand = min_score_transferred_demand;
			
			S.splitInsertion("after", insertion_after_node, insertion_after_route, insertion_after_position, insertion_after_demand);
			
		} else if (replacement){
		// if (replacement){
			
			int replacement_node = min_score_node;
			int replacement_route = min_score_route;
			int replacement_position = min_score_position;
			
			S.replaceNodeAt(replacement_node, replacement_route, replacement_position);
			
		}
		
		
		max_epsilon = *std::max_element(S.epsilon.begin(), S.epsilon.end());
		
		std::string a;
		
		std::cin >> a;
		
		// break;
	}
		
		
		
		
		

		
	
	
	
	
	
	
	
	
	
	return S;
	
}


