
#include "encrypt.hpp"
#include "decrypt.hpp"
#include "analyse.hpp"

#include <map>
#include <vector>
#include <algorithm>
#include <utility>

using namespace std;

/* Alphabet frequency map */
/* https://www.matweb.cz/frekvencni-analyza/ */
std::vector<std::pair<char, double>> alphabet_frequency = {
    {'A', 0.09589269},
    {'B', 0.0177612},
    {'C', 0.02999077},
    {'D', 0.03774841},
    {'E', 0.10904081},
    {'F', 0.0017506},
    {'G', 0.00219812},
    {'H', 0.02497482},
    {'I', 0.06686138},
    {'J', 0.02305759},
    {'K', 0.03528082},
    {'L', 0.05720782},
    {'M', 0.0360545},
    {'N', 0.05917244},
    {'O', 0.08029999},
    {'P', 0.03114961},
    {'Q', 0.00005933},
    {'R', 0.04396827},
    {'S', 0.0558597},
    {'T', 0.05385289},
    {'U', 0.03579031},
    {'V', 0.03952464},
    {'W', 0.00054266},
    {'X', 0.0003591},
    {'Y', 0.02857512},
    {'Z', 0.03302643}
};

/* Find if a key is present in std::map */
bool isInMap(map<char, int> haystack, char needle) {
    auto s = haystack.find(needle);
    return (s != haystack.end());
}

vector<pair<char, double>> frequencyAnalysis(string line) {
    transform(line.begin(), line.end(), line.begin(), ::toupper);
    map<char, int> n_of_occurences;

    int total_characters = 0;

    for (char c : line) {
        // only calculate the A-Z letters (its uppercase), dont count w/ the ' ', '\n' etc.
        if (c >= 65 && c <= 90) {
            // if it's in the map already, just update the value
            if (isInMap(n_of_occurences, c)) {
                n_of_occurences[c] = n_of_occurences[c] + 1;
            }
            // else insert it
            else {
                n_of_occurences[c] = 1;
            }
            total_characters++;
        }
    }

    vector<pair<char, double>> frequencies;
    // iterate over the map to find the freuqnecy of each letter
    for ( auto keyVal : n_of_occurences ) {
        frequencies.push_back(make_pair(keyVal.first, keyVal.second));
    }
    return frequencies;
}

// function to be called from the std::sort() to sort the vector from the highest occurence to the lowest
bool sortFunction(pair<char, double> a, pair<char, double> b) {
    return a.second > b.second;
}

/* Calculate how many letters from my text match the most common text with given a and b */
int getMatches(vector<char> most_commons, vector<char> my_commons, int a, int b) {
    int matches = 0;
    for (auto c : most_commons) {
        for (auto c2 : my_commons) {
            if ((c * a + b) % 26 == c2) {// check if encoding the most_common letter gets me the new one
                matches++;
            }
        }
    }
    return matches;
}


pair<int, int> findKey(vector<pair<char, double>> frequencies, vector<pair<char, double>> alph_freq, struct arg_struct *args) {
    // new_x = (a * x + b) % 26
    // the sentence contained all the leters, ill test the top 3 and bottom 3
    vector<int> possible_as = {1, 3, 5, 7, 9, 11, 15, 17, 19, 21, 23, 25};
    vector<int> possible_bs = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25}; // b can be pretty much anything, but % 26 will end up with one of these
    
    vector<char> most_commons, my_commons;

    int letters_to_cmp = 20; // comparing 20 letters produces best results

    if (frequencies.size() < 20) {
        letters_to_cmp = 15; // if i cant do 20, 15 seems alright
    }
    else if (frequencies.size() < 15) {
        letters_to_cmp = 10; // 10 somehow works, but barely
    }
    else if (frequencies.size() < 10) {
        fprintf(stderr, "Too few letters to analyze!\n");
        exitProgram(ERROR, args);
    }

    // seems to be enough for most things, i tried even bigrams but that doesnt work
    for (auto a = 0; a < letters_to_cmp; a++) {
        most_commons.push_back(alph_freq[a].first - 65);
        my_commons.push_back(frequencies[a].first - 65);
    }

    int probable_akey = 1;
    int probable_bkey = 0;
    int best_match = 0;
    // go through all possible A keys
    for ( auto a : possible_as) {
        // go through B keys
        for ( auto b : possible_bs) {
            int matches = getMatches(most_commons, my_commons, a, b); // how many matches did i get right
            // if it was more matches than I got from the oens before, ill take it
            if (matches > best_match) {
                best_match = matches;
                probable_akey = a;
                probable_bkey = b;
            }
        }
    }
    return make_pair(probable_akey, probable_bkey);
}

void analyseMessage(struct arg_struct *args) {

    if (args->in_flag) {
        vector<string> content = readFile(args);
        string fileContent = "";
        for (string line : content) {
            fileContent += line;
        }
        vector<pair<char, double>> frequencies = frequencyAnalysis(fileContent);

        // sort the read letteres & sort the letters in czech alphabet
        sort (frequencies.begin(), frequencies.end(), sortFunction);
        sort (alphabet_frequency.begin(), alphabet_frequency.end(), sortFunction);

        pair<int, int> decryption = findKey(frequencies, alphabet_frequency, args);

        args->a_key = decryption.first;
        args->b_key = decryption.second;

        string decrypted = decryptOneByOne(fileContent, args);
        writeResult(args, decrypted);
        cout << "a=" << args->a_key << ",b=" << args->b_key << endl;
    }
    else {
        vector<pair<char, double>> frequencies = frequencyAnalysis(args->in_file);

        // sort the read letteres & sort the letters in czech alphabet
        sort (frequencies.begin(), frequencies.end(), sortFunction);
        sort (alphabet_frequency.begin(), alphabet_frequency.end(), sortFunction);
        pair<int, int> decryption = findKey(frequencies, alphabet_frequency, args);

        args->a_key = decryption.first;
        args->b_key = decryption.second;
        string decrypted = decryptOneByOne(args->in_file, args);
        writeResult(args, decrypted);
        cout << "a=" << args->a_key << ",b=" << args->b_key << endl;
    }
}
