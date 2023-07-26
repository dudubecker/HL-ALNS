// Heuristic.cpp 

#include "Heuristic.hpp"
#include <iostream>

// Implementação do método "apply" da classe base "Heuristic"
void Heuristic::apply() {
    std::cout << "Comportamento base de Heuristic\n";
    // Coloque aqui o código base que deve ser comum a todas as classes derivadas

    // Chamada para o método "specificApply" específico da classe derivada
    // Isso garantirá que o comportamento específico da classe derivada seja chamado após o código base.
    specificApply();
}

// Implementação dos construtores das classes derivadas
Heuristic::Heuristic(const std::string& name) : name(name) {}
RemovalHeuristic::RemovalHeuristic(const std::string& name) : Heuristic(name) {}
PartialRandomRemoval::PartialRandomRemoval(const std::string& name) : RemovalHeuristic(name) {}

// Sobrescrita do método "specificApply" para a RemovalHeuristic
void RemovalHeuristic::specificApply() {
    std::cout << "Executando comportamento da RemovalHeuristic...\n";
    // Coloque aqui o código específico para a RemovalHeuristic
}

// Sobrescrita do método "specificApply" para a PartialRandomRemoval
void PartialRandomRemoval::specificApply() {
    // Chamada para o método "specificApply" da classe base "RemovalHeuristic"
    RemovalHeuristic::specificApply();

    std::cout << "Executando comportamento da PartialRandomRemoval...\n";
    // Coloque aqui o código específico para a PartialRandomRemoval
}




/*
// Apply - Here goes the code that will be firstly executed before other heuristic types calls
void Heuristic::apply(){
	
    std::cout << "Comportamento base de Heuristic\n";
    // Coloque aqui o código base que deve ser comum a todas as classes derivadas
    // Por exemplo, alguma inicialização ou lógica comum antes de executar os comportamentos específicos.
}

// Constructors implementation

Heuristic::Heuristic(){};
Heuristic::~Heuristic(){};

RemovalHeuristic::RemovalHeuristic() : Heuristic() {
	
}

InsertionHeuristic::InsertionHeuristic() : Heuristic() {
	
}

PartialRandomRemoval::PartialRandomRemoval() : RemovalHeuristic() {
	
}

// Sobrescrita do método "apply" para o comportamento 1
void RemovalHeuristic::apply() {
    
	// Coloque aqui o código específico para o comportamento 1
	
	std::cout << "Executando comportamento 1...\n";
	
	
}

// Sobrescrita do método "apply" para o comportamento 2
void InsertionHeuristic::apply() {
    std::cout << "Executando comportamento 2...\n";
    // Coloque aqui o código específico para o comportamento 2
}

// Sobrescrita do método "apply" para o comportamento 3
void PartialRandomRemoval::apply() {
    std::cout << "Executando comportamento 3...\n";
    // Coloque aqui o código específico para o comportamento 3
}
*/
