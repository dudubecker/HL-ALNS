#include <iostream>
#include "Instance.hpp"
#include "Sol.hpp"

using namespace std;

int main(){
	
	//// Problem data
	
	std::string file_name = "INST_CR_IM_IR_JO_MA_MA_NA_RE_TE_82_cpp.txt";
	
	int number_of_periods = 2;
	
	Instance inst(file_name, number_of_periods);
	
	Sol S(inst);
	
	S.toTXT(file_name);
	
	
	
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