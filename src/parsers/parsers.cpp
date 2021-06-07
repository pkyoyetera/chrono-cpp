/**
 * Author: Patrick Kyoyetera
 *          2019
 */

#include "parsers.hpp"

using parser::Parser;

Parser::Parser() = default;
Parser::~Parser() = default;

/*
Parser::Parser(bool strict, std::regex _pattern) {
    strictMode = strict;
    pattern = _pattern;
}
*/
/*
std::regex Parser::getPattern() const {
    return std::regex();
}
*/


const
std::shared_ptr<Parser>& Parser::chain(const std::shared_ptr<Parser>& next)
{
    _next = next;
    return next;
}


void Parser::execute(std::string& text,
                     posix_time::ptime& ref,
                     parse::Result& results)
{
    std::smatch match;
    unsigned long idx;

    text = utils::toLowerCase(text);
    std::string remainingText = text;

    try
    {
        std::regex_search(remainingText, match, getPattern());
    } // only throws if the regex can not be built
    catch (std::regex_error& err)
    {
        std::cerr << err.what() << std::endl;  // fixme: use logging
    }

    while (! match.empty())
    {
        // index of match on full text
        idx = match.position(0) + text.length() - remainingText.length();

        parse::ParsedResult res{};
        res = extract(remainingText, match, ref, idx);

        if(/*!strictMode or */res.hasPossibleDates()) {
            results.push_back(res);
        }
        else {
            remainingText = text.substr(match.position(0) + 1);
        }

        // set remaining text to string immediately following the matched string
        remainingText = text.substr(idx + match.length(0));

        try
        {
            std::regex_search(remainingText, match, getPattern());
        }
        catch (std::regex_error& err)
        {
            std::cerr << err.what() << std::endl;
        }
    }

    if (_next)
        _next->execute(text, ref, results);
}