#include "Heuristic.hpp"

// Delta methods - insertion, removal (of individual nodes, arcs and segments), replacement, regarding time, cost and epsilon

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

double Heuristic::deltaInsertionArc(std::string delta_type, Sol &S, int &first_arc_node_index, int &second_arc_node_index , int &route_index, int &insertion_index){
	
	// Returned variable "delta"
	double delta = {};
	
	// Checking if it's last position
	
	bool last_position = false;
	
	if (insertion_index == (S.RSize.at(route_index))){
		
		last_position = true;
		
	}
	
	if (delta_type == "time"){
		
		double inserted_arc_time = S.inst.t.at(first_arc_node_index).at(second_arc_node_index).at(route_index);
		
		if (last_position){
			
			int node_before = S.R.at(route_index).at(insertion_index - 1);
			
			double first_arc_time = S.inst.t.at(node_before).at(first_arc_node_index).at(route_index);
			
			delta = first_arc_time + inserted_arc_time;
			
		} else {
			
			int node_before = S.R.at(route_index).at(insertion_index - 1);
			
			int node_after = S.R.at(route_index).at(insertion_index);
			
			double first_arc_time = S.inst.t.at(node_before).at(first_arc_node_index).at(route_index);
			
			double second_arc_time = S.inst.t.at(second_arc_node_index).at(node_after).at(route_index);
			
			double old_arc_time = S.inst.t.at(node_before).at(node_after).at(route_index);
			
			delta = first_arc_time + inserted_arc_time + second_arc_time - old_arc_time;
			
		}
		
	} else if (delta_type == "costs"){
		
		double inserted_arc_cost = S.inst.c.at(first_arc_node_index).at(second_arc_node_index).at(route_index);
		
		if (last_position){
			
			int node_before = S.R.at(route_index).at(insertion_index - 1);
			
			double first_arc_cost = S.inst.c.at(node_before).at(first_arc_node_index).at(route_index);
			
			delta = first_arc_cost + inserted_arc_cost;
			
		} else {
			
			int node_before = S.R.at(route_index).at(insertion_index - 1);
			
			int node_after = S.R.at(route_index).at(insertion_index);
			
			double first_arc_cost = S.inst.c.at(node_before).at(first_arc_node_index).at(route_index);
			
			double second_arc_cost = S.inst.c.at(second_arc_node_index).at(node_after).at(route_index);
			
			double old_arc_cost = S.inst.c.at(node_before).at(node_after).at(route_index);
			
			delta = first_arc_cost + inserted_arc_cost + second_arc_cost - old_arc_cost;
			
		}
		
	} else {
		
		std::cout << "Not a valid type for 'delta_type' parameter!" << std::endl;
		
	}
	
	return delta;
	
}

