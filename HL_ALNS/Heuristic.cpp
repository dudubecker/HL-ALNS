#include "Heuristic.hpp"

#include <iostream>

// Implementação do método "apply" da classe base "Heuristic"
void Heuristic::apply() {
    std::cout << "Comportamento base de Heuristic\n";
    // Coloque aqui o código base que deve ser comum a todas as classes derivadas
    // Por exemplo, alguma inicialização ou lógica comum antes de executar os comportamentos específicos.
}

// Implementação dos construtores das classes derivadas
Heuristic::Heuristic(const std::string& name) : name(name) {}
HeuristicBehavior1::HeuristicBehavior1(const std::string& name) : Heuristic(name) {}
HeuristicBehavior2::HeuristicBehavior2(const std::string& name) : Heuristic(name) {}

// Sobrescrita do método "apply" para o comportamento 1
void HeuristicBehavior1::apply() {
    std::cout << "Executando comportamento 1...\n";
    // Coloque aqui o código específico para o comportamento 1
}

// Sobrescrita do método "apply" para o comportamento 2
void HeuristicBehavior2::apply() {
    std::cout << "Executando comportamento 2...\n";
    // Coloque aqui o código específico para o comportamento 2
}

