#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <string>
#include "argParse.hpp"

void encryptMessage(struct arg_struct *args);
std::string encryptOneByOne(std::string line, struct arg_struct *args);
void a_keyValidity(struct arg_struct *args);

#endif