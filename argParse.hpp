#ifndef ARGPARSE_H
#define ARGPARSE_H

#include <string>

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

struct arg_struct *parseArgs(int argc, char* argv[]);
void argValidity(struct arg_struct *args);

#endif