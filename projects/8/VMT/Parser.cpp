#include "Parser.hpp"
#include <cstring>
#include <iostream>

Parser::Parser() {}

Parser::~Parser() {
    if (inputFile.is_open())
        inputFile.close();
}

bool Parser::hasMoreLines() {
    if (inputFile.is_open())
        return !inputFile.eof();
    return false;
}

void Parser::open(std::string path) {
    if (inputFile.is_open())
        inputFile.close();
    inputFile.open(path);
}

std::vector<std::string> Parser::getTokens(const std::string& line,
                                           const std::string& delimiter = " ") {
    std::vector<std::string> tokens;
    if (line == "")
        return tokens;
    char* strs = new char[line.length() + 1];
    strcpy(strs, line.c_str());
    char* delim = new char[delimiter.length() + 1];
    strcpy(delim, delimiter.c_str());
    char* now = strtok(strs, delim);
    while (now != nullptr) {
        tokens.push_back(now);
        now = strtok(NULL, delim);
    }
    delete strs;
    delete delim;
    return tokens;
}

std::string Parser::removeComments(std::string singleLine) {
    size_t found = singleLine.find("//");
    if (found != std::string::npos) {
        singleLine = singleLine.substr(0, found);
    }
    return singleLine;
}

bool Parser::advance() {
    std::string command;
    getline(inputFile, command);
    command = removeComments(command);
    tokens = getTokens(command);
    if (tokens.size() == 0)
        return false;
    std::string token0 = tokens[0];
    if (token0 == "add" || token0 == "sub" || token0 == "neg" ||
        token0 == "eq" || token0 == "lt" || token0 == "gt" || token0 == "and" ||
        token0 == "or" || token0 == "not") {
        type = C_ARITHMETIC;
    } else if (token0 == "pop") {
        type = C_POP;
    } else if (token0 == "push") {
        type = C_PUSH;
    } else if (token0 == "label") {
        type = C_LABEL;
    } else if (token0 == "goto") {
        type = C_GOTO;
    } else if (token0 == "if-goto") {
        type = C_IF;
    } else if (token0 == "function") {
        type = C_FUNCTION;
    } else if (token0 == "call") {
        type = C_CALL;
    } else if (token0 == "return") {
        type = C_RETURN;
    } else {
        type = C_FALSE;
    }
    return true;
}

Parser::ctype Parser::commandType() {
    return type;
}

std::string Parser::arg1() {
    if (type == C_ARITHMETIC || type == C_GOTO || type == C_IF ||
        type == C_LABEL) {
        return tokens[0];
    } else if (type == C_RETURN) {
        return "ERROR";
    } else {
        return tokens[1];
    }
}

std::string Parser::arg2() {
    if (type == C_PUSH || type == C_POP || type == C_FUNCTION ||
        type == C_CALL) {
        return tokens[2];
    } else if (type == C_IF || type == C_GOTO || type == C_LABEL) {
        return tokens[1];
    } else {
        return "ERROR";
    }
}
