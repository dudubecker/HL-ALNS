#ifndef HEURISTIC_HPP
#define HEURISTIC_HPP

#include <iostream>
#include <memory>
#include "Sol.hpp"

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


    // Método "apply" virtual
    virtual Sol apply(Sol &S);

    // Método "specificApply" puramente virtual
    virtual int chooseNumberofNodes(Sol &S) = 0;
	
	virtual Sol specificApply(Sol &S)  = 0;

    virtual ~Heuristic() {} // Adicionamos um destrutor virtual

protected:
    // Construtor protegido para evitar instanciar a classe Heuristic diretamente
    Heuristic() {}
};

class RemovalHeuristic : public Heuristic {
public:
    // Sobrescrita do método "specificApply" para a RemovalHeuristic
    int chooseNumberofNodes(Sol &S) override;
};

class PartialRandomRemoval : public RemovalHeuristic {
public:
    // Sobrescrita do método "specificApply" para a PartialRandomRemoval
    Sol specificApply(Sol &S) override;
};




/*
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
	
	// Constructor/Destructor
	
	Heuristic();
	
	~Heuristic();
	
	// apply method - all child classes have this
	virtual void apply();

// protected:
    
};

class RemovalHeuristic : public Heuristic {
	

public:

    RemovalHeuristic();

    // Sobrescrita do método "apply" para o comportamento 1
    void apply() override;
};

class InsertionHeuristic : public Heuristic {
public:
    InsertionHeuristic();

    // Sobrescrita do método "apply" para o comportamento 2
    void apply() override;
};

class PartialRandomRemoval : public RemovalHeuristic {
	

public:
    PartialRandomRemoval();

    // Sobrescrita do método "apply" para o comportamento 1
    void apply() override;
};
*/


#endif // HEURISTIC_HPP
