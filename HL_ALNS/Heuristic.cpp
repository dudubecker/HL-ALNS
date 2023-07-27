#include "Heuristic.hpp"
#include <ctime>
#include <random>
#include <chrono>
#include <algorithm>
#include <numeric>

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
int RemovalHeuristic::chooseNumberofNodes(Sol &S) {
	
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
	
	int mi = RemovalHeuristic::chooseNumberofNodes(S);
	
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
	
	int mi = RemovalHeuristic::chooseNumberofNodes(S);
	
	// Choosing random node
	
	
	// Choosing subset of nodes in neighborhood of "node"
	
	
	// While "mi" nodes haven't been removed or while there's still cases of nodes to be removed in neighborhood
	
	
	return S;
}

//// InsertionHeuristic objects implementation