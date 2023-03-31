#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>
#include <string>
#include "argParse.hpp"

/* Constants */
#define ERROR 1
#define SUCCESS 0

void exitProgram(size_t errorcode, struct arg_struct *args);

std::vector<std::string> readFile(struct arg_struct *args);
void writeResult(struct arg_struct *args, std::string result);

void a_keyValidity(struct arg_struct *args);
int modularMultiplicativeInverse(int base);

#endif