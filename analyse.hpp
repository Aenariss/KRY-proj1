#ifndef ANALYSE_H
#define ANALYSE_H

#include "argParse.hpp"
#include "utils.hpp"

#include <utility>
#include <vector>
#include <map>

bool isInMap(std::map<char, int> haystack, char needle);
std::vector<std::pair<char, double>> frequencyAnalysis(std::string line);
bool sortFunction(std::pair<char, double> a, std::pair<char, double> b);
int getMatches(std::vector<char> most_commons, std::vector<char> my_commons, int a, int b);
std::pair<int, int> findKey(std::vector<std::pair<char, double>> frequencies, std::vector<std::pair<char, double>> alph_freq, struct arg_struct *args);
void analyseMessage(struct arg_struct *args);

#endif