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
	
	// Quantity of nodes at each route (this way, no ".size()" operation is needed)
	std::vector<int> RSize {};
	
	// Occurances of client (in N) in solution - This is used for removal operations, so solution doesn't need to be iterated for nodes to be removed
	
	// The first key is the route index, and the second key is the position of node in route
	std::vector<std::vector<std::pair<int, int>>> nodesPositions {};
	
	// Current amount of goods in each node
	std::vector<double> G {};
	
	// Relative met demand for each client
	std::vector<double> Z {};
	
	// Amount delivered for each visit
	std::vector<std::vector<double>> z {};
	
	// Total lenght (in terms of time) of each route
	std::vector<double> W {};
	
	// Initialization by passing instance object
	Sol(Instance &inst_val, double &p, double &Gamma1, double &Gamma2);
	
	~Sol();
	
	//// Methods
	
	// Printing solution
	void printSol();
	
	// Exporting solution
	void toTXT(std::string &file_name);
	
	// FO solution
	void objectiveFunction();
	
	// Insert node
	void insertNode(int &node_index, int &route_index, int &insertion_position);
	
	// Remove node (specific route and position)
	void removeNodeAt(int &route_index, int &removal_index);
	
	// Remove one case of node (at any random position)
	void removeNodeCase(int &node_index);
	
	// Remove all node cases in S
	void removeNodes(int &node_index);
	
	// Remove route
	
	
	

};

#endif // SOL_HPP
