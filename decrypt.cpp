#include "decrypt.hpp"
#include <string>
#include <algorithm>

using namespace std;

// decrypt one by one
string decryptOneByOne(string line, struct arg_struct *args) {
    transform(line.begin(), line.end(), line.begin(), ::toupper);
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
        // else i dont know what it is and dont do anything
        else {
            decrypted += c;
        }
    }
    return decrypted;
}

// decrypt whole string
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