#include "Sol.hpp"
#include <ctime>
#include <random>
#include <chrono>
#include <algorithm>
#include <iomanip>


// Initialization already constructs initial solution!

// By doing so, it become easier to avoid solution copies.

Sol::Sol(Instance &inst_val, double &p, double &Gamma1, double &Gamma2){
	
	
	std::cout << "\n\nConstructed solution: \n\n";
	
	// Replicable data
	// srand(120);
	
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
	
	// Filling depot met demands with very large number
	Z.resize(inst.counties.size());
	std::fill(Z.begin(), Z.end(), 9999);
	
	for (auto &i: inst.D){
		
		Z.at(i) = 0;
		
	}
	
	// nodesPositions attribute (initializing with empty structures)
	
	// std::vector<std::vector<int>> empty_2d_vec = {};
	// std::vector<int> empty_1d_vec = {};
	
	// for (auto route_index {0}; route_index < inst.m; route_index++){
	// 	empty_2d_vec.push_back(empty_1d_vec);
	// }
	
	// nodesPositions.resize(inst.counties.size());
	// std::fill(nodesPositions.begin(), nodesPositions.end(), empty_2d_vec);
	
	

	
	// Amount of goods, at the beginning only defined in pickup nodes
	
	G.resize(inst.counties.size());
	for (auto &i: inst.P){
		
		G.at(i) = inst.d.at(i);
		
	}
	
	// Clients with unmet demand - starts as all clients in D
	unmet_demand_clients = inst.D;
	
	// W vector, with routes lengths in terms of time
	W.resize(inst.m);
	std::fill(W.begin(), W.end(), 0);
	
	// RSize vector, with routes lengths in terms of number of nodes - initiates in 1 because of depot node
	RSize.resize(inst.m);
	std::fill(RSize.begin(), RSize.end(), 1);
	
	// total D attribute - used in epsilon calculations
	for (auto &node: inst.D){
		
		totalD += std::abs(inst.d.at(node));
		
	}
	
	// epsilon vector with 9999 epsilon values (depot and pickups):
	epsilon.resize(inst.counties.size());
	std::fill(epsilon.begin(), epsilon.end(), -9999);
	
	
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
			
			// If it is possible to insert node
			if ((count(inst.P.begin(), inst.P.end(), node)) && (G.at(node) > 0)){
				
				next_pickup_node = node;
				
				break;
				
			}
			
		}
		
		
		// Inserting node at route
		// R.at(route_index).push_back(next_pickup_node);
		
		// Insertion position -> last position
		int insertion_index = RSize.at(route_index);
		
		// Amount of load picked up - minimum between vehicle capacity and amount of goods in pickup node
		double picked_up_load = std::min(inst.Q.at(route_index), G.at(next_pickup_node));
		
		insertNodeAt(next_pickup_node, route_index, insertion_index, picked_up_load);
		
		
		// If insertion is the first one, epsilon values can be initialized, as totalZ will be different than 0!
		if (totalZ == picked_up_load){
			
			for (auto &i: inst.D){
				
		 		epsilon.at(i) = std::abs((G.at(i)/totalZ) - (std::abs(inst.d.at(i))/totalD));
				
		 	}
			
		}
		
		
		
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
			
			// Calculating score for each possible node
			
			for (auto &node: inst.N){
				
				double score {};
				
				double index_proximity = std::find(sorted_proximities.begin(),sorted_proximities.end(), node) - sorted_proximities.begin();
				
				// double index_met_demand = std::find(sorted_met_demands.begin(),sorted_met_demands.end(), node) - sorted_met_demands.begin();
				
				// If node is not depot or pickup - this enables checking that statement very quickly, instead of looking for node in setP/setS
				if (Z.at(node) != 9999){
					
					//score = Gamma1*index_proximity + Gamma2*index_met_demand;
					score = (Gamma1)*(index_proximity/inst.N.size()) + (Gamma2)*(Z.at(node));
					
					
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
				
				epsilon = ((double) rand() / (RAND_MAX));
				
				index_delivery_node = round(pow(epsilon, p)*((sorted_scores).size()-1));
				
				next_delivery_node = sorted_scores.at(index_delivery_node);
				
				
				// if (Z.at(next_delivery_node) < 0.9999999){
				if (Z.at(next_delivery_node) < 1){
					
					unvalid_node = false;
					
				}
				
			}
			
			// Amount of delivered load - minimum between available load and amount of goods demanded
			double delivered_load = std::min(available_load, (std::abs(inst.d.at(next_delivery_node)) - G.at(next_delivery_node)));
			
			// Insertion position -> last position
			int insertion_index = RSize.at(route_index);
				
			insertNodeAt(next_delivery_node, route_index, insertion_index, delivered_load);
			
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
		
		
	}
	
	// Excluding last nodes until all routes are feasible in period
	
	for (int route_index {0}; route_index < inst.m; route_index++){
		
		while (W.at(route_index) > routes_max_length){
			
			int last_node_position = R.at(route_index).size() - 1;
			removeNodeAt(route_index, last_node_position);
		}
		
	}
	
	
	
	
	
	
};

