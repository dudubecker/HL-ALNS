#include <iostream>
#include "Instance.hpp"
#include "Sol.hpp"

using namespace std;

int main(){
	
	//// Problem data
	
	std::string file_name = "INST_BE_CA_CA_CO_CR_FO_HE_IM_IR_JO_MA_MA_NA_RE_RO_RO_TE_209_cpp.txt";
	
	int number_of_periods = 7;
	
	Instance inst(file_name, number_of_periods);
	
	Sol S(inst);
	
	S.printSol();
	
	// S.toTXT(file_name);
	
	
	
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