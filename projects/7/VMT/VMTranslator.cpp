#include "VMTranslator.hpp"
#include <iostream>
VMTranslator::VMTranslator(std::string inputPath, std::string outputPath)
    : parser(inputPath), codeWriter(outputPath) {}

VMTranslator::~VMTranslator() {}

void VMTranslator::convert() {
    while (parser.hasMoreLines()) {
        bool has_meaning = parser.advance();
        if (has_meaning == false)
            continue;
        if (parser.commandType() == Parser::C_ARITHMETIC) {
            codeWriter.writeArithmetic(parser.arg1());
        } else if (parser.commandType() == Parser::C_PUSH ||
                   parser.commandType() == Parser::C_POP) {
            std::string type =
                parser.commandType() == Parser::C_PUSH ? "push" : "pop";
            codeWriter.writePushPop(type, parser.arg1(), parser.arg2());
        }
    }
    codeWriter.writeEnd();
}