#include <iostream>
#include "Instance.hpp"
#include "Sol.hpp"

using namespace std;

int main(){
	
	//// Problem data
	
	std::string file_name = "INST_JO_NA_9_cpp.txt";
	
	int number_of_periods = 3;
	
	Instance inst(file_name, number_of_periods);
	
	Sol S(inst);
	
	cout << "\n\n\n";
	
	printDouble(S.inst.d);
	
	//cout << (S.Z).size() << endl;;
	
	//printInt(S.inst.N);
	
	S.printSol();
	
	printDouble(S.G);
	
	double total_met_demand = {};
	
	for (int i; i < S.inst.m; i++){
		
		printDouble(S.z.at(i));
		
		for (auto value: S.z.at(i)){
			
			if (value > 0){
				
				total_met_demand += value;
				
			}
			
		}
		
		
	}
	
	printDouble(S.Z);
	
	std::cout << total_met_demand << std::endl;
	
	
	//open file for writing
	ofstream fw("CPlusPlusSampleFile.txt", std::ofstream::out);
	fw.close();
	
	/*
	
	cout << "All proximities:\n";
	
	for (auto i=0; i < S.inst.d.size(); i++){
		
		printInt(S.inst.proximitiesN.at(i));
		
	}
	
	cout << "\nProximities of PUD:\n";
	
	for (auto i=0; i < S.inst.d.size(); i++){
		
		if (S.inst.d.at(i) != 0){
			
			printInt(S.inst.proximitiesPUD[i]);
			
		}
		
		
	}
	
	*/
	
	return 0;
	
}