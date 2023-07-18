#include <iostream>
#include "Instance.hpp"
#include "Sol.hpp"

using namespace std;

void print(std::vector<double> &input)
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
	
	
	
	
}