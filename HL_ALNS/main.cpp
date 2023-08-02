#include <iostream>
#include "Instance.hpp"
#include "Sol.hpp"
#include "Heuristic.hpp"
#include <iterator>

using namespace std;

int main(){
	
	
	//// Problem data
	
	std::string file_name = "INST_JO_NA_9_cpp.txt";
	
	int number_of_periods = 1;
	
	// Randomness parameter
	double p {4};
	
	// Proximity weight
	double Gamma1 {0.6};
	
	// Met demand weight
	double Gamma2 {0.4};
	
	Instance inst(file_name, number_of_periods);
	
	Sol S(inst, p, Gamma1, Gamma2);
	
	// std::cout << S.inst.t.at(1).at(3).at(0) << std::endl;
	
	// std::cout << S.inst.t.at(3).at(2).at(0) << std::endl;
	
	// std::cout << S.inst.t.at(2).at(2).at(0) << std::endl;
	
	
	S.printSol();
	
	// S.printSol();
	
	// printDouble(S.W);
	
	BasicGreedyInsertion bgi {};
	
	bgi.apply(S);
	
	S.printSol();
	
	PartialRandomRemoval prr {};
	
	prr.apply(S);
	
	S.printSol();
	
	bgi.apply(S);
	
	S.printSol();
	
	printDouble(S.W);
	
	// Epsilon
	
	/*
	double Z = 0;
	
	for (int i; i < S.inst.m; i++){
	
		for (auto value: S.z.at(i)){
			
			if (value < 0){
				
				Z += std::abs(value);
				
			}
		}
	}
	
	double D = 0;
	
	for (auto node: S.inst.D){
		
		D += std::abs(S.inst.d.at(node));
		
	}
	
	for (auto node: S.inst.D){
		
		double z_i = S.G.at(node);
		
		double d_i = std::abs(S.inst.d.at(node));
		
		double epsilon = std::abs((z_i/Z) - (d_i/D));
		
		std::cout << "Epsilon node " << node << ": " << epsilon*100 << "%" << std::endl;
		
		
	}
	
	// printDouble(S.W);
	
	// ConcentricRemoval cr(200);
	
	// cr.apply(S);
	
	// S.printSol();
	
	// printDouble(S.W);
	
	// bgi.apply(S);
	
	// S.printSol();
	
	// printDouble(S.W);
	
	*/
	
	
	
	return 0;
	
}