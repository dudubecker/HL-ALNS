// Heuristic.hpp

#ifndef HEURISTIC_HPP
#define HEURISTIC_HPP

#include <string>

class Heuristic {
public:
    Heuristic(const std::string& name);

    // Método "apply" virtual
    virtual void apply();

    // Método "specificApply" puramente virtual
    virtual void specificApply() = 0;

protected:
    std::string name;
};

class RemovalHeuristic : public Heuristic {
public:
    RemovalHeuristic(const std::string& name);

    // Sobrescrita do método "specificApply" para a RemovalHeuristic
    void specificApply() override;
};

class PartialRandomRemoval : public RemovalHeuristic {
public:
    PartialRandomRemoval(const std::string& name);

    // Sobrescrita do método "specificApply" para a PartialRandomRemoval
    void specificApply() override;
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
