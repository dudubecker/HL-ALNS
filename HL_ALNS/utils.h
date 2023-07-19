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

void printDouble(std::vector<double> &input);

void printInt(std::vector<int> &input);


#endif