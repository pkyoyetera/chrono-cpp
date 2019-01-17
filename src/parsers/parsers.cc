#include <regex>
#include "parsers.hpp"


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

Result Parser::execute(std::string& text, posix_time::ptime& ref) {
    Result results;
    std::smatch match;
    bool possible_match;
    unsigned long idx;

    std::transform(text.begin(), text.end(), text.begin(), ::tolower);
    std::string remainingText = text;

    try {
        possible_match = std::regex_search(remainingText, match, getPattern());
    } catch (std::regex_error& err) {
        std::cerr << err.what() << std::endl;
    }

    while(possible_match) {
        idx = match.position(0) + text.length() - remainingText.length(); // index of match on full text

        parse::ParsedResult res{};
        res = extract(remainingText, match, ref);

        if (/*!strictMode or */res.hasPossibleDates())
            results.push_back(res);
        else
            remainingText = text.substr(match.position() + 1);

        // set remaining text to string immediately following the matched string
        remainingText = text.substr(idx + match[0].str().length());

        try {
            possible_match = std::regex_search(remainingText, match, getPattern());
        } catch (std::regex_error& err) {
            std::cerr << err.what() << std::endl;
        }
    }

    return results;
}

