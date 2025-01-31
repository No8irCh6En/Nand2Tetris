#include "CodeWriter.hpp"
#include <iostream>
CodeWriter::CodeWriter(std::string path) {
    outputFile.open(path + ".asm");
    staticLabel = path;
    int slash_index;
    while ((slash_index = staticLabel.find("/")) != -1) {
        staticLabel = staticLabel.substr(slash_index + 1);
    }
    staticLabel += ".";
}

CodeWriter::~CodeWriter() {
    outputFile.close();
}

void CodeWriter::writeArithmetic(std::string command) {
    if (command == "add") {
        outputFile << "@SP" << std::endl;
        outputFile << "AM=M-1" << std::endl;
        outputFile << "D=M" << std::endl;
        outputFile << "A=A-1" << std::endl;
        outputFile << "M=M+D" << std::endl;
    } else if (command == "sub") {
        outputFile << "@SP" << std::endl;
        outputFile << "AM=M-1" << std::endl;
        outputFile << "D=M" << std::endl;
        outputFile << "A=A-1" << std::endl;
        outputFile << "M=M-D" << std::endl;
    } else if (command == "neg") {
        outputFile << "@SP" << std::endl;
        outputFile << "A=M-1" << std::endl;
        outputFile << "M=-M" << std::endl;
    } else if (command == "eq") {
        outputFile << "@SP" << std::endl;
        outputFile << "AM=M-1" << std::endl;
        outputFile << "D=M" << std::endl;
        outputFile << "A=A-1" << std::endl;
        outputFile << "D=M-D" << std::endl;
        outputFile << "M=-1" << std::endl;  // eq
        outputFile << "@eq_" + std::to_string(eq_i) << std::endl;
        outputFile << "D;JEQ" << std::endl;
        outputFile << "@SP" << std::endl;  // eq
        outputFile << "A=M-1" << std::endl;
        outputFile << "M=0" << std::endl;  // not eq
        outputFile << "(eq_" + std::to_string(eq_i) + ")" << std::endl;
        eq_i++;
    } else if (command == "gt") {
        outputFile << "@SP" << std::endl;
        outputFile << "AM=M-1" << std::endl;
        outputFile << "D=M" << std::endl;
        outputFile << "A=A-1" << std::endl;
        outputFile << "D=M-D" << std::endl;
        outputFile << "M=-1" << std::endl;  // gt
        outputFile << "@gt_" + std::to_string(gt_i) << std::endl;
        outputFile << "D;JGT" << std::endl;
        outputFile << "@SP" << std::endl;
        outputFile << "A=M-1" << std::endl;
        outputFile << "M=0" << std::endl;  // not gt
        outputFile << "(gt_" + std::to_string(gt_i) + ")" << std::endl;
        gt_i++;
    } else if (command == "lt") {
        outputFile << "@SP" << std::endl;
        outputFile << "AM=M-1" << std::endl;
        outputFile << "D=M" << std::endl;
        outputFile << "A=A-1" << std::endl;
        outputFile << "D=M-D" << std::endl;
        outputFile << "M=-1" << std::endl;  // lt
        outputFile << "@lt_" + std::to_string(lt_i) << std::endl;
        outputFile << "D;JLT" << std::endl;
        outputFile << "@SP" << std::endl;
        outputFile << "A=M-1" << std::endl;
        outputFile << "M=0" << std::endl;  // not lt
        outputFile << "(lt_" + std::to_string(lt_i) + ")" << std::endl;
        lt_i++;
    } else if (command == "and") {
        outputFile << "@SP" << std::endl;
        outputFile << "AM=M-1" << std::endl;
        outputFile << "D=M" << std::endl;
        outputFile << "A=A-1" << std::endl;
        outputFile << "M=M&D" << std::endl;
    } else if (command == "or") {
        outputFile << "@SP" << std::endl;
        outputFile << "AM=M-1" << std::endl;
        outputFile << "D=M" << std::endl;
        outputFile << "A=A-1" << std::endl;
        outputFile << "M=M|D" << std::endl;
    } else if (command == "not") {
        outputFile << "@SP" << std::endl;
        outputFile << "A=M-1" << std::endl;
        outputFile << "M=!M" << std::endl;
    }
}

void CodeWriter::writePushPop(std::string command,
                              std::string segment,
                              std::string index) {
    if (command == "push") {
        if (segment == "constant") {
            outputFile << "@" + index << std::endl;
            outputFile << "D=A" << std::endl;
        } else if (segment == "pointer") {
            outputFile << "@" + index << std::endl;
            outputFile << "D=A" << std::endl;
            outputFile << "@3" << std::endl;
            outputFile << "A=A+D" << std::endl;
            outputFile << "D=M" << std::endl;
        } else if (segment == "static") {
            outputFile << "@" + staticLabel + index << std::endl;
            outputFile << "D=M" << std::endl;
        } else if (labelMap.count(segment)) {
            outputFile << "@" + index << std::endl;
            outputFile << "D=A" << std::endl;
            outputFile << "@" + labelMap[segment] << std::endl;
            outputFile << "A=M+D" << std::endl;
            outputFile << "D=M" << std::endl;
        } else if (segment == "temp") {
            outputFile << "@5" << std::endl;
            outputFile << "D=A" << std::endl;
            outputFile << "@" + index << std::endl;
            outputFile << "A=A+D" << std::endl;
            outputFile << "D=M" << std::endl;
        } else {
            std::cerr << "Not a command." << std::endl;
            return;
        }
        outputFile << "@SP" << std::endl;
        outputFile << "A=M" << std::endl;
        outputFile << "M=D" << std::endl;
        outputFile << "@SP" << std::endl;
        outputFile << "M=M+1" << std::endl;
    } else {  // pop
        if (segment == "pointer") {
            outputFile << "@" + index << std::endl;
            outputFile << "D=A" << std::endl;
            outputFile << "@3" << std::endl;
            outputFile << "D=A+D" << std::endl;
        } else if (segment == "static") {
            outputFile << "@" + staticLabel + index << std::endl;
            outputFile << "D=A" << std::endl;
        } else if (labelMap.count(segment)) {
            outputFile << "@" + index << std::endl;
            outputFile << "D=A" << std::endl;
            outputFile << "@" + labelMap[segment] << std::endl;
            outputFile << "D=M+D" << std::endl;
        } else if (segment == "temp") {
            outputFile << "@5" << std::endl;
            outputFile << "D=A" << std::endl;
            outputFile << "@" + index << std::endl;
            outputFile << "D=A+D" << std::endl;
        } else {
            std::cerr << "Not a command." << std::endl;
            return;
        }
        outputFile << "@R15" << std::endl;
        outputFile << "M=D" << std::endl;

        outputFile << "@SP" << std::endl;
        outputFile << "AM=M-1" << std::endl;
        outputFile << "D=M" << std::endl;
        outputFile << "@R15" << std::endl;
        outputFile << "A=M" << std::endl;
        outputFile << "M=D" << std::endl;
    }
}

void CodeWriter::writeEnd() {
    outputFile << "(end)" << std::endl;
    outputFile << "@end" << std::endl;
    outputFile << "0;JMP" << std::endl;
}