#ifndef HEURISTIC_HPP
#define HEURISTIC_HPP

#include <string>

class Heuristic {
public:
    Heuristic(const std::string& name);

    // Método "apply" virtual, que pode ser sobrescrito pelas classes derivadas
    virtual void apply();

protected:
    std::string name;
};

class HeuristicBehavior1 : public Heuristic {
public:
    HeuristicBehavior1(const std::string& name);

    // Sobrescrita do método "apply" para o comportamento 1
    void apply() override;
};

class HeuristicBehavior2 : public Heuristic {
public:
    HeuristicBehavior2(const std::string& name);

    // Sobrescrita do método "apply" para o comportamento 2
    void apply() override;
};

#endif // HEURISTIC_HPP