Sol::~Sol()
{
}

// Removes node by passing specific positions (Used by all other removal methods)
void Sol::removeNodeAt(int &route_index, int &removal_index){
	
	// Corresponding node on removal position
	int node_index = R.at(route_index).at(removal_index);
	
	// Nodes indexes to be removed by operation
	int segment_size = 1;
	
	// "node_index" can never be the first node on route! But it can be the last one (before "post-processing" in which final depots are included)
	
	// Updating W attribute first, as it is arc dependent
	
	// If node is the last one on route, calculation of W is different
	bool last_node = false;
	if (removal_index == (R.at(route_index).size() - 1)){
		
		last_node = true;
		
	}
	
	if (last_node){
		
		int last_but_one_node = R.at(route_index).at(removal_index - 1);
		
		W.at(route_index) -= inst.t.at(last_but_one_node).at(node_index).at(route_index);;
		
	} else {
		
		// If node to be removed is pickup node, then the whole segment is removed
		if (Z.at(node_index) == 9999){
			
			// std::cout << "Segment to be removed" << std::endl;
			
			// Size of route
			int route_size = R.at(route_index).size();
			
			// Boolean variable for controlling if it is last segment
			bool last_segment = false;
			
			
			// Starting index of segment, which is the own removal index
			int starting_index = removal_index;
			
			// Final index of segment, to be incremented
			int final_index = removal_index;
			
			// Amount load/unloaded in next node, which is the stop criteria - if it gets positive, it means the segment is ended
			double load_on_node = z.at(route_index).at(final_index + 1);
			
			while (load_on_node < 0){
				
				// Check later if it's right! But I guess it's making sense
				
				final_index += 1;
				
				// Incrementing segment size
				segment_size += 1;
				
				
				// Treating case when segment is last element on route
				if ((final_index + 1) == route_size){
					
					final_index -= 1;
					last_segment = true;
					break;
					
				} else {
					
					load_on_node = z.at(route_index).at(final_index + 1);
					
				}
				
			}
			
			// Travel time of old segment (for loop)
			double old_segment_time = 0;
			
			// std::cout << "\n";
			
			for (int index {starting_index - 1}; index < final_index + 1 ;index++){
				
				int i = R.at(route_index).at(index);
				
				int j = R.at(route_index).at(index + 1);
				
				// std::cout << i << "-" << j << std::endl;
				// inst.t.at(i).at(j).at(route_index) << std::endl;
				
				old_segment_time += inst.t.at(i).at(j).at(route_index);
				
			}
			
			// std::cout << "\n";
			
			// Travel time of new arc
			
			// First node of new arc
			int first_node = R.at(route_index).at(starting_index - 1);
			
			// Second node of new arc
			int second_node = R.at(route_index).at(final_index + 1);
			
			// std::cout << "New arc: " << first_node << "-" << second_node << "\n" << std::endl;
			
			double new_arc_time = 0;
			
			if (!last_segment){
				
				new_arc_time = inst.t.at(first_node).at(second_node).at(route_index);
				
			}
			
			// std::cout << "New arc time: " << new_arc_time << std::endl;
			// std::cout << "Old segment time: " << old_segment_time << std::endl;
			
			// Variation of time
			double delta_time = new_arc_time - old_segment_time;
			
			
			// Incrementing time attribute
			W.at(route_index) += delta_time;
			
			
		// If node to be remove is delivery node, then only the single node is removed
		} else {
			
			// Arc time travel difference
			
			int first_node = R.at(route_index).at(removal_index - 1);
			
			int second_node = R.at(route_index).at(removal_index + 1);
			
			double first_old_arc_time = inst.t.at(first_node).at(node_index).at(route_index);
			
			double second_old_arc_time = inst.t.at(node_index).at(second_node).at(route_index);
			
			double new_arc_time = inst.t.at(first_node).at(second_node).at(route_index);
			
			double delta_time = new_arc_time - (first_old_arc_time + second_old_arc_time);
			
			W.at(route_index) += delta_time;
			
			
		}
		
		
	}
	
	
	for (int segment_element {0}; segment_element < segment_size; segment_element++){
		
		// Node which is being deleted
		int current_node =  R.at(route_index).at(removal_index);
		
		// Removing node from route
		R.at(route_index).erase(R.at(route_index).begin() + removal_index);
		
		// Storing load picked-up/delivered in visit
		double load = z.at(route_index).at(removal_index);
		
		// Removing position of visit in z attribute
		z.at(route_index).erase(z.at(route_index).begin() + removal_index);
		
		// Updating G attribute - Signal of load is convenient here
		G.at(current_node) += load;
		
		// Removing node to nodes counter attribute
		RSize.at(route_index) -= 1;
		
		// Updating attributes - Only at delivery nodes
		if (load < 0){
			
			
			// Including client in unmet demand, if it was previously fully served
			// if (Z.at(current_node) > 0.999){
			if ((Z.at(current_node) >= 1) and (!count(unmet_demand_clients.begin(), unmet_demand_clients.end(), current_node))){
				
				unmet_demand_clients.push_back(current_node);
				
			}
			
			Z.at(current_node) = G.at(current_node)/std::abs(inst.d.at(current_node));
			
			totalZ -= std::abs(load);
			
			
			
			
		}
		
		
		/*
		// "nodesPositions" code starts here
		
		// The great advantage of this approach is that, instead of iterating in all routes and positions of solution to remove a node,
		// it is possible to iterate only in node specific occurrences, which drastically reduces the iterating process
		
		
		nodesPositions.at(current_node).at(route_index).erase(std::remove_if(nodesPositions.at(current_node).at(route_index).begin(), nodesPositions.at(current_node).at(route_index).end(), [&removal_index](int value) -> bool { return value == removal_index; }), nodesPositions.at(current_node).at(route_index).end());
		
		// Updating positions of other nodes in route (only nodes after the removed node!)
		
		// Terrible way to update positions:
		
		std::vector<int> updated_nodes = {};
		
		for (auto node_position {removal_index}; node_position < RSize.at(route_index); node_position++){
			
			int node_in_route = R.at(route_index).at(node_position);
			
			int cont = 0;
			
			if (!count(updated_nodes.begin(), updated_nodes.end(), node_in_route)){
				
				for (auto &occurrence: nodesPositions.at(node_in_route).at(route_index)){
					
					if (occurrence > node_position){
						
						// std::cout << occurrence - 1 << std::endl;
						
						nodesPositions.at(node_in_route).at(route_index).at(cont) = occurrence - 1;
						
						
					}
					
					updated_nodes.push_back(node_in_route);
					cont += 1;
				}
				
			}
			
		}
		
		// "nodesPositions" code ends here
		*/
	}
	
	updateEpsilon();
}

