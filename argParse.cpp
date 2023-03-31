#include <getopt.h>
#include "argParse.hpp"
#include "utils.hpp"

using namespace std;

struct arg_struct *parseArgs(int argc, char* argv[]) {
    struct arg_struct *args = new struct arg_struct;
    int c;
    while((c = getopt(argc, argv, ":o:f:a:b:edc")) != -1) {
        switch (c) {
            case ':': // missing req. argument handling
                switch (optopt) { // which argument is missing
                    default:
                        fprintf(stderr, "Missing required argument near the -%c parameter! Exiting...\n", optopt);
                        exitProgram(ERROR, args);
                        break;
                }
            case 'd':
                args->decrypt = true;
                break;
            case 'c':
                args->analyse = true;
                break;
            case 'e':
                args->encrypt = true;
                break;
            case 'a':
                args->a_key = stoi(optarg);
                break;
            case 'b':
                args->b_key = stoi(optarg);
                break;
            case 'f':
                args->in_file = optarg;
                args->in_flag = true;
                break;
            case 'o':
                args->out_file = optarg;
                break;
            case '?':
            default:
                fprintf(stderr, "Unknown argument -%c! Exiting...\n", optopt);
                exitProgram(ERROR, args);
                break;

        }
    }
    args->argv = argv;
    args->argc = argc;
    args->optind = optind;

    argValidity(args);
    return args;
}

void argValidity(struct arg_struct *args) {

    if (args->argc-args->optind == 1 && args->in_file == "") { // exactly 1 argument
        args->in_file = args->argv[args->argc-1];
    }
    else if (args->argc-args->optind == 1 && args->in_file != "") {
        fprintf(stderr, "Invalid usage of arguments! How am I supposed to know what the input is? Exiting...\n");
        exitProgram(ERROR, args);
    }
    else if (args->argc-args->optind >= 1) {
        fprintf(stderr, "Invalid usage of arguments! How am I supposed to know what the input is? Exiting...\n");
        exitProgram(ERROR, args);
    }

    if (args->in_file == "") { // 
        fprintf(stderr, "Invalid usage of arguments! When doing anything, you must give something to analyse using an argument or -f! Exiting...\n");
        exitProgram(ERROR, args);
    }

    if ((args->a_key == -99998 || args->b_key == -99998) && (args->encrypt || args->decrypt)) {
        fprintf(stderr, "Invalid usage of arguments! When encrypting or decrypting, you must give 2 keys! Exiting...\n");
        exitProgram(ERROR, args);
    }

    if ((args->encrypt && args->decrypt) || (args->encrypt && args->analyse) || (args->analyse && args->decrypt) || (args->encrypt == false && args->decrypt == false && args->analyse == false)) {
        fprintf(stderr, "Invalid usage of arguments! Must do at least encrypting, decrypting or analysing, but not 2 or more at the same time! Exiting...\n");
        exitProgram(ERROR, args);
    }
}
