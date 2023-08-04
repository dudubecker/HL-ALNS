#ifndef UTIL_H  
#define UTIL_H


#include <iostream>
#include <algorithm>
#include <vector>
#include <map>



// Comparator function to sort pairs
// according to second value
bool cmp(std::pair<int, double>& a,
        std::pair<int, double>& b);
 
// Function to sort the map according
// to value in a (key-value) pairs
std::vector<int> sortIndexes(std::vector<double> &input_vector);

// Prints vectors:

void printDouble(std::vector<double> &input);

void printInt(std::vector<int> &input);

double roundToDecimalPlaces(double &value, int decimalPlaces);

// Increment in costs through insertion of node_index at route_index at insertion_index
// double deltaCostsInsertion(Sol &S, int &node_index, int &route_index, int &insertion_index);

// Increment in costs through insertion of arc pickup-delivery at route_index at insertion_index
// double deltaCostsInsertion(Sol &S, int &pickup_node_index , int &delivery_node_index , int &route_index, int &insertion_index);

// Increment in costs through insertion of node_index at route_index at insertion_index
// double deltaCostsRemoval(Sol &S, int &node_index, int &route_index, int &removal_index);

#endif