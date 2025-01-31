#include "VMTranslator.hpp"
#include <iostream>
VMTranslator::VMTranslator(std::vector<std::string> list,
                           std::string outputPath,
                           bool init)
    : infileList(list), codeWriter(outputPath), to_init(init) {}

VMTranslator::~VMTranslator() {}

bool VMTranslator::need_init() {
    return to_init;
}

void VMTranslator::convert() {
    if (need_init()) {
        codeWriter.writeInit();
    }
    for (auto path : infileList) {
        parser.open(path);
        int slash_idx = path.find('/');
        int dot_idx = path.find('.');
        std::string field = path.substr(slash_idx + 1, dot_idx - slash_idx - 1);
        codeWriter.setStaticField(field);
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
            } else if (parser.commandType() == Parser::C_LABEL) {
                codeWriter.writeLabel(parser.arg2());
            } else if (parser.commandType() == Parser::C_GOTO) {
                codeWriter.writeGoTo(parser.arg2());
            } else if (parser.commandType() == Parser::C_FUNCTION) {
                codeWriter.writeFunction(parser.arg1(),
                                         std::stoi(parser.arg2()));
            } else if (parser.commandType() == Parser::C_CALL) {
                codeWriter.writeCall(parser.arg1(), std::stoi(parser.arg2()));
            } else if (parser.commandType() == Parser::C_RETURN) {
                codeWriter.writeReturn();
            }
        }
    }
    if (need_init()) {
        codeWriter.writeEnd();
    }
}