void Sol::removeRandomNode(){
	
	// Getting random route
	int random_route = rand()%inst.m;
		
	// Getting random position at route, not including position 0!
	int random_position = 1 + rand()%(RSize.at(random_route) - 1);
	// int random_position = 1 + rand()%(RSize(random_route) - 2);
	
	removeNodeAt(random_route, random_position);
	
}


// Removes random case
void Sol::removeNodeCase(int &node_index){
	
	// Node needs to have already been visited in solution for removal to be possible!
	
	// This is done by checking:
	// if pickup node (Z == 9999), G needs to be smaller than d
	// if delivery node (Z != 9999), Z needs to be greater than 1
	
	if (((Z.at(node_index) == 9999) and (G.at(node_index) < std::abs(inst.d.at(node_index)))) or ((Z.at(node_index) != 9999) and (Z.at(node_index) > 0.001))){
		
		// Getting random route
		int random_route = rand()%inst.m;
		
		// Getting random position at route, not including position 0!
		int random_position = 1 + rand()%(RSize.at(random_route) - 2);
		
		// Node in route
		int node = R.at(random_route).at(random_position);
		
		while (node != node_index){
			
			// Getting random route
			random_route = rand()%inst.m;
			
			// Getting random position at route, not including position 0!
			random_position = 1 + rand()%(RSize.at(random_route) - 1);
			
			// Node in route
			node = R.at(random_route).at(random_position);
			
		}
		
		// The search process occurs randomly until the node in position is equal to the desired node
		
		// Removing it from solution
		removeNodeAt(random_route, random_position);
		
		// std::cout << node_index << " " << random_route << " " << random_occasion << " " << W.at(random_route) << std::endl;
		
		
	} else {
		
		//std::cout << "Node not visited in solution" << std::endl;
		
	}
	
	
	
}



