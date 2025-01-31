#include "CodeWriter.hpp"
#include "Parser.hpp"

class VMTranslator {
    Parser parser;
    CodeWriter codeWriter;

   public:
    VMTranslator(std::string inputPath, std::string outputPath);
    ~VMTranslator();
    void convert();
};