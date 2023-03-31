#ifndef DECRYPT_H
#define DECRYPT_H

#include <string>
#include "argParse.hpp"
#include "utils.hpp"

std::string decryptOneByOne(std::string line, struct arg_struct *args);
void decryptMessage(struct arg_struct *args);

#endif