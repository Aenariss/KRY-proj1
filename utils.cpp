#include <fstream>
#include <algorithm>
#include "utils.hpp"

using namespace std;

void exitProgram(size_t errorcode, struct arg_struct *args) {
    delete args;
    exit(errorcode);
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
    transform(result.begin(), result.end(), result.begin(), ::toupper);
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

// Check if a key is valid
void a_keyValidity(struct arg_struct *args) {
    vector<int> coPrimes = {1, 3, 5, 7, 9, 11, 15, 17, 19, 21, 23, 25}; // coPrime numbers with 26
    // check a-key validity
    if (std::find(coPrimes.begin(), coPrimes.end(), args->a_key) == coPrimes.end()) {
        fprintf(stderr, "Given `A`-key (first part of the key) has invalid value! Exiting...\n");
        exitProgram(ERROR, args);
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
