#ifndef VMTRANSLATOR_H
#define VMTRANSLATOR_H

#include <vector>
#include "CodeWriter.hpp"
#include "Parser.hpp"
class VMTranslator {
    Parser parser;
    CodeWriter codeWriter;
    std::vector<std::string> infileList;
    bool to_init;

   public:
    VMTranslator(std::vector<std::string> list,
                 std::string outputPath,
                 bool init);
    ~VMTranslator();
    void convert();
    bool need_init();
};

#endif