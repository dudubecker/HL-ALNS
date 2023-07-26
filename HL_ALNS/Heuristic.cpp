#include "Heuristic.hpp"
#include <ctime>
#include <random>
#include <chrono>
#include <algorithm>
#include <numeric>

// Implementação do método "apply" da classe base "Heuristic"
Sol Heuristic::apply(Sol &S){
    std::cout << "Comportamento base de Heuristic\n";
    // Coloque aqui o código base que deve ser comum a todas as classes derivadas

    // Chamada para o método "specificApply" específico da classe derivada
    // Isso garantirá que o comportamento específico da classe derivada seja chamado após o código base.
	specificApply(S);
	
	return S;
	
}


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