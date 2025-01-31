#include <iostream>
#include "VMTranslator.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "invalid number of arguments" << std::endl;
        std::cerr << "Usage: VMTranslator <filename>.vm" << std::endl;
        return 1;
    }
    std::string inpath(argv[1]);
    int dot_index = inpath.find_last_of('.');
    if (dot_index == -1) {
        std::cerr << "VMTranslator: invalid filename" << std::endl;
        std::cerr << "Usage: VMTranslator <filename>.vm" << std::endl;
        return 1;
    }
    std::string suffix = inpath.substr(dot_index);
    if (suffix != ".vm") {
        std::cerr << "VMTranslator: invalid filename" << std::endl;
        std::cerr << "Usage: VMTranslator <filename>.vm" << std::endl;
        return 1;
    }
    std::string outpath = inpath.substr(0, dot_index);
    VMTranslator translator(inpath, outpath);
    translator.convert();
    return 0;
}