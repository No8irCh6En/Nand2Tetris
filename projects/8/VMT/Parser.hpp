/* Handles the parsing of .vm file
Reads a VM command, parses the command into its lexical components,
and provides convenient access to these components
Ignores white space and comments
*/

#ifndef PARSER_H
#define PARSER_H
#include <fstream>
#include <string>
#include <vector>

class Parser {
    std::ifstream inputFile;
    std::vector<std::string> tokens;

   public:
    enum ctype {
        C_FALSE,
        C_ARITHMETIC,
        C_PUSH,
        C_POP,
        C_LABEL,
        C_GOTO,
        C_IF,
        C_FUNCTION,
        C_RETURN,
        C_CALL
    };
    ctype type;
    Parser();
    void open(std::string path);
    ~Parser();
    bool hasMoreLines();
    std::vector<std::string> getTokens(const std::string&, const std::string&);
    std::string removeComments(std::string singleLine);

    bool advance();
    ctype commandType();
    std::string arg1();
    std::string arg2();
};

#endif