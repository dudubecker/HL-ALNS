#include "Instance.hpp"
#include <numeric>


// Auxiliar functions for initialization

// Reading vehicles data (capacity, velocity and price per distance)
void vehicleDataReader(std::vector<std::vector<double>>  &input_vector, std::string &instance)
{
	
	std::string data_string = instance.substr(0, instance.find('\n')) + ',';
	
	// Iterating for each depot
	
	size_t pos_depot = 0;
	
	std::string data_per_depot_string;
	
	while ((pos_depot = data_string.find(',')) != std::string::npos){
		
		data_per_depot_string = data_string.substr(0, pos_depot) + ' ';
		
		// Iterating for each parameter value
		size_t pos_parameter = 0;
		
		std::string parameter_value;
		
		std::vector<double> parameter_value_vector;
		
		while ((pos_parameter = data_per_depot_string.find(' ')) != std::string::npos) {
			
			parameter_value = data_per_depot_string.substr(0, pos_parameter);
			
			// Checking if capacity is valid, by computing its length of characters
			
			if (parameter_value.length() > 0){
				
				parameter_value_vector.push_back(std::stoi(parameter_value));
				
			}
			
			data_per_depot_string.erase(0, pos_parameter + 1);
		}
		
		input_vector.push_back(parameter_value_vector);
		
		data_string.erase(0, pos_depot + 1);
	}
	
	instance.erase(0, instance.find('\n') + 1);
	
}


// Reducing dimension of vehicle data vectors (2D to 1D)

std::vector<double> reduceDimension(std::vector<std::vector<double>> &input){
	
	std::vector<double> return_vector {};
	
	for (unsigned i = 0; i < input.size(); i++){
		
		for (auto j: input.at(i)){
			
			return_vector.push_back(j);
		}
		
	}
		return return_vector;
	
}

// Raw initialization

Instance::Instance()
{
}


// Initialization with file and number of periods

