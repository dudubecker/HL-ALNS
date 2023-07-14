#ifndef INSTANCE_HPP
#define INSTANCE_HPP
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <map>

// using namespace std;

class Instance
{
	
	// Attributes
	public:
	
	// Number of delivery requests
	int n {};
	
	// Number of vehicles
	int m {};
	
	// Counties
	std::vector<std::string> counties {};
	
	// Service times
	std::vector<double> s {};
	
	// Demands
	std::vector<double> d {};
	
	// Opening time windows
	std::vector<double> w_a {};
	
	// Closing time windows
	std::vector<double> w_b {};
	
	// Capacities
	std::vector<double> Q {};
	
	// Velocities
	std::vector<double> V {};
	
	// Vehicles traveling costs
	std::vector<double> C {};
	
	// Traveling times
	std::vector<std::vector<std::vector<double>>> t {};
	
	// Traveling costs
	std::vector<std::vector<std::vector<double>>> c {};
	
	//Constructor/destructor
	Instance();
	~Instance();
	
	// Methods
	void read(std::string &file_name);
};

#endif // INSTANCE_HPP