// Replace node at specific route and position, used in insertion methods
void Sol::replaceNodeAt(int node_index, int route_index, int replace_index){
	
	// Demand in position
	double demand = std::abs(z.at(route_index).at(replace_index));
	
	// Removing old node in position
	removeNodeAt(route_index, replace_index);
	
	// Inserting new node in position, with corresponding demand
	insertNodeAt(node_index, route_index, replace_index, demand);
	
}

// Inserts node at specific position
void Sol::insertNodeAt(int &node_index, int &route_index, int &insertion_index, double &demand){
	
	// Trying to fix bug: clients reveicing more than demand
	if ((Z.at(node_index) != 9999)){
		
		double unmet_demand = std::abs(inst.d.at(node_index)) - G.at(node_index);
		
		demand = std::min(demand, unmet_demand);
		
	}
	
	
	
	// Boolean to control exception 1 - Inserted node is equal to left node
	bool exception1 = false;
	
	// Time attribute - first one to be updated!
	
	// Process is different if insertion position is the last one!
	if (insertion_index == RSize.at(route_index)){
		
		int last_node_index = R.at(route_index).back();
		
		double new_arc_time = inst.t.at(last_node_index).at(node_index).at(route_index);
		
		W.at(route_index) += new_arc_time;
		
		
	// If it's not last position, arcs difference needs to be taken into account
	} else {
		
		int old_arc_first_node = R.at(route_index).at(insertion_index - 1);
		
		int old_arc_second_node = R.at(route_index).at(insertion_index);
		
		double old_arc_time = inst.t.at(old_arc_first_node).at(old_arc_second_node).at(route_index);
		
		double first_new_arc_time = inst.t.at(old_arc_first_node).at(node_index).at(route_index);
		
		double second_new_arc_time = inst.t.at(node_index).at(old_arc_second_node).at(route_index);
		
		double delta_time = (first_new_arc_time + second_new_arc_time) - old_arc_time;
		
		W.at(route_index) += delta_time;
		
	}
	
	// Inserting node in routes attribute
	R.at(route_index).insert(R.at(route_index).begin() + insertion_index, node_index);
	
	// Updating routes length in number of nodes
	RSize.at(route_index) += 1;
	
	// nodesPositions.at(node_index).at(route_index).push_back(insertion_index);
	
	// Counting picked-up/delivered demand at corresponding attributes
	
	// If pickup node
	if (Z.at(node_index) == 9999){
		
		G.at(node_index) -= demand;
		z.at(route_index).insert(z.at(route_index).begin() + insertion_index, demand);
		
	// If delivery node
	} else {
		
		
		///////// For fixing bug of consecutive nodes of same client in insertion!
		if (insertion_index != RSize.at(route_index) - 1){
			
			if (node_index == R.at(route_index).at(insertion_index - 1)){
				
				
				std::cout << "\n\n\n HERE IS THE EXCEPTION 0!!! \n\n\n";
				
				// demand = demand + std::abs(z.at(route_index).at(insertion_index));
				
				
				
			} else if (node_index == R.at(route_index).at(insertion_index + 1)){
				
				
				// std::cout << "\n\n\n HERE IS THE EXCEPTION 2!!! \n\n\n";
				
				// std::cout << demand << " " << z.at(route_index).at(insertion_index + 1) << std::endl;
				
				// std::cout << "\n\n\n\n\n\n\n\n" << demand << "\n\n\n\n\n\n\n\n";
				
				// std::cout << "\n\n\n\n\n\n\n\n" << z.at(route_index).at(insertion_index) << "\n\n\n\n\n\n\n\n";
				
				// std::cout << "\n\n\n HERE IS THE EXCEPTION!!! \n\n\n";
				
				demand = demand + std::abs(z.at(route_index).at(insertion_index));
				removeNodeAt(route_index, insertion_index);
				
				
				
			}
			
		} else {
			
			if (node_index == R.at(route_index).at(insertion_index - 1)){
				
				
		 		std::cout << "\n\n\n HERE IS THE EXCEPTION 1!!! \n\n\n";
				
				demand = demand + std::abs(z.at(route_index).at(insertion_index - 1));
				
				exception1 = true;
				
			}
			
			
			
			
		}
		/////////
		
		
		G.at(node_index) += demand;
		totalZ += demand;
		// Z.at(node_index) += demand/std::abs(inst.d.at(node_index));
		Z.at(node_index) = G.at(node_index)/std::abs(inst.d.at(node_index));
		z.at(route_index).insert(z.at(route_index).begin() + insertion_index, -demand);
		
		// if (Z.at(node_index) > 0.999){
		if (Z.at(node_index) == 1){
			
			
			unmet_demand_clients.erase(std::remove_if(unmet_demand_clients.begin(), unmet_demand_clients.end(), [&node_index](int value) -> bool { return value == node_index; }), unmet_demand_clients.end());
			
		}
		
	}
	
	
	
	// Left node is equal to inserted node
	if (exception1){
		
		int equal_node_position = insertion_index - 1;
		
		removeNodeAt(route_index, equal_node_position);
		
	}
	
	
	updateEpsilon();
	
	
}

