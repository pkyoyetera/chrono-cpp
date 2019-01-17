#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include "src/result.hpp"
#include "src/utils/utils.hpp"

class Parser {
protected:
    bool strictMode;
    std::regex pattern;

public:
    Parser();
    Parser(bool, std::regex);       // strictMode, pattern
    std::regex getPattern();

    virtual parse::ParsedResult extract(std::string, std::smatch, posix_time::ptime&) {
        parse::ParsedResult tmp{ };
        return tmp;
    }

    Result execute(std::string&, posix_time::ptime&);
    ~Parser();
};

#endif
