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

// Delta methods - insertion, removal (of individual nodes, arcs and segments), replacement, regarding time, cost and epsilon

//// RemovalHeuristic objects implementation

// Sobrescrita do método "specificApply" para a RemovalHeuristic
int RemovalHeuristic::initializeMethod(Sol &S) {
	
	// std::cout << "Removal heuristics base code\n";
	
	// Replicable data
	// srand(119);
	
	// True random data
	// srand(time(NULL));
	
	// Number of nodes in solution
	int number_of_nodes = std::accumulate(S.RSize.begin(), S.RSize.end(), 0);
	
	// Valor mínimo de mi (10% da quantidade de pedidos)
	int low_mi = round(number_of_nodes*0.2);
	
	// Valor máximo de mi (40% da quantidade de pedidos)
	int high_mi = round(number_of_nodes*0.4);
	
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
	
	while ((initial_number_of_nodes - current_number_of_nodes) < mi){
		
		S.removeRandomNode();
		
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
	
	// Limiting number of iterations - very rare case when while loop does not stop!
	int it = 0;
	
	
	while (((initial_number_of_nodes - current_number_of_nodes) < mi) and (S.containsAny(neighborhood_nodes))){
		
		// // Choosing random node
		int random_index = rand()%amount_of_neighborhood_nodes;
		
		int random_node = neighborhood_nodes.at(random_index);
		
		// Removing node case
		S.removeNodeCase(random_node);
		
		// Updating number of nodes in solution
		current_number_of_nodes = std::accumulate(S.RSize.begin(), S.RSize.end(), 0);
		
		////
		it += 1;
		if (it > 3000){
			
			break;
			
		}
		////
		
	}
	
	// std::cout << "mi = " << mi << std::endl;
	
	// std::cout << "initial_number_of_nodes = " << initial_number_of_nodes << std::endl;
	
	// std::cout << "current_number_of_nodes = " << current_number_of_nodes << std::endl;
	
	
	return S;
}

// Sobrescrita do método "specificApply" para a PartialRandomRemoval

Sol WorstRemoval::specificApply(Sol &S) {
	
	std::cout << "\n\nWorst Removal: \n";
	
	int mi = RemovalHeuristic::initializeMethod(S);
	
	// Counter for removed nodes in solution
	int amount_of_removed_nodes = 0;
	
	
	
	while (amount_of_removed_nodes < mi){
		
		
		
		double random_number = {};
		random_number = ((double) rand() / (RAND_MAX));
		
		// Chance of a segment being selected instead of a node
		
		double segment_removal_chance = 1;
		
		// Removing node
		if (random_number > segment_removal_chance){
			
			// Evaluating delta in costs and epsilon for removing delivery node
			
			// std::cout << "\n\n\nREMOVING NODE\n\n\n";
			
			double min_score = 9999;
			
			int min_score_node = {};
			
			int min_score_route = {};
			
			int min_score_position = {};
			
			for (auto route_index {0}; route_index < S.inst.m; route_index++){
				
				for (auto node_position {1}; node_position < S.RSize.at(route_index); node_position++){
					
					
					int node = S.R.at(route_index).at(node_position);
					
					// Node needs to be delivery node
					if (S.Z.at(node) != 9999){
						
						// Costs:
						double delta_costs = deltaRemoval("cost", S, node, route_index, node_position);
						
						// Normalizing delta costs
						// std::cout << delta_costs << "," << std::endl;
						
						double norm_delta_costs = (delta_costs - (-700))/(320 - (700));
						
						double norm_delta_unmet_demand = std::abs(S.z.at(route_index).at(node_position))/std::abs(S.inst.d.at(node));
						
						
						// double delta_epsilon = deltaEpsilonRemoval(S, node, route_index, node_position);
						
						// std::cout << "Node: " << node << ", Route: " << route_index << ", position: " << node_position << std::endl;
						// std::cout << "Delta in costs: " << delta_costs << std::endl;
						// std::cout << "Delta in epsilon: " << delta_epsilon << "\n\n";
						
						// double score = delta_costs;
						double score = (norm_delta_costs*gamma2 - norm_delta_unmet_demand*gamma3);
						
						if (score < min_score){
							
							min_score = score;
							
							min_score_node = node;
							
							min_score_route = route_index;
							
							min_score_position = node_position;
							
						}
					
					}
					
					
				}
				
			}
			
			
			
			// std::cout << "\n\nRemoved node: " << min_score_node << ", Route: " << min_score_route << ", position: " << min_score_position << ", and delta: " << min_score << "\n\n";
			
			// Old length of route
			int route_old_length = S.RSize.at(min_score_route);
			
			// Removing node from solution
			S.removeNodeAt(min_score_route, min_score_position);
			
			// New length of route
			
			int route_new_length = S.RSize.at(min_score_route);
			
			// Number of removed nodes
			int removed_nodes = route_old_length - route_new_length;
			
			// Incrementing counter
			amount_of_removed_nodes += removed_nodes;
			
		// Removing segment - Handling error!
		} else {
			
				
				
			// std::cout << "\n\n\nREMOVING SEGMENT\n\n\n";
			
			double min_score = 9999;
			
			int min_score_segment_first_node = {};
			
			int min_score_route = {};
			
			int min_score_position = {};
			
			// Segment size for being removed (2 or 3)
			int segment_removal_size = 2 + rand()%2;
			
			// Evaluating delta in costs and epsilon for removing segment
			
			// Segment vector
			std::vector<int> segment {};
			
			// Segment size
			int segment_size = 0;
			
			// Naming all segments in solution, an checking their deltas
			for (auto route_index {0}; route_index < S.inst.m; route_index++){
				
				segment = {};
				segment_size = 0;
				
				for (auto node_position {1}; node_position < S.RSize.at(route_index); node_position++){
					
					int node = S.R.at(route_index).at(node_position);
					
					// If not last segment
					if ((S.Z.at(node) == 9999) and ((node_position > 1))){
						
						// std::cout << "First node (pickup): " << segment.at(0) << std::endl;
						// std::cout << "Route: " << route_index << std::endl;
						// std::cout << "Position: " << node_position - segment_size << std::endl;
						
						// Checking score for segment, if it meets the size
						if (segment_size == segment_removal_size){
							
							int pickup_node_index = segment.at(0);
							
							// Different for last case!!
							int removal_index = node_position - segment_size;
							
							double delta_costs_segment = deltaRemovalSegment("cost", S, pickup_node_index, route_index, removal_index);
							
							double score = delta_costs_segment;
							
							// std::cout << score << "\n\n";
							
							if (score < min_score){
								
								min_score = score;
								
								min_score_segment_first_node = pickup_node_index;
								
								min_score_route = route_index;
								
								min_score_position = removal_index;
								
							}
						}
						
						segment = {};
						segment_size = 0;
						
					}
					
					// Last segment, in this code, need to be treated separetely
					if (node_position == S.RSize.at(route_index) - 1){
						
						segment.push_back(S.R.at(route_index).at(node_position));
						segment_size += 1;
						
						// std::cout << "First node (pickup): " << segment.at(0) << std::endl;
						// std::cout << "Route: " << route_index << std::endl;
						// std::cout << "Position: " << node_position - segment_size + 1 << "\n\n";
						
						// Checking score for segment, if it meets the size
						if (segment_size == segment_removal_size){
							
							int pickup_node_index = segment.at(0);
							
							// Different for last case!!
							int removal_index = node_position - segment_size + 1;
							
							double delta_costs_segment = deltaRemovalSegment("cost", S, pickup_node_index, route_index, removal_index);
							
							double score = delta_costs_segment;
							
							// std::cout << score << "\n\n";
							
							if (score < min_score){
								
								min_score = score;
								
								min_score_segment_first_node = pickup_node_index;
								
								min_score_route = route_index;
								
								min_score_position = removal_index;
								
							}
						}
						
						
						
					}
					
					segment.push_back(node);
					segment_size += 1;
					
				}
				
				
			}
			
			
			
			// std::cout << "\n\nRemoved node: " << min_score_segment_first_node << ", Route: " << min_score_route << ", position: " << min_score_position << ", and delta: " << min_score << "\n\n";
			
			// Old length of route
			int route_old_length = S.RSize.at(min_score_route);
			
			// Removing minimum score segment - Only if segment of right length was found!
			if (min_score != 9999){
				S.removeNodeAt(min_score_route, min_score_position);
			}
			// New length of route
			int route_new_length = S.RSize.at(min_score_route);
			
			// Number of removed nodes
			int removed_nodes = route_old_length - route_new_length;
			
			// Incrementing counter
			amount_of_removed_nodes += removed_nodes;
			
			
		}
		
		
	}
	
	return S;
}

//// InsertionHeuristic objects implementation

void InsertionHeuristic::initializeMethod() {
	
	// std::cout << "Initializing Insertion" << std::endl;
	
	// Replicable data
	// srand(130);
	
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
	// std::vector<int> available_nodes = S.unmet_demand_clients;
	
	// Taking out fully served clients
	for (auto node: available_nodes){
		
	 	if (S.Z.at(node) == 1){
			
	 		available_nodes.erase(std::remove_if(available_nodes.begin(), available_nodes.end(), [&node](int value) -> bool { return value == node; }), available_nodes.end());
			
		}
		
	}
	
	
	// While there are idle segments in solution:
	while ((idle_segments) and (available_nodes.size() > 0)){
		
		// std::cout << available_nodes.size() << std::endl;
		
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
		
		// Minimum insertion value
		double min_score = 100000;
		
		// Corresponding idle demand of segment of minimum insertion costs
		double min_score_idle_demand = {};
		
		// Min costs node
		int min_score_node = {};
		
		// Pair with positions for insertion - First value is route, second is position
		std::pair<int, int> min_score_positions;
		
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
							
							// std::cout << "Posicao factivel encontrada para cliente " << insertion_node << std::endl;
							
							any_feasible_position = true;
							
							feasible_position_node = true;
							
						}
						
						// If feasible, delta in position is checked
						if (feasible){
							
							// Here, other types of scores can be calculated!
							
							double delta_costs = deltaInsertion("cost", S, insertion_node, route_index, insertion_position);
							
							// Normalizing costs
							double norm_delta_costs = (delta_costs - 2100)/(2100 - (-300));
							
							// Demand
							double available_demand = std::min(segments_idle_demands_vector.at(route_index).at(segment_index), ( std::abs(S.inst.d.at(insertion_node)) - S.G.at(insertion_node)));
							
							// Calculating epsilon delta
							double current_epsilon = std::abs((S.G.at(insertion_node))/(S.totalZ) - (std::abs(S.inst.d.at(insertion_node)))/(S.totalD));
							
							double epsilon_after_insertion = std::abs(((S.G.at(insertion_node) + available_demand))/(S.totalZ) - (std::abs(S.inst.d.at(insertion_node)))/(S.totalD));
							
							double delta_epsilon = epsilon_after_insertion - current_epsilon;
							
							// Normalizing epsilon
							double norm_delta_epsilon = (delta_epsilon - (-0.05) )/(0.03 - (-0.05));
							
							// Calculating Z delta
							
							// double delta_unmet_demand = 
							
							double score = norm_delta_epsilon*gamma1 + norm_delta_costs*gamma2;
							
							// Checking if this is the position with lowest score
							if (score < min_score){
								
								// If delta is the lowest, positions and delta are updated
								
								min_score = score;
								
								min_score_node = insertion_node;
								
								min_score_positions.first = route_index;
								min_score_positions.second = insertion_position;
								
								min_score_idle_demand = available_demand;
								
								// std::cout << "\n" << cost_delta << std::endl;
								
								// std::cout << min_cost_positions.first << std::endl;
								
								// std::cout << min_cost_positions.second << "\n\n";
								
								
							}
							
						}
						
					}
					
				}
			}
			
			// If no feasible positions in segment were found, node is not available anymore to be selected
			if (!feasible_position_node){
			
			// std::cout << insertion_node << std::endl;
			
				available_nodes.erase(std::remove_if(available_nodes.begin(), available_nodes.end(), [&insertion_node](int value) -> bool { return value == insertion_node; }), available_nodes.end());
			
			// printInt(available_nodes);
			}
			
		}
		
		// Calculating demand assigned to node
		
		// It's the minimum value between segment idle demand and client's current unmet demand (in absolute terms)
		double demand = std::min(min_score_idle_demand, ( std::abs(S.inst.d.at(min_score_node)) - S.G.at(min_score_node)));
		
		// std::cout << min_cost_node  << " " << min_score_idle_demand << " " << std::abs(S.inst.d.at(min_cost_node)) - S.G.at(min_cost_node) << std::endl;
		
		if ((any_feasible_position) and (demand > 0)){
			
			// Inserting client in positions with lowest delta
			S.insertNodeAt(min_score_node, min_score_positions.first, min_score_positions.second, demand);
			
		}
		
		// Node is no longer available if its demand is fully covered
		if (S.Z.at(min_score_node) > 0.999){
			
			available_nodes.erase(std::remove_if(available_nodes.begin(), available_nodes.end(), [&min_score_node](int value) -> bool { return value == min_score_node; }), available_nodes.end());
			
		}
		
	}
	
	
}	
	
	// S.printSol();
	
	// Beginning second phase new code:
	double routes_max_length = S.inst.T*S.inst.w_b.at(0);
	
	// Tidying up solution
	S.tidyUp();
	
	// While there are feasible positions (this is only temporary)
	
	bool feasible_positions = true;
	
	while (feasible_positions){
		
		// Insert P-D probability - important parameter to tune for method!
		double pd_insertion_prob = 1;
		
		double random_value = 0;
		random_value = ((double) rand() / (RAND_MAX));
		
		// Here begins P-D insertion code:
		if (random_value < pd_insertion_prob){
			
			// Boolean variable that controls if there are feasible insertion positions
			feasible_positions = false;
			
			// Updating values with corresponding data from iteration
			double min_score = 9999;
			// Corresponding pickup node of minimum score
			int min_score_pickup_node = {};
			// Corresponding pickup node of minimum score
			int min_score_delivery_node = {};
			// Corresponding route of minimum score
			int min_score_route = {};
			// Corresponding position of minimum score
			int min_score_position = {};
			// Corresponding transferred demand of minimum score
			double min_score_transferred_demand = {};
			
			// Available nodes to be inserted: starts with all nodes in D
			std::vector<int> available_nodes = S.inst.D;
			// std::vector<int> available_nodes = S.unmet_demand_clients;
			
			// Taking out fully served clients
			for (auto node: available_nodes){
				
				if (S.Z.at(node) == 1){
					
					available_nodes.erase(std::remove_if(available_nodes.begin(), available_nodes.end(), [&node](int value) -> bool { return value == node; }), available_nodes.end());
					
				}
				
			}
			
			// unmet_demand_clients attribute still isn't 100% trustable
			
			//for (auto &delivery_node: S.unmet_demand_clients){
			for (auto &delivery_node: available_nodes){
				
				for (auto &pickup_node: S.inst.P){
					
					if (S.G.at(pickup_node) > 0){
						
						// With arc, all possible insertion positions are searched
						// An arc cannot be inserted at the middle of a segment!
						for (auto route_index {0}; route_index < S.inst.m; route_index++){
							
							// Starts in one because of depot node
							for (auto arc_insertion_position {1}; arc_insertion_position < S.RSize.at(route_index); arc_insertion_position++){
								
								int node_at_position = S.R.at(route_index).at(arc_insertion_position);
								
								// If node in position is pickup node, it means that it is the beginning of a segment.
								// Thus, arcs can only be inserted at these positions
								if (S.Z.at(node_at_position) == 9999){
									
									if (S.W.at(route_index) + deltaInsertionArc("time", S, pickup_node, delivery_node, route_index, arc_insertion_position) < routes_max_length){
										
										// Boolean variable
										// feasible_positions = true;
										
										// std::cout << "Arc " << pickup_node << "-" << delivery_node << " can be inserted in route " << route_index << " at position " << arc_insertion_position << std::endl;
										
										double delta_costs = deltaInsertionArc("costs", S, pickup_node, delivery_node, route_index, arc_insertion_position);
										
										// Available demand in arc: minimum value between pickup node current capacity, vehicle capacity and unmet demand of client
										double available_demand = std::min(S.inst.Q.at(route_index), S.G.at(pickup_node));
										double unmet_demand = std::abs(S.inst.d.at(delivery_node)) - S.G.at(delivery_node);
										available_demand = std::min(available_demand, unmet_demand);
										
										// If available demand is greater than zero:
										
										if (available_demand > 0){
											
											
											double delta_epsilon = deltaEpsilonInsertionArc(S, delivery_node, available_demand);
											
											// Normalizing values
											double norm_delta_costs = (delta_costs - 60)/(2000 - 60);
											
											double norm_delta_epsilon = (delta_epsilon - (-0.03))/(0.03 - (-0.03));
											
											// std::cout << "Delta in costs: " << delta_costs << std::endl;
											
											// std::cout << "Delta in epsilon: " << delta_epsilon << std::endl;
											
											double score = norm_delta_epsilon*gamma1 + norm_delta_costs*gamma2;
											
											// double score = delta_epsilon*100000 + delta_costs;
											
											// std::cout << "Score of this insertion: " << score << "\n\n";
											
											// If arc insertion corresponds to minimum score found in search
											if (score < min_score){
												
												// Updating values with corresponding data from iteration
												min_score = score;
												// Corresponding pickup node of minimum score
												min_score_pickup_node = pickup_node;
												// Corresponding pickup node of minimum score
												min_score_delivery_node = delivery_node;
												// Corresponding route of minimum score
												min_score_route = route_index;
												// Corresponding position of minimum score
												min_score_position = arc_insertion_position;
												// Corresponding transferred demand of minimum score
												min_score_transferred_demand = available_demand;
												// Boolean variable
												feasible_positions = true;
												
											}
										
										}
									}
									
									
								}
								
								
							}
							
						}
						
					}
					
				}
				
			}
			
			if (feasible_positions){
				
				// std::cout << "Inserting arc " << min_score_pickup_node << "-" << min_score_delivery_node << " in route " << min_score_route << " at position " << min_score_position << " with score " << min_score <<std::endl;
				
				S.insertArcAt(min_score_pickup_node, min_score_delivery_node, min_score_route, min_score_position, min_score_transferred_demand);
				
			}
			
			
		// Here begins split insertion code:
		} else {
			
			
			
		}
	
	}
	
	S.tidyUp();
	
	
	/*
	// Route max length - maybe there's a better way for doing that!
	double routes_max_length = S.inst.T*S.inst.w_b.at(0);
	
	// Second part of insertion - Giving feasibility to solution regarding epsilon value
	
{
	// Available nodes to be inserted: starts with all nodes in D
	std::vector<int> available_nodes = {};
	
	double global_epsilon = 0.05;
	
	// Threshold for min best score
	// double min_best_score = -500;
	double min_best_score = 0;
	
	// Route max length - maybe there's a better way for doing that!
	double routes_max_length = S.inst.T*S.inst.w_b.at(0);
	
	// Max epsilon
	double max_epsilon = *std::max_element(S.epsilon.begin(), S.epsilon.end());
	
	int it = 0;
	
	while ((max_epsilon > global_epsilon)){
		
		///////
		
		it += 1;
		if (it>100){
			break;
		}
		
		///////
		
		
		// Available nodes to be inserted: starts with all nodes in D
		
		// available_nodes = S.unmet_demand_clients;
		std::vector<int> available_nodes = S.inst.D;
		// std::vector<int> available_nodes = S.unmet_demand_clients;
		
		// Taking out fully served clients
		for (auto node: available_nodes){
			
			if (S.Z.at(node) == 1){
				
				available_nodes.erase(std::remove_if(available_nodes.begin(), available_nodes.end(), [&node](int value) -> bool { return value == node; }), available_nodes.end());
				
			}
			
		}
		
		// Minimum score found so far: great scores are negative scores!
		double min_score = 9999;
		
		// Corresponding node of minimum score, for insertion after, before and replacement
		int min_score_node = {};
		
		// Corresponding nodes for arc insertion - Pickup and Delivery nodes
		int min_score_pickup_node = {};
		
		int min_score_delivery_node = {};
		
		// Corresponding route of minimum score
		int min_score_route = {};
		
		// Corresponding position of minimum score
		int min_score_position = {};
		
		// Corresponding transferred demand of minimum score
		double min_score_transferred_demand = {};
		
		// Boolean variable, that controls if best score corresponds to a insertion before
		bool insertion_before = false;
		
		// Boolean variable, that controls if best score corresponds to a insertion after
		bool insertion_after = false;
		
		// Boolean variable, that controls if best score corresponds to a replacement
		bool replacement = false;
		
		// Boolean variable, that controls if best score corresponds to a P-D insertion
		bool arc_insertion = false;
		
		// Iterating, for all available nodes for insertion, for all routes and positions
		
		for (auto &receiver_node_index : available_nodes){
			
			for (int route_index {0}; route_index < S.inst.m; route_index++){
				
				// Nodes can be inserted from position 2 of S, as position 0 is depot and position 1 is first pickup node
				for (int position_index {2}; position_index < S.RSize.at(route_index); position_index++){
					
					int source_node_index = S.R.at(route_index).at(position_index);
					
					// Node before source_node_index -> this is used to treat the case when the source_node_index is different than 
					// the receiver_node_index, but the node before source_node_index is equal to receiver_node_index!
					
					int node_before_source = S.R.at(route_index).at(position_index - 1);
					
					// If source_node_index is not pickup node
					if ((S.Z.at(source_node_index) != 9999) and (source_node_index != receiver_node_index)){
						
						// Checking feasibilities and, if feasible, scores
						int position_before = position_index;
						int position_after = position_index + 1;
						
						double delta_epsilon, transferred_demand;
						
						// Feasibility for inserting node before source node
						if ((S.W.at(route_index) + deltaInsertion("time", S, receiver_node_index, route_index, position_before) < routes_max_length) and (receiver_node_index != node_before_source)){
							
							// Calculating scores
							
							// Epsilon
							std::tie(delta_epsilon, transferred_demand) = deltaEpsilonInsertion(S, source_node_index, receiver_node_index, route_index, position_index);
							
							// Normalizing epsilon:
							double norm_delta_epsilon = (delta_epsilon - (-0.05) )/(0.03 - (-0.05));
							
							// Costs
							double delta_costs_insertion_before = deltaInsertion("cost", S, receiver_node_index, route_index, position_before);
							
							// Normalizing costs:
							double norm_delta_costs_insertion_before = (delta_costs_insertion_before - 2100)/(2100 - (-300));
							
							// Unmet demand (naturally normalized) - the bigger this value, the better!
							double norm_delta_unmet_demand = transferred_demand/std::abs(S.inst.d.at(receiver_node_index));
							
							// Score:
							double norm_score_insertion_before = norm_delta_epsilon*gamma1 + norm_delta_costs_insertion_before*gamma2 - norm_delta_unmet_demand*gamma3;
							
							// std::cout << "Delta epsilon: " << delta_epsilon << "," << std::endl;
							// std::cout << "Delta costs: " << delta_costs_insertion_before << "," << std::endl;
							// std::cout << "Delta unmet demand: " << norm_delta_unmet_demand << "," << std::endl;
							// std::cout << "Normalized delta epsilon:" << norm_delta_epsilon << "," << std::endl;
							// std::cout << "Normalized delta costs:" << norm_delta_costs_insertion_before << "," << std::endl;
							// std::cout << "Normalized delta unmet demand:" << norm_delta_unmet_demand << "," << std::endl;
							// std::cout << "Normalized score: " << norm_score_insertion_before << ",\n\n" << std::endl;
							
							if (norm_score_insertion_before < min_score){
								
								// Updating values with corresponding data from iteration
								// min_score = score_insertion_before;
								min_score = norm_score_insertion_before;
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
								arc_insertion = false;
								
							}
						}
						
						// Feasibility for inserting node after source node
						if ((S.W.at(route_index) + deltaInsertion("time", S, receiver_node_index, route_index, position_after) < routes_max_length) and (position_after <= S.RSize.at(route_index))){
						//if ((S.W.at(route_index) + deltaInsertion("time", S, receiver_node_index, route_index, position_after) < routes_max_length) and (S.Z.at(position_after) != 9999)){
							
							// Calculating scores
							
							// Epsilon
							std::tie(delta_epsilon, transferred_demand) = deltaEpsilonInsertion(S, source_node_index, receiver_node_index, route_index, position_index);
							
							// Normalizing epsilon
							double norm_delta_epsilon = (delta_epsilon - (-0.05) )/(0.03 - (-0.05));
							
							// Costs:
							double delta_costs_insertion_after = deltaInsertion("cost", S, receiver_node_index, route_index, position_after);
							
							// Normalizing costs:
							double norm_delta_costs_insertion_after = (delta_costs_insertion_after - 2100)/(2100 - (-300));
							
							// Unmet demand (naturally normalized)
							double norm_delta_unmet_demand = transferred_demand/std::abs(S.inst.d.at(receiver_node_index));;
							
							// Score:
							double norm_score_insertion_after = norm_delta_epsilon*gamma1 + norm_delta_costs_insertion_after*gamma2 - norm_delta_unmet_demand*gamma3;
							
							if (norm_score_insertion_after < min_score){
								
								// Updating values with corresponding data from iteration
								// min_score = score_insertion_after;
								min_score = norm_score_insertion_after;
								// Corresponding node of minimum score
								min_score_node = receiver_node_index;
								// Corresponding route of minimum score
								min_score_route = route_index;
								// Corresponding position of minimum score
								min_score_position = position_after;
								// Corresponding transferred demand of minimum score
								min_score_transferred_demand = transferred_demand;
								// Boolean variables
								replacement = false;
								insertion_after = true;
								insertion_before = false;
								arc_insertion = false;
								
							}
							
							// std::cout << "Score: " << score_insertion_after << "\n\n";
							
						}
						// Feasibility for replacing source node for receiving node
						if (S.W.at(route_index) + deltaReplacement("time", S, receiver_node_index, route_index, position_index) < routes_max_length){
							
							// Calculating scores
							
							// Epsilon
							std::tie(delta_epsilon, transferred_demand) = deltaEpsilonInsertion(S, source_node_index, receiver_node_index, route_index, position_index);
							
							// Normalizing epsilon
							double norm_delta_epsilon = (delta_epsilon - (-0.05) )/(0.03 - (-0.05));
							
							// Costs:
							double delta_costs_replacement = deltaReplacement("cost", S, receiver_node_index, route_index, position_index);
							
							// Normalizing costs:
							double norm_delta_costs_replacement = (delta_costs_replacement - 2100)/(2100 - (-300));
							
							// Unmet demand (naturally normalized)
							double norm_delta_unmet_demand = transferred_demand/std::abs(S.inst.d.at(receiver_node_index));;
							
							// Score:
							double norm_score_replacement = norm_delta_epsilon*gamma1 + norm_delta_costs_replacement*gamma2 - norm_delta_unmet_demand*gamma3;
							
							
							// Replacement has an additional constraint: I can only replace if transferring all demand doesn't impact epsilon sums!
							if ((norm_score_replacement < min_score) and (transferred_demand == std::abs(S.z.at(route_index).at(position_index)))){
								
								// std::cout << "\n\n\nREPLACEMENT POSITION HAS BECOME THE BEST ONE\n\n\n" << std::endl;
								
								// Updating values with corresponding data from iteration
								// min_score = score_replacement;
								min_score = norm_score_replacement;
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
								arc_insertion = false;
								
							}
						}
					}
				}
				
				// std::cout << "\n\n\n";
			}
		}
		
		
		// Other phase of insertion method - finding out scores for inserting P-D arcs!
		
		for (auto &delivery_node: S.unmet_demand_clients){
			
			for (auto &pickup_node: S.inst.P){
				
				if (S.G.at(pickup_node) > 0){
					
					// std::cout << "Possible arc: " << pickup_node << "-" << node << ", with " << S.G.at(pickup_node) << " available demand." <<std::endl;
					
					// With arc, all possible insertion positions are searched
					// An arc cannot be inserted at the middle of a segment!
					for (auto route_index {0}; route_index < S.inst.m; route_index++){
						
						// Starts in one because of depot node
						for (auto arc_insertion_position {1}; arc_insertion_position < S.RSize.at(route_index); arc_insertion_position++){
							
							int node_at_position = S.R.at(route_index).at(arc_insertion_position);
							
							// If node in position is pickup node, it means that it is the beginning of a segment.
							// Thus, arcs can only be inserted at these positions
							if (S.Z.at(node_at_position) == 9999){
								
								if (S.W.at(route_index) + deltaInsertionArc("time", S, pickup_node, delivery_node, route_index, arc_insertion_position) < routes_max_length){
									
									// Calculating scores
									
									// Available demand in arc: minimum value between pickup node current capacity, vehicle capacity and current node unmet demand
									double available_demand = std::min(S.inst.Q.at(route_index), S.G.at(pickup_node));
									
									//////// JUST ADDED, IT CAN LEAD TO A BUG!!!!!!!!
									available_demand = std::min(available_demand, (S.G.at(node_at_position) - std::abs(S.inst.d.at(node_at_position))));
									
									// Epsilon:
									double delta_epsilon = deltaEpsilonInsertionArc(S, delivery_node, available_demand);
									
									// Normalizing epsilon:
									double norm_delta_epsilon = (delta_epsilon - (-0.05) )/(0.03 - (-0.05));
									
									// Costs:
									double delta_costs = deltaInsertionArc("costs", S, pickup_node, delivery_node, route_index, arc_insertion_position);
									
									// Normalizing costs:
									double norm_delta_costs = (delta_costs - 2100)/(2100 - (-300));
									
									// Unmet demand (naturally normalized)
									double norm_delta_unmet_demand = available_demand/std::abs(S.inst.d.at(delivery_node));;
									
									double norm_score = norm_delta_epsilon*gamma1 + norm_delta_costs*gamma2 - norm_delta_unmet_demand*gamma3;
									
									// If arc insertion corresponds to minimum score found in search
									if (norm_score < min_score){
										
										// Updating values with corresponding data from iteration
										min_score = norm_score;
										// Corresponding pickup node of minimum score
										min_score_pickup_node = pickup_node;
										// Corresponding pickup node of minimum score
										min_score_delivery_node = delivery_node;
										// Corresponding route of minimum score
										min_score_route = route_index;
										// Corresponding position of minimum score
										min_score_position = arc_insertion_position;
										// Corresponding transferred demand of minimum score
										min_score_transferred_demand = available_demand;
										// Boolean variables
										replacement = false;
										insertion_after = false;
										insertion_before = false;
										arc_insertion = true;
									
									
									}
									
									
								}
								
								
							}
							
							
						}
						
					}
					
				}
				
				// std::cout << "\n\n";
				
			}
			
		}
		
		std::cout << "Minimum score on iteration: " << min_score << std::endl;
		
		// Checking if mininum found score is better than threshold
		if (min_best_score < min_score){
			
			break;
			
		}
		
		//std::cout << "\n\nMinimum found score: " << min_score << std::endl;
		
		//std::cout << "Corresponding route: " << min_score_route << std::endl;
		// Corresponding position of minimum score
		//std::cout << "Corresponding position at route: " << min_score_position  << std::endl;
		// Corresponding source node
		
		//std::cout << "Corresponding demand to be transferred: " << min_score_transferred_demand << std::endl;
		// Boolean variable, that controls if best score corresponds to a replacement
		//std::cout << "Is it a replacement? -> " << replacement << std::endl;
		//std::cout << "Is it an insertion_before? -> " << insertion_before << std::endl;
		//std::cout << "Is it an insertion_after? -> " << insertion_after << std::endl;
		//std::cout << "Is it an arc_insertion? -> " << arc_insertion << std::endl;
		
		// Making changes in Solution object
		
		if (insertion_before){
			
			S.splitInsertion("before", min_score_node, min_score_route, min_score_position, min_score_transferred_demand);
			
		} else if (insertion_after){
			
			S.splitInsertion("after", min_score_node, min_score_route, min_score_position, min_score_transferred_demand);
			
		} else if (replacement){
		
			S.replaceNodeAt(min_score_node, min_score_route, min_score_position);
			
		} else if (arc_insertion){
			
			S.insertArcAt(min_score_pickup_node, min_score_delivery_node, min_score_route, min_score_position, min_score_transferred_demand);
			
		}
		
		
		max_epsilon = *std::max_element(S.epsilon.begin(), S.epsilon.end());
		
		// std::string a;
		
		// std::cin >> a;
		
		// S.printSol();
		
		// break;
	}
	
	
	
	// Last phase
	
	// std::cout << "\n\nBeginning last phase\n\n" << std::endl;
	
	bool feasible_positions = true;
	
	
	while (feasible_positions){
		
		// Updating values with corresponding data from iteration
		double min_score = 9999;
		// Corresponding pickup node of minimum score
		int min_score_pickup_node = {};
		// Corresponding pickup node of minimum score
		int min_score_delivery_node = {};
		// Corresponding route of minimum score
		int min_score_route = {};
		// Corresponding position of minimum score
		int min_score_position = {};
		// Corresponding transferred demand of minimum score
		double min_score_transferred_demand = {};
		
		feasible_positions = false;
		
		for (auto &delivery_node: S.unmet_demand_clients){
			
			for (auto &pickup_node: S.inst.P){
				
				if (S.G.at(pickup_node) > 0){
					
					// With arc, all possible insertion positions are searched
					// An arc cannot be inserted at the middle of a segment!
					for (auto route_index {0}; route_index < S.inst.m; route_index++){
						
						// Starts in one because of depot node
						for (auto arc_insertion_position {1}; arc_insertion_position < S.RSize.at(route_index); arc_insertion_position++){
							
							int node_at_position = S.R.at(route_index).at(arc_insertion_position);
							
							// If node in position is pickup node, it means that it is the beginning of a segment.
							// Thus, arcs can only be inserted at these positions
							if (S.Z.at(node_at_position) == 9999){
								
								if (S.W.at(route_index) + deltaInsertionArc("time", S, pickup_node, delivery_node, route_index, arc_insertion_position) < routes_max_length){
									
									// Boolean variable
									// feasible_positions = true;
									
									// std::cout << "Arc " << pickup_node << "-" << delivery_node << " can be inserted in route " << route_index << " at position " << arc_insertion_position << std::endl;
									
									double delta_costs = deltaInsertionArc("costs", S, pickup_node, delivery_node, route_index, arc_insertion_position);
									
									// Available demand in arc: minimum value between pickup node current capacity and vehicle capacity
									double available_demand = std::min(S.inst.Q.at(route_index), S.G.at(pickup_node));
									
									double delta_epsilon = deltaEpsilonInsertionArc(S, delivery_node, available_demand);
									
									// std::cout << "Delta in costs: " << delta_costs << std::endl;
									
									// std::cout << "Delta in epsilon: " << delta_epsilon << std::endl;
									
									double score = delta_epsilon*100000 + delta_costs;
									
									// std::cout << "Score of this insertion: " << score << "\n\n";
									
									// If arc insertion corresponds to minimum score found in search
									if (score < min_score){
										
										// Updating values with corresponding data from iteration
										min_score = score;
										// Corresponding pickup node of minimum score
										min_score_pickup_node = pickup_node;
										// Corresponding pickup node of minimum score
										min_score_delivery_node = delivery_node;
										// Corresponding route of minimum score
										min_score_route = route_index;
										// Corresponding position of minimum score
										min_score_position = arc_insertion_position;
										// Corresponding transferred demand of minimum score
										min_score_transferred_demand = available_demand;
										// Boolean variable
										feasible_positions = true;
										
									}
									
									
								}
								
								
							}
							
							
						}
						
					}
					
				}
				
				// std::cout << "\n\n";
				
			}
			
		}
		
		if (feasible_positions){
			
			// S.printSol();
			
			// std::cout << "Inserting arc " << min_score_pickup_node << "-" << min_score_delivery_node << " in route " << min_score_route << " at position " << min_score_position << " with score " << min_score <<std::endl;
			
			// std::string a;
			
			// std::cin >> a;
			
			S.insertArcAt(min_score_pickup_node, min_score_delivery_node, min_score_route, min_score_position, min_score_transferred_demand);
			
		} else {
			
			break;
			
		}
		
		
	}
//}
	*/
	
	return S;
	
}


