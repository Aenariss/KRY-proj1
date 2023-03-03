#include <iostream>
#include <fstream>
#include <vector>
#include <getopt.h>
#include <math.h>
#include <algorithm>
#include "kry.hpp"

using namespace std;

void exitProgram(size_t errorcode, struct arg_struct *args) {
    delete args;
    exit(errorcode);
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

vector<string> readFile(struct arg_struct *args) {
    vector<string> content;
    string tmp;
    ifstream FileRead(args->in_file);
    if (FileRead.fail()) {
        fprintf(stderr, "Couldn't open the input file! Exiting...\n");
        exitProgram(ERROR, args);
    }
    while (getline(FileRead, tmp)) {
        content.push_back(tmp);
    }
    FileRead.close();
    return content;
}

void writeResult(struct arg_struct *args, string result) {
    if (args->out_file != "") {
        ofstream outFile;
        outFile.open(args->out_file, ios_base::app);
        outFile << result << endl;
        outFile.close();
    }
    else {
        cout << result << endl;
    }
}

string encryptOneByOne(string line, struct arg_struct *args) {
    string encrypted = "";
    // A-Z = 65-90
    // a-z = 97-122
    for (auto c : line) {
        // capital
        if (c >= 65 && c <= 90) {
            c -= 65;
            int result = (args->a_key * c + args->b_key) % 26;
            encrypted += result + 65;
        }
        // lowercase
        else if (c >= 97 && c <= 122) {
            c -= 97;
            char result = (args->a_key * c + args->b_key) % 26;
            encrypted += result + 97;
        }
        // else i dont know what it is and dont do anything
        else {
            encrypted += c;
        }
    }
    return encrypted;
}

void a_keyValidity(struct arg_struct *args) {
    vector<int> coPrimes = {1, 3, 5, 7, 9, 11, 15, 17, 19, 21, 23, 25}; // coPrime numbers with 26
    // check a-key validity
    if (std::find(coPrimes.begin(), coPrimes.end(), args->a_key) == coPrimes.end()) {
        fprintf(stderr, "Given `A`-key (first part of the key) has invalid value! Exiting...\n");
        exitProgram(ERROR, args);
    }
}

void encryptMessage(struct arg_struct *args) {
    // letter = (a * x + b) % 26
    a_keyValidity(args);

    if (args->in_flag) {
        vector<string> content = readFile(args);
        for (string line : content) {
            string encrypted = encryptOneByOne(line, args);
            writeResult(args, encrypted);
        }
    }
    else {
        string encrypted = encryptOneByOne(args->in_file, args);
        writeResult(args, encrypted);
    }
}

int modularMultiplicativeInverse(int base) {
    // 1 = a*a^-1 mod m where a = a_key, m = 26
    // a^-1 wont be higher than the number (... tried them all)
    for (int a = 1; a < 26; a++) {
        if ((base * a) % 26 == 1) {
            return a;
        }
    }
    return 0;
}

string decryptOneByOne(string line, struct arg_struct *args) {
    string decrypted = "";
    // A-Z = 65-90
    // a-z = 97-122
    auto modular = modularMultiplicativeInverse(args->a_key);
    for (auto c : line) {
        // capital
        if (c >= 65 && c <= 90) {
            c -= 65;
            int result = (26 + ((modular * (c - args->b_key)) % 26)) % 26; // c++ defaults eg. -27 % 26 to -1 but I want 25, so I do 26 + (-27 % 26) % 26 
            decrypted += result + 65;
        }
        // lowercase
        else if (c >= 97 && c <= 122) {
            c -= 97;
            int result = (26 + ((modular * (c - args->b_key)) % 26)) % 26;
            decrypted += result + 97;
        }
        // else i dont know what it is and dont do anything
        else {
            decrypted += c;
        }
    }
    return decrypted;
}

void decryptMessage(struct arg_struct *args) {
    a_keyValidity(args);
    if (args->in_flag) {
        vector<string> content = readFile(args);
        for (string line : content) {
            string decrypted = decryptOneByOne(line, args);
            writeResult(args, decrypted);
        }
    }
    else {
        string decrypted = decryptOneByOne(args->in_file, args);
        writeResult(args, decrypted);
    }

}

void frequencyAnalysis(struct arg_struct *args) {

    if (args->in_flag) {
        vector<string> content = readFile(args);
        for (string line : content) {
            string decrypted = decryptOneByOne(line, args);
            writeResult(args, decrypted);
        }
    }
    else {
        string decrypted = decryptOneByOne(args->in_file, args);
        writeResult(args, decrypted);
    }
}

int main(int argc, char* argv[]) {
    struct arg_struct *args = parseArgs(argc, argv);

    if (args->encrypt) {
        encryptMessage(args);
    }
    else if (args->decrypt) {
        decryptMessage(args);
    }
    else if (args->analyse) {
        frequencyAnalysis(args);
    }

    delete args;
    return (SUCCESS);
}
