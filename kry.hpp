#ifndef KRY_H
#define KRY_H

#include <getopt.h>
#include <string>
#include <vector>
#include <utility>

/* Argument structure */
struct arg_struct {
    bool encrypt = false;
    bool decrypt = false;
    bool analyse = false;
    bool in_flag = false;
    int a_key = -99998;
    int b_key = -99998;
    int argc;
    char** argv;
    int optind;
    std::string in_file = "";
    std::string out_file = "";
};

/* Alphabet frequency map */
/* https://www.matweb.cz/frekvencni-analyza/ */
std::vector<std::pair<std::string, double>> alphabet_frequency = {
    {"A", 0.09589269},
    {"B", 0.0177612},
    {"C", 0.02999077},
    {"D", 0.03774841},
    {"E", 0.10904081},
    {"F", 0.0017506},
    {"G", 0.00219812},
    {"H", 0.02497482},
    {"I", 0.06686138},
    {"J", 0.02305759},
    {"K", 0.03528082},
    {"L", 0.05720782},
    {"M", 0.0360545},
    {"N", 0.05917244},
    {"O", 0.08029999},
    {"P", 0.03114961},
    {"Q", 0.00005933},
    {"R", 0.04396827},
    {"S", 0.0558597},
    {"T", 0.05385289},
    {"U", 0.03579031},
    {"V", 0.03952464},
    {"W", 0.00054266},
    {"X", 0.0003591},
    {"Y", 0.02857512},
    {"Z", 0.03302643}
};

/* Functions */
void argValidity(struct arg_struct *args);
struct arg_struct *parseArgs(int argc, char* argv[]);
void encryptMessage(struct arg_struct *args);
std::vector<std::string> readFile(struct arg_struct *args);
void writeResult(struct arg_struct *args, std::string result);
std::string encryptOneByOne(std::string line, struct arg_struct *args);
void a_keyValidity(struct arg_struct *args);
void decryptMessage(struct arg_struct *args);
std::string decryptOneByOne(std::string line, struct arg_struct *args);
int modularMultiplicativeInverse(int base);
void frequencyAnalysis(struct arg_struct *args);
int main(int argc, char* argv[]);

/* Constants */
#define ERROR 1
#define SUCCESS 0

#endif