void Sol::insertArcAt(int pickup_node_index, int delivery_node_index, int route_index, int insertion_index, double demand){
	
	// Indexes of insertion
	int pickup_insertion_index = insertion_index;
	int delivery_insertion_index = insertion_index + 1;
	
	// Inserting pickup node
	insertNodeAt(pickup_node_index, route_index, pickup_insertion_index, demand);
	
	// Inserting delivery node, after pickup node
	insertNodeAt(delivery_node_index, route_index, delivery_insertion_index, demand);
	
}

// Performs a split insertion
void Sol::splitInsertion(std::string how ,int receiver_node_index, int route_index, int insertion_index, double splitted_demand){
	
	// Splits demand with node before "insertion_index"
	if (how == "before"){
		
		// Total demand splitted between the two nodes
		double total_demand = std::abs(z.at(route_index).at(insertion_index));
		
		// Source node
		int source_node_index = R.at(route_index).at(insertion_index);
		
		
		// Removing source node
		removeNodeAt(route_index, insertion_index);
		
		// Inserting receiver node
		insertNodeAt(receiver_node_index, route_index, insertion_index, splitted_demand);
		
		int source_node_new_position = insertion_index + 1;
		double source_node_new_demand = total_demand - splitted_demand;
		
		// Inserting source node after receiver node
		insertNodeAt(source_node_index, route_index, source_node_new_position, source_node_new_demand);
		
	// Splits demand with node after "insertion_index"
	} else if (how == "after"){
		
		// Total demand splitted between the two nodes
		double total_demand = std::abs(z.at(route_index).at(insertion_index - 1));
		
		// Source node
		int source_node_index = R.at(route_index).at(insertion_index - 1);
		
		// Source node position in route
		int source_node_position = insertion_index - 1;
		
		// Removing source node
		removeNodeAt(route_index, source_node_position);
		
		// Inserting receiver node in source node original position
		insertNodeAt(receiver_node_index, route_index, source_node_position, splitted_demand);
		
		double source_node_new_demand = total_demand - splitted_demand;
		
		// Inserting source node before receiver node
		insertNodeAt(source_node_index, route_index, source_node_position, source_node_new_demand);
		
		
		
		
		// Removing source node
		// removeNodeAt(route_index, insertion_index);
		
		// Inserting receiver node
		// insertNodeAt(receiver_node_index, route_index, insertion_index, splitted_demand);
		
		// int source_node_new_position = insertion_index - 1;
		// double source_node_new_demand = total_demand - splitted_demand;
		
		// Inserting source node before receiver node
		// insertNodeAt(source_node_index, route_index, source_node_new_position, source_node_new_demand);
		
		
	} else {
		
		std::cout << "Not a valid type!" << std::endl;
		
	}
	
	
}

