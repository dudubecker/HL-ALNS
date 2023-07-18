#ifndef SOL_HPP
#define SOL_HPP
#include <vector>
#include "Instance.hpp"

class Sol
{
public:

	// Attributes
	
	// Instance object of solution
	Instance inst;
	
	// Routes - 2D vector with concatenated routes
	std::vector<std::vector<int>> R {};
	
	// Total met demand for each client
	std::vector<double> Z {};
	
	// Amount delivered for each visit
	std::vector<std::vector<double>> z {};
	
	// Total duration of each route
	std::vector<double> W {};
	
	// Initialization by passing instance object
	Sol(Instance &inst_val);
	
	~Sol();
	
	//// Methods
	
	// Printing solution
	void printSol();
	
	// FO solution
	void objectiveFunction();
	
	// Insert node
	void insertNode(int &node_index, int &route_index, int &insertion_position);
	
	// Remove node (specific position)
	void removeNode(int &node_index, int &route_index, int &removal_position);
	
	// Remove node (any position)
	void removeNode(int &node_index, int &route_index);
	
	// Remove route
	
	
	

};

#endif // SOL_HPP
