#include "Sol.hpp"
#include <ctime>
#include <random>
#include <chrono>
#include <algorithm>


// Initialization already constructs initial solution!

// By doing so, it become easier to avoid solution copies.

Sol::Sol(Instance &inst_val, double &p, double &Gamma1, double &Gamma2){
	
	// Replicable data
	srand(120);
	
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
	
	// nodesPositions attribute (initializing with empty structures)
	
	std::vector<std::vector<int>> empty_2d_vec = {};
	std::vector<int> empty_1d_vec = {};
	
	for (auto route_index {0}; route_index < inst.m; route_index++){
		empty_2d_vec.push_back(empty_1d_vec);
	}
	
	nodesPositions.resize(inst.counties.size());
	std::fill(nodesPositions.begin(), nodesPositions.end(), empty_2d_vec);
	
	// Filling depot met demands with very large number
	
	for (auto &i: inst.D){
		
		Z.at(i) = 0;
		
	}
	
	// Amount of goods, at the beginning only defined in pickup nodes
	
	G.resize(inst.counties.size());
	for (auto &i: inst.P){
		
		G.at(i) = inst.d.at(i);
		
	}
	
	// W vector, with routes lengths in terms of time
	W.resize(inst.m);
	std::fill(W.begin(), W.end(), 0);
	
	// RSize vector, with routes lengths in terms of number of nodes - initiates in 1 because of depot node
	RSize.resize(inst.m);
	std::fill(RSize.begin(), RSize.end(), 1);
	
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
	
	
};

Sol::~Sol()
{
}