// Inserts node at specific position
void Sol::updateEpsilon(){
	
	for (auto &i: inst.D){
		
		epsilon.at(i) = std::abs((G.at(i)/totalZ) - (std::abs(inst.d.at(i))/totalD));
		
	}
	
}

// Checking if solution contains all node(s)
bool Sol::containsAll(std::vector<int> &nodes_vector){
	
	bool all_nodes_in_solution = true;
	
	for (auto node_index: nodes_vector){
		
	// Node needs to have already been visited in solution for removal to be possible!
	
	// This is done by checking:
	// if pickup node (Z == 9999), G needs to be smaller than d
	// if delivery node (Z != 9999), Z needs to be greater than 1
		
		if (((Z.at(node_index) == 9999) and (G.at(node_index) < inst.d.at(node_index))) or ((Z.at(node_index) != 9999) and (Z.at(node_index) > 0.0001))){
			;
		} else {
			
			all_nodes_in_solution = false;
			
		}
		
	}
	
	return all_nodes_in_solution;
	
}

bool Sol::containsAny(std::vector<int> &nodes_vector){
	
	bool any_node_in_solution = false;
	
	for (auto node_index: nodes_vector){
		
	// Node needs to have already been visited in solution for removal to be possible!
	
	// This is done by checking:
	// if pickup node (Z == 9999), G needs to be smaller than d
	// if delivery node (Z != 9999), Z needs to be greater than 1
		
		if (((Z.at(node_index) == 9999) and (G.at(node_index) < inst.d.at(node_index))) or ((Z.at(node_index) != 9999) and (Z.at(node_index) > 0.0001))){
			any_node_in_solution = true;
		} else {
			
			;
			
		}
		
	}
	
	return any_node_in_solution;
	
}


