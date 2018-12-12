#include <regex>
#include "parsers.h"


Parser::Parser() {
    strictMode = false;
}

Parser::~Parser() { }

Parser::Parser(bool strict, std::regex _pattern) {
    strictMode = strict;
    pattern = _pattern;
}

std::regex Parser::getPattern() {
    return pattern;
}
// virtual parse::ParsedResult extract();

std::vector<parse::ParsedResult> Parser::execute(std::string& text, parse::ParsedComponents& ref) {
    std::vector<parse::ParsedResult> results;
    std::string remainingText = text;
    std::smatch match;
    bool possible_match;
    unsigned idx;

    do {
        try {
            possible_match = std::regex_search(remainingText, match, getPattern());
        } catch (std::regex_error& err) {
            std::cerr << err.what() << std::endl;
        }

        idx = match.position(0) + text.length() - remainingText.length(); // index of match on full text

        parse::ParsedResult res = extract(remainingText, match, ref);

        if (!strictMode or res.hasPossibleDates())
            results.push_back(res);
        else {
            // std::cerr << "here is the dumbass loop!" << std::endl;
            remainingText = text.substr(match.position() + 1);
        }
            // set remaining text to string immediately following the matched string
        remainingText = text.substr(idx, match[0].str().length());
    } while(possible_match);
        /*try {
            possible_match = std::regex_search(remainingText, match, getPattern());

        } catch (std::regex_error& err) {
            std::cerr << err.what() << std::endl;
        }
         */

    return results;
}