std::pair<double, double> Heuristic::deltaEpsilonInsertion(Sol &S, int &source_node_index, int &receiver_node_index, int &route_index, int &position_index){

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

double Heuristic::deltaEpsilonInsertionArc(Sol &S, int &delivery_node_index, double &demand){
	
	double old_epsilon = S.epsilon.at(delivery_node_index);
	
	double old_met_demand = S.G.at(delivery_node_index);
	
	double new_met_demand = old_met_demand + demand;
	
	double new_epsilon = std::abs((new_met_demand/S.totalZ) - (std::abs(S.inst.d.at(delivery_node_index))/S.totalD));
	
	// double delta_epsilon = old_epsilon - new_epsilon;
	
	double delta_epsilon = new_epsilon - old_epsilon;
	
	return delta_epsilon;
	
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

double Heuristic::deltaRemovalSegment(std::string delta_type, Sol &S, int &pickup_node_index, int &route_index, int &removal_index){
	
	// Variable to be returned
	double delta = {};
	
	// Checking if segment is the last segment
	bool last_segment = true;
	
	// Segment size
	int segment_size = 0;
	
	// Determining if it's last segment and its size
	
	for (auto node_position {removal_index}; node_position < S.RSize.at(route_index); node_position++){
		
		int node = S.R.at(route_index).at(node_position);
		
		if ((S.Z.at(node) == 9999) and (node_position > removal_index)){
			
			last_segment = false;
			
			break;
			
		}
		
		segment_size += 1;
		
	}
	
	// Time delta
	if (delta_type == "time"){
		
		if (last_segment){
			
			// Node before segment
			int node_before = S.R.at(route_index).at(removal_index - 1);
			
			// Old arc time
			double old_arc_time = S.inst.t.at(node_before).at(pickup_node_index).at(route_index);
			
			// Segment time
			double segment_time = 0;
			
			for (auto node_position {removal_index};node_position < removal_index + segment_size - 1; node_position++){
				
				int first_arc_node = S.R.at(route_index).at(node_position);
				
				int second_arc_node = S.R.at(route_index).at(node_position + 1);
				
				// std::cout << first_arc_node << "-" << second_arc_node << std::endl;
				
				double arc_time = S.inst.t.at(first_arc_node).at(second_arc_node).at(route_index);
				
				segment_time += arc_time;
				
			}
			
			// Finally, determining delta
			delta = - (old_arc_time + segment_time);
			
		} else {
			
			// Node before segment
			int node_before = S.R.at(route_index).at(removal_index - 1);
			
			// Node after segment
			int node_after = S.R.at(route_index).at(removal_index + segment_size);
			
			// Segment time
			double segment_time = 0;
			
			for (auto node_position {removal_index};node_position < removal_index + segment_size - 1; node_position++){
				
				int first_arc_node = S.R.at(route_index).at(node_position);
				
				int second_arc_node = S.R.at(route_index).at(node_position + 1);
				
				double arc_time = S.inst.t.at(first_arc_node).at(second_arc_node).at(route_index);
				
				segment_time += arc_time;
				
			}
			
			// First old arc time
			double first_old_arc_time = S.inst.t.at(node_before).at(pickup_node_index).at(route_index);
			
			// Second old arc time
			int last_node_segment = S.R.at(route_index).at(removal_index + segment_size - 1);
			double second_old_arc_time = S.inst.t.at(last_node_segment).at(node_after).at(route_index);
			
			
			// New arc time
			double new_arc_time = S.inst.t.at(node_before).at(node_after).at(route_index);
			
			
			// Finally, determining delta
			delta = new_arc_time - (first_old_arc_time + second_old_arc_time + segment_time);
			
		}
		
	} else if (delta_type == "cost"){
		
		if (last_segment){
			
			// Node before segment
			int node_before = S.R.at(route_index).at(removal_index - 1);
			
			// Old arc cost
			double old_arc_cost = S.inst.c.at(node_before).at(pickup_node_index).at(route_index);
			
			// Segment cost
			double segment_cost = 0;
			
			for (auto node_position {removal_index};node_position < removal_index + segment_size - 1; node_position++){
				
				int first_arc_node = S.R.at(route_index).at(node_position);
				
				int second_arc_node = S.R.at(route_index).at(node_position + 1);
				
				// std::cout << first_arc_node << "-" << second_arc_node << std::endl;
				
				double arc_cost = S.inst.c.at(first_arc_node).at(second_arc_node).at(route_index);
				
				segment_cost += arc_cost;
				
			}
			
			// Finally, determining delta
			delta = - (old_arc_cost + segment_cost);
			
		} else {
			
			// Node before segment
			int node_before = S.R.at(route_index).at(removal_index - 1);
			
			// Node after segment
			int node_after = S.R.at(route_index).at(removal_index + segment_size);
			
			// Segment cost
			double segment_cost = 0;
			
			for (auto node_position {removal_index};node_position < removal_index + segment_size - 1; node_position++){
				
				int first_arc_node = S.R.at(route_index).at(node_position);
				
				int second_arc_node = S.R.at(route_index).at(node_position + 1);
				
				double arc_cost = S.inst.c.at(first_arc_node).at(second_arc_node).at(route_index);
				
				segment_cost += arc_cost;
				
			}
			
			// First old arc cost
			double first_old_arc_cost = S.inst.c.at(node_before).at(pickup_node_index).at(route_index);
			
			// Second old arc cost
			int last_node_segment = S.R.at(route_index).at(removal_index + segment_size - 1);
			double second_old_arc_cost = S.inst.c.at(last_node_segment).at(node_after).at(route_index);
			
			
			// New arc cost
			double new_arc_cost = S.inst.c.at(node_before).at(node_after).at(route_index);
			
			
			// Finally, determining delta
			delta = new_arc_cost - (first_old_arc_cost + second_old_arc_cost + segment_cost);
			
		}
		
	} else {
		
		std::cout << "Not a valid delta_type for deltaRemovalSegment method!" << std::endl;
		
	}
	
	
	return delta;
	
}

double Heuristic::deltaEpsilonRemoval(Sol &S, int &node_index, int &route_index, int &removal_index){
	
	double old_epsilon = S.epsilon.at(node_index);
	
	double old_met_demand = S.G.at(node_index);
	
	double removed_demand = std::abs(S.z.at(route_index).at(removal_index));
	
	double new_met_demand = old_met_demand - removed_demand;
	
	double new_epsilon = std::abs((new_met_demand/S.totalZ) - (std::abs(S.inst.d.at(node_index))/S.totalD));
	
	double delta_epsilon = new_epsilon - old_epsilon;
	
	return delta_epsilon;
}

double Heuristic::deltaEpsilonRemovalSegment(Sol &S, int &pickup_node_index, int &route_index, int &removal_index){
	
	return 0;
	
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

