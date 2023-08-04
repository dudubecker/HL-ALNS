#include "utils.h"
#include <iostream>
#include <vector>
#include <map>
#include <cmath>


// Comparator function to sort pairs
// according to second value
bool cmp(std::pair<int, double>& a,
        std::pair<int, double>& b)
{
    return a.second < b.second;
};
 
// Function to sort the map according
// to value in a (key-value) pairs
std::vector<int> sortIndexes(std::vector<double> &input_vector){
	
	std::vector<int> return_vector {};
	
	std::map<int, double> M;
	
	for (auto i = 0; i < input_vector.size(); i ++){
		
		M[i] = input_vector.at(i);
		
	}
	
    // Declare vector of pairs
    std::vector<std::pair<int, double>> A;
 
    // Copy key-value pair from Map
    // to vector of pairs
    for (auto& it : M) {
        A.push_back(it);
    }
 
    // Sort using comparator function
    sort(A.begin(), A.end(), cmp);
 
    // Print the sorted value
    for (auto& it : A) {
		
		return_vector.push_back(it.first);
		
        // cout << it.first << ' '
        //     << it.second << endl;
    }
	
	return return_vector;
	
};


void printDouble(std::vector<double> &input)
{
    for (auto const &i: input) {
        std::cout << i << " ";
    }
	
	std::cout << std::endl;
	
};

void printInt(std::vector<int> &input)
{
    for (auto const &i: input) {
        std::cout << i << " ";
    }
	
	std::cout << std::endl;
	
};

double roundToDecimalPlaces(double &value, int decimalPlaces) {
    double multiplier = std::pow(10.0, decimalPlaces);
    return std::round(value * multiplier) / multiplier;
}

// double deltaInsertion(Sol &S, int &node_index, int &route_index, int &insertion_index){
	
	
// }


/*
// For printing nodesPositions attribute:
for (auto node_index {0}; node_index < S.nodesPositions.size(); node_index++){
	
	std::cout << node_index << ": ";
	
	for(auto position_pair: S.nodesPositions.at(node_index)){
		
		
		std::cout << position_pair.first << " " << position_pair.second << "; ";
		
		
	}
	
	std::cout << "\n\n";
	
}
 * 
 * 
 * 
 * // Splitting "S" into segments
	
	for (int route_index {0}; route_index < S.inst.m; route_index++){
		
		for (auto &node: S.R.at(route_index)){
			
			if ((S.Z.at(node) == 9999)){
				
				cout << "\n";
				
			}
			
			cout << node << " ";
			
			
		}
		
		
		
	}

// Applying heuristic of heuristic vector
 * 
// Criando um vetor de smart pointers para Heuristic (classe base)
std::vector<Heuristic*> heuristicVector;

// Criando objetos das classes derivadas e adicionando ao vetor
	
PartialRandomRemoval prr {};
	
heuristicVector.push_back(&prr);

heuristicVector.at(0) -> apply();
*/