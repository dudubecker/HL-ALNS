#ifndef HEURISTIC_HPP
#define HEURISTIC_HPP

#include <iostream>
#include <memory>
#include <string>
#include "Sol.hpp"

// Heuristic main object

class Heuristic {
public:
	
	// Attributes - all heuristic objects have these
	
	// Heuristic score, updated during algorithm
	double score {0};
	
	// Heuristic weight, that determines its probability of being chosen
	double weight {0};
	
	// Number of iterations on last segment
	int n_it {0};
	
	// Number of iterations in total
	int n_it_total {0};
	
	// Computational time of heuristic
	double processing_time {};
	
	// Auxiliar methods
	
	// Variation of insertion of a node, in terms of time or costs
	double deltaInsertion(std::string delta_type, Sol &S, int &node_index, int &route_index, int &insertion_index);
	
	// Variation of insertion of an arc, in terms of time, costs or epsilon
	double deltaInsertionArc(std::string delta_type, Sol &S, int &first_arc_node_index, int &second_arc_node_index , int &route_index, int &insertion_index);
	
	// Variation of epsilon by transfering demand from source to receiver nodes
	std::pair<double, double> deltaEpsilonInsertion(Sol &S, int &source_node_index, int &receiver_node_index, int &route_index, int &position_index);
	
	// Variation of epsilon by inserting an arc with "delivery_node_index"
	double deltaEpsilonInsertionArc(Sol &S, int &delivery_node_index, double &demand);
	
	// Variation of removal of a node, in terms of time or costs
	double deltaRemoval(std::string delta_type, Sol &S, int &node_index, int &route_index, int &removal_index);
	
	// Variation of removal of a full segment, in terms of time or costs
	double deltaRemovalSegment(std::string delta_type, Sol &S, int &pickup_node_index, int &route_index, int &removal_index);
	
	// Variation of epsilon by removing a delivery node in positions
	double deltaEpsilonRemoval(Sol &S, int &node_index, int &route_index, int &removal_index);
	
	// Variation of epsilon by removing a full segment, whose pickup node is in "removal_index"
	double deltaEpsilonRemovalSegment(Sol &S, int &pickup_node_index, int &route_index, int &removal_index);
	
	// Variation of replacement of a node, in terms of time or costs
	double deltaReplacement(std::string delta_type, Sol &S, int &node_index, int &route_index, int &position_index);
	
	
	// Main methods
	
	// Main method for applying heuristic
	virtual Sol apply(Sol &S);
	
	// Method for initialization of removal methods
	virtual int initializeMethod(Sol &S) = 0;
	
	// Method for initialization of insertion methods
	virtual void initializeMethod() = 0;
	
	// Specific method for each heuristic
	virtual Sol specificApply(Sol &S)  = 0;
	
	/////
	
	virtual ~Heuristic() {} // Adicionamos um destrutor virtual

protected:
    // Construtor protegido para evitar instanciar a classe Heuristic diretamente
    Heuristic() {}
};

// Removal heuristic object

class RemovalHeuristic : public Heuristic {
public:
    // Sobrescrita do método "specificApply" para a RemovalHeuristic
    int initializeMethod(Sol &S) override;
	
	void initializeMethod() override {};
	
};

// Removal heuristics inherited objects

class PartialRandomRemoval : public RemovalHeuristic {
public:
    // Sobrescrita do método "specificApply" para a PartialRandomRemoval
    Sol specificApply(Sol &S) override;
	
	
};

class WorstRemoval : public RemovalHeuristic {
public:
    
	
	// Attribute - weight for epsilon
	double gamma1 {};
	
	// Attribute - weight for costs
	double gamma2 {};
	
	// Attribute - weight for unmet_demand
	double gamma3 {};
	
	// Constructor:
	WorstRemoval(int epsilon_weight, int costs_weight, int unmet_demand_weight){
		gamma1 = epsilon_weight;
		gamma2 = costs_weight;
		gamma3 = unmet_demand_weight;
	};
	
	
    Sol specificApply(Sol &S) override;
	
	
};

class ConcentricRemoval : public RemovalHeuristic {
public:
	
	// Attribute - radius of removal, in terms of traveling times
	double radius {};
	
	// Constructor:
	ConcentricRemoval(double r){
		radius = r; 
	};
	
	// Sobrescrita do método "specificApply" para a PartialRandomRemoval
	Sol specificApply(Sol &S) override;
};

// Insertion heuristic object


class InsertionHeuristic : public Heuristic {
public:

	
	// Sobrescrita do método "specificApply" para a RemovalHeuristic
    int initializeMethod(Sol &S) override {};
	
	void initializeMethod() override;
};


// Insertion heuristics inherited objects
class BasicGreedyInsertion : public InsertionHeuristic {
public:
	
	// Attribute - weight for epsilon
	double gamma1 {};
	
	// Attribute - weight for costs
	double gamma2 {};
	
	// Attribute - weight for unmet_demand
	double gamma3 {};
	
	// Constructor:
	BasicGreedyInsertion(int epsilon_weight, int costs_weight, int unmet_demand_weight){
		gamma1 = epsilon_weight;
		gamma2 = costs_weight;
		gamma3 = unmet_demand_weight;
	};
	
	
    Sol specificApply(Sol &S) override;
};




#endif // HEURISTIC_HPP
