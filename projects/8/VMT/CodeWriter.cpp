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

void CodeWriter::setStaticField(std::string field) {
    staticLabel = field;
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

void CodeWriter::writeLabel(std::string label) {
    outputFile << "// " << label << std::endl;
    outputFile << "(" + label + ")" << std::endl;
}

void CodeWriter::writeGoTo(std::string label) {
    outputFile << "// goto" << std::endl;
    outputFile << "@" + label << std::endl;
    outputFile << "0;JMP" << std::endl;
}

void CodeWriter::writeIf(std::string label) {
    outputFile << "// if-goto" << std::endl;
    outputFile << "@SP" << std::endl;
    outputFile << "AM=M-1" << std::endl;
    outputFile << "D=M" << std::endl;
    outputFile << "@" + label << std::endl;
    outputFile << "D;JNE" << std::endl;
}

void CodeWriter::writeFunction(std::string funName, int nVars) {
    outputFile << "// function " << funName << " " << nVars << std::endl;
    writeLabel(funName);
    for (int i = 0; i < nVars; i++) {
        outputFile << "@SP" << std::endl;
        outputFile << "A=M" << std::endl;
        outputFile << "M=0" << std::endl;
        outputFile << "@SP" << std::endl;
        outputFile << "M=M+1" << std::endl;
    }
}

void CodeWriter::saveCaller(std::string symbol) {
    outputFile << "@" + symbol << std::endl;
    outputFile << "D=M" << std::endl;
    outputFile << "@SP" << std::endl;
    outputFile << "A=M" << std::endl;
    outputFile << "M=D" << std::endl;
    outputFile << "@SP" << std::endl;
    outputFile << "M=M+1" << std::endl;
}

void CodeWriter::writeCall(std::string funName, int nArgs) {
    outputFile << "// call" << funName << " " << nArgs << std::endl;

    std::string returnAddr = "func_" + std::to_string(addr_i);
    writePushPop("push", "constant", returnAddr);
    saveCaller("LCL");
    saveCaller("ARG");
    saveCaller("THIS");
    saveCaller("THAT");

    // ARG=SP-5-nArgs
    outputFile << "@5" << std::endl;
    outputFile << "D=A" << std::endl;
    outputFile << "@" + std::to_string(nArgs) << std::endl;
    outputFile << "D=D+A" << std::endl;
    outputFile << "@SP" << std::endl;
    outputFile << "D=M-D" << std::endl;
    outputFile << "@ARG" << std::endl;
    outputFile << "M=D" << std::endl;
    // LCL=SP
    outputFile << "@SP" << std::endl;
    outputFile << "D=M" << std::endl;
    outputFile << "@LCL" << std::endl;
    outputFile << "M=D" << std::endl;

    writeGoTo(funName);
    writeLabel(returnAddr);

    addr_i++;
}

void CodeWriter::writeReturn() {
    outputFile << "// return" << std::endl;
    // endFrame = LCL
    outputFile << "@LCL" << std::endl;
    outputFile << "D=M" << std::endl;
    outputFile << "@R15" << std::endl;
    outputFile << "M=D" << std::endl;
    // retAddr = *(endFrame-5)
    outputFile << "@5" << std::endl;
    outputFile << "A=D-A" << std::endl;
    outputFile << "D=M" << std::endl;
    outputFile << "@R14" << std::endl;
    outputFile << "M=D" << std::endl;
    // *ARG = pop()
    outputFile << "@SP" << std::endl;
    outputFile << "AM=M-1" << std::endl;
    outputFile << "D=M" << std::endl;
    outputFile << "@ARG" << std::endl;
    outputFile << "A=M" << std::endl;
    outputFile << "M=D" << std::endl;
    // SP = ARG+1
    outputFile << "@ARG" << std::endl;
    outputFile << "D=M+1" << std::endl;
    outputFile << "@SP" << std::endl;
    outputFile << "M=D" << std::endl;
    // THAT = *(engFrame-1)
    outputFile << "@R15" << std::endl;
    outputFile << "A=M-1" << std::endl;
    outputFile << "D=M" << std::endl;
    outputFile << "@THAT" << std::endl;
    outputFile << "M=D" << std::endl;
    // THIS = *(endFrame-2)
    outputFile << "@2" << std::endl;
    outputFile << "D=A" << std::endl;
    outputFile << "@R15" << std::endl;
    outputFile << "A=M-D" << std::endl;
    outputFile << "D=M" << std::endl;
    outputFile << "@THIS" << std::endl;
    outputFile << "M=D" << std::endl;
    // ARG = *(endFrame-3)
    outputFile << "@3" << std::endl;
    outputFile << "D=A" << std::endl;
    outputFile << "@R15" << std::endl;
    outputFile << "A=M-D" << std::endl;
    outputFile << "D=M" << std::endl;
    outputFile << "@ARG" << std::endl;
    outputFile << "M=D" << std::endl;
    // LCL = *(endFrame-4)
    outputFile << "@4" << std::endl;
    outputFile << "D=A" << std::endl;
    outputFile << "@R15" << std::endl;
    outputFile << "A=M-D" << std::endl;
    outputFile << "D=M" << std::endl;
    outputFile << "@LCL" << std::endl;
    outputFile << "M=D" << std::endl;
    // goto retAddr
    outputFile << "@R14" << std::endl;
    outputFile << "A=M" << std::endl;
    outputFile << "0;JMP" << std::endl;

    outputFile << std::endl;
}

void CodeWriter::writeEnd() {
    outputFile << "(end)" << std::endl;
    outputFile << "@end" << std::endl;
    outputFile << "0;JMP" << std::endl;
}

void CodeWriter::writeInit() {
    outputFile << "// init" << std::endl;
    outputFile << "@256" << std::endl;
    outputFile << "D=A" << std::endl;
    outputFile << "@SP" << std::endl;
    outputFile << "M=D" << std::endl;

    outputFile << "@1" << std::endl;
    outputFile << "D=A" << std::endl;
    outputFile << "@LCL" << std::endl;
    outputFile << "M=D" << std::endl;
    outputFile << "@2" << std::endl;
    outputFile << "D=A" << std::endl;
    outputFile << "@ARG" << std::endl;
    outputFile << "M=D" << std::endl;
    outputFile << "@3" << std::endl;
    outputFile << "D=A" << std::endl;
    outputFile << "@THIS" << std::endl;
    outputFile << "M=D" << std::endl;
    outputFile << "@4" << std::endl;
    outputFile << "D=A" << std::endl;
    outputFile << "@THAT" << std::endl;
    outputFile << "M=D" << std::endl;

    writeCall("Sys.init", 0);
}