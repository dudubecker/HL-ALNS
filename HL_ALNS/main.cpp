#include <iostream>
#include "Instance.hpp"
#include "Sol.hpp"

using namespace std;

void printDouble(std::vector<double> &input)
{
    for (auto const &i: input) {
        std::cout << i << " ";
    }
	
	std::cout << std::endl;
	
}

void printInt(std::vector<int> &input)
{
    for (auto const &i: input) {
        std::cout << i << " ";
    }
	
	std::cout << std::endl;
	
}



int main(){
	
	//// Problem data
	
	std::string file_name = "INST_JO_NA_9_cpp.txt";
	
	int number_of_periods = 1;
	
	Instance inst(file_name, number_of_periods);
	
	Sol S(inst);
	
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
	
	
	return 0;
	
}