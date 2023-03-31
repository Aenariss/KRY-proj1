#include "encrypt.hpp"
#include "utils.hpp"
#include <algorithm>
#include <vector>

using namespace std;

// encrypt each letter in string
string encryptOneByOne(string line, struct arg_struct *args) {
    transform(line.begin(), line.end(), line.begin(), ::toupper);
    string encrypted = "";
    // A-Z = 65-90
    for (auto c : line) {
        // capital
        if (c >= 65 && c <= 90) {
            c -= 65;
            int result = (args->a_key * c + args->b_key) % 26;
            encrypted += result + 65;
        }
        // else i dont know what it is and dont do anything
        else {
            encrypted += c;
        }
    }
    return encrypted;
}

// encrypt the message
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