Instance::Instance(std::string &file_name, int &number_of_periods)
{
	T = number_of_periods;
	
	std::string instance;
	
	std::fstream txt_file;
	txt_file.open(file_name, std::ios::in);
	
	if (txt_file.is_open()){
		
		std::string line;
		
		while (getline(txt_file, line)){
			
			instance.append(line + '\n');
			
		}
		
		txt_file.close();
		
	} else {
		
		std::cout << "No file with the name " << file_name << " was found." << std::endl;
		
	}
	
	
	//// First parameters of instance
	
	// Number of delivery requests
	n = std::stoi(instance.substr(0, instance.find('\n')));
	
	instance.erase(0, instance.find('\n') + 1);
	
	// Number of vehicles
	m = stoi(instance.substr(0, instance.find('\n')));
	
	instance.erase(0, instance.find('\n') + 1);
	
	
	//// Vehicle parameters
	
	// Vehicles capacities
	
	vehicleDataReader(Q_loc, instance);
	
	Q = reduceDimension(Q_loc);
	
	// Vehicles velocities
	std::vector<std::vector<double>> V_loc;
	vehicleDataReader(V_loc, instance);
	
	V = reduceDimension(V_loc);
	
	// Vehicles traveling costs
	std::vector<std::vector<double>> C_loc;
	vehicleDataReader(C_loc, instance);
	
	C = reduceDimension(C_loc);
	
	//// Other parameters
	
	while (instance.length() > 0){
		
		std::string line_string =  instance.substr(0, instance.find('\n')) + " ";
		
		std::string county_string = line_string.substr(0, instance.find('.') - 2);
		
		counties.push_back(county_string);
		
		line_string.erase(0, county_string.length() + 1);
		
		
		// Integer for appending right data to each vector
		
		int counter {};
		
		while (line_string.length() > 0){
			
		 	std::string data_string = line_string.substr(0, line_string.find(" "));
			
			switch (counter){
				
				case 0:
				
				s.push_back(stod(data_string));
				
				break;
				
				case 1:
				
				d.push_back(stod(data_string));
				
				break;
				
				case 2:
				
				w_a.push_back(stod(data_string));
				
				break;
				
				case 3:
				
				w_b.push_back(stod(data_string));
				
				break;
				
			}
			
			counter += 1;
			
		 	line_string.erase(0, line_string.find(" ") + 1);
			
		}
		
		instance.erase(0, instance.find('\n') + 1);
		
	}
	
	
	//// Distances
	
	// Map for distances
	std::map<std::string, std::map<std::string, double>> distances_data {};
	
	std::vector<std::string> destination_counties {};
	
	
	std::ifstream in_file;
	in_file.open("DistancesCpp.csv");
	
	if (in_file.is_open()) {
		;
	}
	else{
		
		std::cout << "CSV error: no file was found." << std::endl;
	}
	
	// Getting first line with data (destination counties)
	
	std::string first_line;
	
	getline(in_file, first_line, '\n');
	
	first_line = first_line.substr(first_line.find(',') + 1, first_line.length()) + ",";
	
	while (first_line.length() > 0){
		
		std::string county_name = first_line.substr(0, first_line.find(","));
		
		destination_counties.push_back(county_name);
		
		first_line.erase(0, first_line.find(",") + 1);
		
	}
	
	// Getting rest of the data (origin counties and distances)
	
	// Stop criteria
	std::string last_county = destination_counties.at(destination_counties.size() - 1);
	
	std::string line_data;
	std::string origin_county;
	
	while (origin_county != last_county) {
		
		getline(in_file, origin_county, ',');
		getline(in_file, line_data, '\n');
		
		line_data += ",";
		
		int county_index = 0;
		
		while (line_data.length() > 0){
			
			double distance = stod(line_data.substr(0, line_data.find(",")));
			
			std::string destination_county = destination_counties.at(county_index);
			
			distances_data[origin_county].insert({destination_county, distance});
			
			line_data.erase(0, line_data.find(",") + 1);
			
			county_index += 1;
		
		}
		
	}
	
	
	// std::cout << distances_data["JOAO PESSOA"]["NATAL"] << std::endl;
	
	in_file.close();
	
	// Finally, using distances for calculating travel times and costs
	
	// Travel times
	
	t = std::vector<std::vector<std::vector<double>>> (counties.size(), std::vector<std::vector<double>>(counties.size(), std::vector<double>(m)));
	
	// Travel costs
	
	c = std::vector<std::vector<std::vector<double>>> (counties.size(), std::vector<std::vector<double>>(counties.size(), std::vector<double>(m)));
	
	
	for (unsigned i = 0; i < counties.size(); i++){
		
		for (unsigned j = 0; j < counties.size(); j++){
			
			for (auto k = 0; k < m; k++){
				
				double distance = distances_data[counties.at(i)][counties.at(j)];
				
				// if (i != j){
					
				// 	std::cout << distance << std::endl;
					
				// }
				
				double time = distance/V[k] + s[j];
				
				double costs = distance*C[k];
				
				t.at(i).at(j).at(k) = time;
				c.at(i).at(j).at(k) = costs;
				
			}
			
		}
		
	}
	
	// Sets of the problem - initial nodes, pickups, deliveries and final nodes
	
	// Getting information of node type by demands (d) data
	
	int node_index = 0;
	
	
	// Set S_0
	while (d.at(node_index) == 0){
		
		S_0.push_back(node_index);
		
		node_index += 1;
		
	}
	
	// Set P
	while (d.at(node_index) > 0){
		
		P.push_back(node_index);
		
		node_index += 1;
		
	}
	
	// Set D
	while (d.at(node_index) < 0){
		
		D.push_back(node_index);
		
		node_index += 1;
		
	}
	
	// Set S_f
	for (unsigned i = node_index; i < counties.size(); i++){
		
		S_f.push_back(i);
		
	}
	
	// Set N
	
	N =  std::vector<int>(counties.size());
	
	std::iota(N.begin(), N.end(), 0);
	
	
	
	
	// Creating ordered lists
	for (auto i = 0; i < d.size(); i++){
		
		std::vector<double> traveling_times {};
		
		for (auto j = 0; j < d.size(); j++){
			
			traveling_times.push_back(t.at(i).at(j).at(0));
		
		}
		
		std::vector<int> sorted_traveling_times = sortIndexes(traveling_times);
		
		// If its a pickup or delivery, ordered map is updated
		if (d.at(i) != 0){
			
			proximitiesPUD[i] = sorted_traveling_times;
			
		}
		
		
		proximitiesN.push_back(sorted_traveling_times);
		
		
	}
	
	// Excluding non pickup or delivery nodes
	
	
	
	
	
	
	
	
	
}

Instance::~Instance()
{
}