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
	
	// Variation of insertion/removal of a node, in terms of time or costs
	double deltaInsertion(std::string delta_type, Sol &S, int &node_index, int &route_index, int &insertion_index);
	
	double deltaRemoval(std::string delta_type, Sol &S, int &node_index, int &route_index, int &removal_index);
	
	double deltaEpsilon(Sol &S, int &source_node_index, int &receiver_node_index, int &route_index, int &position_index);
	
	
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
    // Sobrescrita do método "specificApply" para a PartialRandomRemoval
    Sol specificApply(Sol &S) override;
};




#endif // HEURISTIC_HPP
