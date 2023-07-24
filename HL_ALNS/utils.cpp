#include "utils.h"
#include <iostream>
#include <vector>
#include <map>

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

/*
// For printing nodesPositions attribute:
for (auto node_index {0}; node_index < S.nodesPositions.size(); node_index++){
	
	std::cout << node_index << ": ";
	
	for(auto position_pair: S.nodesPositions.at(node_index)){
		
		
		std::cout << position_pair.first << " " << position_pair.second << "; ";
		
		
	}
	
	std::cout << "\n\n";
	
}
*/
