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
	
	// Occurrences of client (in N) in solution - This is used for removal operations, so solution doesn't need to be iterated for nodes to be removed
	
	// The first key is the route index, and the second key is the position of node in route
	// std::vector<std::vector<std::pair<int, int>>> nodesPositions {};
	// std::vector<std::vector<std::vector<int>>> nodesPositions {};
	
	// Current amount of goods in each node
	std::vector<double> G {};
	
	// Relative met demand for each client
	std::vector<double> Z {};
	
	// Epsilon operation for each client (9999 for depots/pickups)
	std::vector<double> epsilon {};
	
	// Amount delivered for each visit
	std::vector<std::vector<double>> z {};
	
	// Total lenght (in terms of time) of each route
	std::vector<double> W {};
	
	// Total met demand in by solution
	double totalZ {};
	
	// Total demand in by solution
	double totalD {};
	
	// Clients with unmet demand
	std::vector<int> unmet_demand_clients {};
	
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
	
	// Remove random node and position
	void removeRandomNode();
	
	// Remove all node cases in S
	void removeNodeCases(int &node_index);
	
	// Replace node at specific route and position, used in insertion methods
	void replaceNodeAt(int node_index, int route_index, int insertion_index);
	
	// Insert node (specific route, position and demand)
	// Obs -> "demand" argument is always positive! (This needs to be established)
	void insertNodeAt(int &node_index, int &route_index, int &insertion_index, double &demand);
	
	// Insert P-D arc (specific route, position and demand)
	void insertArcAt(int pickup_node_index, int delivery_node_index, int route_index, int insertion_index, double demand);
	
	// Split insertion -> splits demand from "source_node" to "receiver_node", inserting in right positions
	void splitInsertion(std::string how ,int node_index, int route_index, int insertion_index, double demand);
	
	
	
	// Returns true if solution contains all nodes in vector and false otherwise
	bool containsAll(std::vector<int> &nodes_vector);
	
	// Returns true if solution contains at least one node in vector and false otherwise
	bool containsAny(std::vector<int> &nodes_vector);
	
	// Updates epsilon values
	void updateEpsilon();
	
	// Rebalances pickup visits with all possibly picked up capacity
	void tidyUp();
	

};

#endif // SOL_HPP
