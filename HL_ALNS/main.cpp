#include <iostream>
#include "Instance.hpp"
#include "Sol.hpp"
#include "Heuristic.hpp"
#include <iterator>

using namespace std;

int main(){
	
	
	//// Problem data
	
	std::string file_name = "INST_MA_RE_20_cpp.txt";
	
	int number_of_periods = 7;
	
	// Randomness parameter
	double p {4};
	
	// Proximity weight
	double Gamma1 {0.3};
	
	// Met demand weight
	double Gamma2 {0.7};
	
	Instance inst(file_name, number_of_periods);
	
	Sol S(inst, p, Gamma1, Gamma2);
	
	S.printSol();
	
	// S.printSol();
	
	// printDouble(S.W);
	
	BasicGreedyInsertion bgi {};
	
	bgi.apply(S);
	
	// PartialRandomRemoval prr {};
	
	// prr.apply(S);
	
	// S.printSol();
	
	// bgi.apply(S);
	
	// S.printSol();
	
	// std::cout << "\n";
	
	
	//int source_node_index = 9;
	//int receiver_node_index = 8;
	
	//int route_index = 1;
	
	//int position_index = 4;
	
	// bgi.deltaEpsilon(S, source_node_index, receiver_node_index, route_index, position_index);;
	
	
	// Epsilon
	
	std::cout << "\n\n";
	
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
	
	std::cout << "\n\n";
	
	S.printSol();
	
	// S.updateEpsilon();
	
	printDouble(S.epsilon);
	
	printDouble(S.G);
	
	printDouble(S.inst.d);
	
	std::cout << S.totalZ << std::endl;
	
	std::cout << S.totalD << std::endl;
	
	// printDouble(S.W);
	
	// ConcentricRemoval cr(200);
	
	// cr.apply(S);
	
	// S.printSol();
	
	// printDouble(S.W);
	
	// bgi.apply(S);
	
	// S.printSol();
	
	// printDouble(S.W);
	
	
	
	return 0;
	
}