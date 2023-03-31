#include "kry.hpp"
#include "argParse.hpp"
#include "encrypt.hpp"
#include "decrypt.hpp"
#include "analyse.hpp"

int main(int argc, char* argv[]) {
    struct arg_struct *args = parseArgs(argc, argv);

    if (args->encrypt) {
        encryptMessage(args);
    }
    else if (args->decrypt) {
        decryptMessage(args);
    }
    else if (args->analyse) {
        analyseMessage(args);
    }

    delete args;
    return (SUCCESS);
}