void Sol::tidyUp(){
	
	// For each node in solution
	for (auto route_index {0}; route_index < inst.m; route_index++){
		
		// For each position
		for (auto node_position {1}; node_position < RSize.at(route_index); node_position++){
			
			// Actual node index
			int node = R.at(route_index).at(node_position);
			
			// If it's pickup node and there's still pickup capacity
			if ((Z.at(node) == 9999) and (G.at(node) > 0)){
				
				// Check if picked-up amount is lower than vehicle capacity
				if (z.at(route_index).at(node_position) < inst.Q.at(route_index)){
					
					// Amount of load that I can add to segment
					// It's the minimum value between available load on pickup point and the difference of vehicle capacity and picked up amount!
					double transferrable_load = std::min((inst.Q.at(route_index) - z.at(route_index).at(node_position)),(G.at(node)));
					
					// If it's possible to transfer any demand at all
					if (transferrable_load > 0){
						
						// std::cout << "Pickup node " << node << " has " << transferrable_load << " transferrable load at route " << route_index << " and position " << node_position << std::endl;
						
						// Calculating unmet demand of segment
						double segment_unmet_demand = 0;
						
						// Vector of old picked up and delivered amounts in segment
						std::vector<double> segment_old_z = {z.at(route_index).at(node_position)};
						
						// Vector for clients in segment
						std::vector<int> segment_nodes = {node};
						
						for (auto node_segment_position {node_position + 1}; node_segment_position < RSize.at(route_index); node_segment_position++){
							
							int node_at_segment = R.at(route_index).at(node_segment_position);
							
							
							// If pickup node, segment is over
							if (Z.at(node_at_segment) == 9999){
								
								break;
								
							}
							
							double unmet_demand = std::abs(inst.d.at(node_at_segment)) - G.at(node_at_segment);
							
							// std::cout << "Client " << node_at_segment << " has " << unmet_demand << " unmet demand." << std::endl;
							
							// Storing old delivered amount, for next step
							segment_old_z.push_back(z.at(route_index).at(node_segment_position));
							
							// Storing node in segment
							segment_nodes.push_back(node_at_segment);
							
							segment_unmet_demand += unmet_demand;
							
						}
						
						// If my segment has any client with unmet demands:
						if (segment_unmet_demand > 0){
							
							// The load that I can add to the pickup visit is the minimum value
							// between the full segment unmet demand and the transferrable demand
							
							transferrable_load = std::min(transferrable_load, segment_unmet_demand);
							
							// New amount picked up in visit - it's equal to transferrable demand plus the old collected demand!
							double new_pickup_load = transferrable_load + z.at(route_index).at(node_position);
							
							// Vector of new picked up and delivered amounts in segment
							std::vector<double> segment_new_z = segment_old_z;
							segment_new_z.at(0) = new_pickup_load;
							
							// Variable that controls how much load was added to segment
							double total_additional_load = 0;
							
							// Variable that controls the position _in segment_ (iterations)
							int it {1};
							
							// For each delivery node on segment
							for (auto node_segment_position {node_position + 1}; node_segment_position < RSize.at(route_index); node_segment_position++){
								
								int node_at_segment = R.at(route_index).at(node_segment_position);
								
								// Unmet demand of client
								double unmet_demand = std::abs(inst.d.at(node_at_segment)) - G.at(node_at_segment);
								
								// Additional load - minimum value between transferrable load and client current unmet demand
								double additional_load = std::min(unmet_demand, transferrable_load);
								
								// Updating new z values with additional load at position
								double new_z = segment_old_z.at(it) - additional_load;
								segment_new_z.at(it) = new_z;
								
								total_additional_load += additional_load;
								
								
								if (total_additional_load == transferrable_load){
									
									break;
									
								}
								
								it += 1;
								
							}
							
							// printDouble(segment_old_z);
							// printDouble(segment_new_z);
							
							// Manually updating values, without remove/insert operators:
							// This is done because by removing/inserting nodes, main for loops bug, leading to errors in solutions!
							
							// Variable that stores iteration (position in segment)
							
							it = 0;
							
							// Adding nodes again
							for (auto node_segment_position {node_position}; node_segment_position < RSize.at(route_index); node_segment_position++){
								
								int node_at_segment = segment_nodes.at(it);
								double demand_at_segment = std::abs(segment_new_z.at(it));
								
								// If pickup node
								if (Z.at(node_at_segment) == 9999){
									
									// Updating z value
									z.at(route_index).at(node_segment_position) = demand_at_segment;
									
									// Updating G value
									G.at(node_at_segment) -= std::abs(std::abs(segment_new_z.at(it)) - std::abs(segment_old_z.at(it)));
									
									// Updating global Z value
									totalZ += (segment_new_z.at(it) - segment_old_z.at(it));
									
									
								// If delivery node
								} else {
									
									// Updating z value
									z.at(route_index).at(node_segment_position) = -demand_at_segment;
									
									// Updating G value
									G.at(node_at_segment) += std::abs(std::abs(segment_new_z.at(it)) - std::abs(segment_old_z.at(it)));
									
									// Updating Z value
									Z.at(node_at_segment) = G.at(node_at_segment)/std::abs(inst.d.at(node_at_segment));
									
								}
								
								it += 1;
								
								// If segment has ended, loop breaks
								if (it == segment_nodes.size()){
									
									break;
								}
							}
							
							
							
						}
						
						
					}
					
					
					
				}
				
			}
			
		}
		
		
	}
	
	
	
	// Updating epsilons
	updateEpsilon();
	
	
}

//// Print/Output methods

