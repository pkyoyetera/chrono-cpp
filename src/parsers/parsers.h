#ifndef PARSER_H
#define PARSER_H


#include <iostream>
#include <regex>
#include <string>
// #include <optional>

#include "../result.h"

class Parser {
protected:
    bool strictMode;
    std::regex pattern;


public:
    Parser();
    Parser(bool, std::regex);       // strictMode, pattern
    std::regex getPattern();
    virtual parse::ParsedResult extract(std::string, std::smatch, parse::ParsedComponents&) {
        parse::ParsedResult tmp;
        return tmp;
    }
    std::vector<parse::ParsedResult> execute(std::string&, parse::ParsedComponents&);
    ~Parser();
};


#endif