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
	
	// Number of periods
	int T {};
	
	// Number of delivery requests
	int n {};
	
	// Number of vehicles
	int m {};
	
	// Counties
	std::vector<std::string> counties {};
	
	// Initial nodes (set S_0)
	std::vector<int> S_0 {};
	
	// Pickup nodes (set P)
	std::vector<int> P {};
	
	// Delivery nodes (set D)
	std::vector<int> D {};
	
	// Final nodes (set S_f)
	std::vector<int> S_f {};
	
	// Service times
	std::vector<double> s {};
	
	// Demands
	std::vector<double> d {};
	
	// Opening time windows
	std::vector<double> w_a {};
	
	// Closing time windows
	std::vector<double> w_b {};
	
	// Vehicles locations (in terms of capacities)
	std::vector<std::vector<double>> Q_loc;
	
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
	Instance(std::string &file_name, int &number_of_periods);
	Instance();
	~Instance();
	
	
};

#endif // INSTANCE_HPP