// Removing node by passing specific positions (Used by all other removal methods)

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
		
		W.at(route_index) -= inst.t.at(node_index).at(last_but_one_node).at(route_index);
		
		
	} else {
		
		// If node to be removed is pickup node, then the whole segment is removed
		if (Z.at(node_index) == 9999){
			
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
			
			for (int index {starting_index - 1}; index < final_index + 1 ;index++){
				
				int i = R.at(route_index).at(index);
				
				int j = R.at(route_index).at(index + 1);
				
				// std::cout << i << "-" << j << ": " << inst.t.at(i).at(j).at(route_index) << std::endl;
				
				old_segment_time += inst.t.at(i).at(j).at(route_index);
				
			}
			
			// Travel time of new arc
			
			// First node of new arc
			int first_node = R.at(route_index).at(starting_index - 1);
			
			// Second node of new arc
			int second_node = R.at(route_index).at(final_index + 1);
			
			double new_arc_time = 0;
			
			if (!last_segment){
				
				double new_arc_time = inst.t.at(first_node).at(second_node).at(route_index);
				
			}
			
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
	
	// Variable used for removal positions in pairs
	
	// The explanation is that, to remove full segments in a route, it is possible to always remove the same index at "x" times, being "x" the segment size
	// But this is not possible for nodesPositions attribute, as the indexes are static
	// int removal_position_in_pair = removal_index;
	
	for (int segment_element {0}; segment_element < segment_size; segment_element++){
		
		// Node which is being deleted (information used to update nodesPositions!)
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
		
		// Updating Z attribute - Only at delivery nodes
		if (load < 0){
			
			Z.at(current_node) -= load/inst.d.at(node_index);
			
		}
		
		// Search for pair position to be removed
		
		// The pair position that will be removed has "node_index" as key, and the corresponding pair first element is "route_index" and second element is "removal_index"
		
		// The great advantage of this approach is that, instead of iterating in all routes and positions of solution to remove a node,
		// it is possible to iterate only in node specific occurrences, which drastically reduces the iterating process
		
		/*
		int occurrence = 0;
		
		for (int node_occurrence {0}; node_occurrence < nodesPositions.at(current_node).size(); node_occurrence++){
			
			
			// If current occurrence is the one that I'm trying to find, I store this and break loop
			if ((nodesPositions.at(current_node).at(node_occurrence).first == route_index) and (nodesPositions.at(current_node).at(node_occurrence).second == removal_position_in_pair)){
				
				occurrence = node_occurrence;
				
				// std::cout << current_node << " " << " " << route_index << " " << removal_position_in_pair << "\n\n\n\n";
				
				removal_position_in_pair += 1;
				
				break;
				
			}
			
		}
		
		// Finally deleting position in nodesPositions attribute
		nodesPositions.at(current_node).erase(nodesPositions.at(current_node).begin() + occurrence);
		
		*/
	}
	
	
	
}
		/*
		
		nodesPositions.at(current_node).at(route_index).erase(std::remove_if(nodesPositions.at(current_node).at(route_index).begin(), nodesPositions.at(current_node).at(route_index).end(), [&removal_position_in_pair](int value) -> bool { return value == removal_position_in_pair; }), nodesPositions.at(current_node).at(route_index).end());
		
		
		
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
			
			// std::cout << "\n";
			
		}
				
				
				
	
		
	}
	
	
	*/


// Removes random case
void Sol::removeNodeCase(int &node_index){
	
	// Node needs to have already been visited in solution for removal to be possible!
	
	// This is done by checking:
	// if pickup node (Z == 9999), G needs to be smaller than d
	// if delivery node (Z != 9999), Z needs to be greater than 1
	
	if (((Z.at(node_index) == 9999) and (G.at(node_index) < inst.d.at(node_index))) or ((Z.at(node_index) != 9999) and (Z.at(node_index) > 0.0001))){
		
		// Getting random route
		
		int random_route = rand()%inst.m;
		
		// Node need to be on route 
		
		while (nodesPositions.at(node_index).at(random_route).size() == 0){
			
			random_route = rand()%inst.m;
			
		}
		
		// Getting random position
		
		int number_of_occurrences = nodesPositions.at(node_index).at(random_route).size();
		
		int random_occasion = nodesPositions.at(node_index).at(random_route).at(rand()%number_of_occurrences);
		
		std::cout << "\n\n\n\n" << random_route << " " <<  node_index << " " << random_occasion << "\n\n\n\n" << std::endl;
		
		// Removing it from solution
		removeNodeAt(random_route, random_occasion);
		
		
	} else {
		
		std::cout << "BUG: Node not visited in solution" << std::endl;
		
	}
	
	
	
}


/*
// Removes all cases
void Sol::removeNodeCases(int &node_index){
	
	// Node needs to have already been visited in solution for removal to be possible!
	
	if (((Z.at(node_index) == 9999) and (G.at(node_index) < inst.d.at(node_index))) or ((Z.at(node_index) != 9999) and (Z.at(node_index) > 0))){
		
		// For each route
		for (auto route_index {0}; route_index < inst.m; route_index++){
			
			int number_of_positions = nodesPositions.at(node_index).at(route_index).size();
			
			int available_positions = number_of_positions;
			
			std::cout << available_positions << std::endl;
			
			while (available_positions > 0){
				
				// printInt(nodesPositions.at(node_index).at(route_index));
				
				std::cout << "A" << std::endl;
				
				removeNodeAt(route_index, nodesPositions.at(node_index).at(route_index).at(0));
				
				// printInt(nodesPositions.at(node_index).at(route_index));
				
				// break;
			}
			
			//for (auto position {0}; position < number_of_positions; position++){
				
				
			//}
			
			
			
			/*
			// Positions of node at current route
			std::vector<int> node_route_positions(nodesPositions.at(node_index).at(route_index));
			
			int number_of_positions = node_route_positions.size();
			
			// Removing while there are available removal positions
			while (number_of_positions > 0){
				
				printInt(node_route_positions);
				
				int removal_position = node_route_positions.at(0);
				
				removeNodeAt(route_index, removal_position);
				
				// Updating positions vector after removing node
				node_route_positions.erase(node_route_positions.begin());
				
				number_of_positions -= 1;
				
				
				for (auto i {0}; i < number_of_positions; i++){
					
					node_route_positions.at(i) -= 1;
					
				}
				
				// printInt(node_route_positions);
				
				
				
			}
			 */
		// }
		
		/*
		// Iterating at all possible positions in which "node_index" could be
		for (auto route_index {0}; route_index < nodesPositions.at(node_index).size(); route_index++){
			
			
			
			for (auto node_position: nodesPositions.at(node_index).at(route_index)){
				
				// Removing it from solution
				removeNodeAt(route_index, node_position);
				std::cout << "A" << std::endl;
				
			}
			
			printInt(nodesPositions.at(node_index).at(route_index));
			
		}
		
		
		
	} else {
		
		std::cout << "BUG: Node not visited in solution" << std::endl;
		
	}
	
}
*/



// Insert node at specific position

void Sol::insertNodeAt(int &node_index, int &route_index, int &insertion_index, double &demand){
	
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
	
	nodesPositions.at(node_index).at(route_index).push_back(insertion_index);
	
	// Counting picked-up/delivered demand at corresponding attributes
	
	// If pickup node
	if (Z.at(node_index) == 9999){
		
		G.at(node_index) -= demand;
		z.at(route_index).insert(z.at(route_index).begin() + insertion_index, demand);
		
	// If delivery node
	} else {
		
		
		G.at(node_index) += demand;
		Z.at(node_index) += demand/std::abs(inst.d.at(node_index));
		z.at(route_index).insert(z.at(route_index).begin() + insertion_index, -demand);
		
	}
	

	
	
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
	
	double total_met_demand = {};
		
	for (int i; i < inst.m; i++){
	
		for (auto value: z.at(i)){
			
			if (value < 0){
				
				total_met_demand += std::abs(value);
				
			}
		}
	}
	
	double total_demand = {};
	
	for (auto &node: inst.P){
		
		total_demand += inst.d.at(node);
		
	}
	
	std::cout << total_met_demand << " / "  << total_demand << "\n\n";
	
	std::cout << "Relative met demand at each delivery node: \n" << std::endl;
	
	std::vector<double> relative_met_demands = {};
	
	for (int node; node < Z.size(); node++){
	
		if (Z.at(node) <= 1){
			
			std::cout << node << ": " << G.at(node) << " / " << std::abs(inst.d.at(node)) << " = " << Z.at(node) << "\n";
			relative_met_demands.push_back(Z.at(node));
		}
		
	}
	
	double sum = std::accumulate(std::begin(relative_met_demands), std::end(relative_met_demands), 0.0);
	double m =  sum / relative_met_demands.size();

	double accum = 0.0;
	std::for_each (std::begin(relative_met_demands), std::end(relative_met_demands), [&](const double d) {
		accum += (d - m) * (d - m);
	});

	double stdev = std::sqrt(accum / (relative_met_demands.size()-1));
	
	std::cout << "\n\nStandard deviation of met demands: " << stdev << "\n\n" << std::endl;
	
	
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
		
			fw << "[ ";
			
			for (auto &node: route){
				
				fw << node << " ";
			}
			
			fw << "]\n";
			
		}
		
		fw << "\n\n";
		
		// Writing total met demand data
		
		double total_met_demand = {};
		
		for (int i; i < inst.m; i++){
		
			for (auto value: z.at(i)){
				
				if (value < 0){
					
					total_met_demand += std::abs(value);
					
				}
			}
		}
		
		fw << total_met_demand <<"\n\n";
		
		
		// Writing met demands
		
		for (int node; node < Z.size(); node++){
		
			if (Z.at(node) <= 1){
				
				fw << node << " " << Z.at(node) << "\n";
				
			}
			
		}
		
		
	}
	else std::cout << "Problem with opening file";
	
	
	fw.close();
	
	
}