void Sol::printSol(){
	
	std::cout << "Routes (R): \n" << std::endl;
	
	for (auto &route: R){
		
		std::cout << "[ ";
		
		for (auto &node: route){
			
			std::cout << node << " ";
		}
		
		std::cout << "]\n";
		
	}
	
	std::cout << "\n\nAmounts collected/delivered (z): \n" << std::endl;
	
	for (auto &route: z){
		
			std::cout << "[ ";
			
			for (auto &node: route){
				
				std::cout << node << " ";
			}
			
			std::cout << "]\n";
			
	}
	
	std::cout << "\n\nTotal met demand: \n" << std::endl;
	
	std::cout << totalZ << " / "  << totalD;
	
	std::cout << "\n\nTraveling times of routes: \n" << std::endl;
	
	double traveling_costs = {};
	
	for (int route_index {0}; route_index < R.size(); route_index++){
		
		double route_traveling_time = 0;
		
		for (int node_index {0}; node_index < R.at(route_index).size() - 1; node_index++){
			
			int first_arc_node = R.at(route_index).at(node_index);
			
			int second_arc_node = R.at(route_index).at(node_index  + 1);
			
			double arc_costs = inst.c.at(first_arc_node).at(second_arc_node).at(route_index);
			
			double arc_time = inst.t.at(first_arc_node).at(second_arc_node).at(route_index);
			
			traveling_costs += arc_costs;
			route_traveling_time += arc_time;
			
		}
		
		std::cout << "Route " << route_index << ": " << route_traveling_time << std::endl;
		
	}
	
	std::cout << "\n\nTraveling costs: " << traveling_costs << std::endl;
	
	std::cout << "\nRelative met demand at each delivery node: \n" << std::endl;
	
	std::vector<double> relative_met_demands = {};
	
	for (int node; node < Z.size(); node++){
	
		if (Z.at(node) <= 1.01){
			
			std::cout << node << ": " << G.at(node) << " / " << std::abs(inst.d.at(node)) << " = " << Z.at(node) << "\n";
			relative_met_demands.push_back(Z.at(node));
		}
		
	}
	
	double sum = std::accumulate(std::begin(relative_met_demands), std::end(relative_met_demands), 0.0);
	double m =  sum / relative_met_demands.size();
	
	std::cout << "\nMean of met demands: " << m << std::endl;
	
	std::cout << "\nMinimum met demand: " << *std::min_element(relative_met_demands.begin(), relative_met_demands.end()) << std::endl;
	
	// Counting number of totally unmet demands
	
	int unmet_demand_count = 0;
    for (double met_demand : relative_met_demands) {
	
		if (met_demand == 0) {
			
			unmet_demand_count++;
		}
	}
	
	std::cout << "\nNumber of totally unmet demands: " << unmet_demand_count << std::endl;
	
	
	double accum = 0.0;
	std::for_each (std::begin(relative_met_demands), std::end(relative_met_demands), [&](const double d) {
		accum += (d - m) * (d - m);
	});

	double stdev = std::sqrt(accum / (relative_met_demands.size()-1));
	
	std::cout << "\nStandard deviation of met demands: " << stdev << "\n\n" << std::endl;
	
	std::cout << "\nEpsilons: \n" << std::endl;
	
	
	int cont = 0;
	for (auto &e: epsilon){
		
		if (e > -9999){
			
			std::cout << cont << ": " << e*100 << "%\n";
			
		}
		cont += 1;
		
	}
	
	double sum_epsilon = 0;
	int size = 0;
	for (double num : epsilon) {
		
		if (num >= 0){
			sum_epsilon += num;
			size += 1;
		}
        
    }
	
	double mean_epsilon =  sum_epsilon / size;
	
	std::cout << "\nMean of epsilons: " << mean_epsilon*100 << "%" << std::endl;
	std::cout << "\nMax epsilon: " <<  *std::max_element(epsilon.begin(), epsilon.end())*100 << "%\n" << std::endl;
}

void Sol::toTXT(std::string &file_name){
	
	// Removing ".txt" from file_name
	file_name.erase(file_name.length() - 4);
	std::string output_name = file_name + "_OUTPUT.txt";
	
	std::ofstream fw(output_name, std::ofstream::out);
	
	// Writing data
	if (fw.is_open())
	{
		// Writing number of periods
		fw << inst.T << "\n\nR:\n";
		
		// Writing routes data
		for (auto &route: R){
		
			
			for (auto &node: route){
				
				fw << node << " ";
			}
			
			fw << "\n";
			
		}
		
		fw << "\nz:\n";
		
		// Writing collected/delivered demand at each visit
		// Writing routes data
		for (auto &route: z){
			
			
			for (auto &node: route){
				
				fw << node << " ";
			}
			
			fw << "\n";
			
		}
		
		fw << "\n\n";
		
	}
	else std::cout << "Problem with opening file";
	
	
	fw.close();
	
	
}
