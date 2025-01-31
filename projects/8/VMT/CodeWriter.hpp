#ifndef CODEWRITER_H
#define CODEWRITER_H

#include <fstream>
#include <string>
#include <unordered_map>

class CodeWriter {
    std::ofstream outputFile;
    std::unordered_map<std::string, std::string> labelMap = {
        {"local", "LCL"},
        {"argument", "ARG"},
        {"this", "THIS"},
        {"that", "THAT"}};
    std::string staticLabel;
    int eq_i = 0;
    int gt_i = 0;
    int lt_i = 0;
    int addr_i = 0;
    void saveCaller(std::string);

   public:
    CodeWriter(std::string);
    ~CodeWriter();
    void writeInit();
    void writeLabel(std::string);
    void writeGoTo(std::string);
    void writeIf(std::string);
    void writeCall(std::string, int);
    void writeReturn();
    void writeFunction(std::string, int);
    void setStaticField(std::string);
    void writeArithmetic(std::string);
    void writePushPop(std::string, std::string, std::string);
    void writeEnd();
};

#endif