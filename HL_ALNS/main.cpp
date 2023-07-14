#include <iostream>
#include "Instance.hpp"

void print(std::vector<double> &input)
{
    for (auto const &i: input) {
        std::cout << i << " ";
    }
	
	std::cout << std::endl;
	
}

int main(){
	
	Instance inst;
	
	std::string file_name = "INST_JO_NA_9_cpp.txt";
	
	inst.read(file_name);
	
	print(inst.w_b);
	
	
}