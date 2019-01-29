#ifndef ENWEEKEXPRESSIONPARSER_HPP
#define ENWEEKEXPRESSIONPARSER_HPP

#define PATTERN "(\\W|^)((?:last|next)\\s*(?:(\\d+)|(\\w{3,9}))?\\s*weeks?|this\\s*week)(?=\\W|$)"

#include "src/parsers/parsers.hpp"


class ENWeekExpressionParser : public Parser {
public:
    ENWeekExpressionParser() : Parser(false, std::regex(PATTERN, std::regex::icase)) { }

    parse::ParsedResult extract(std::string tx, std::smatch match, posix_time::ptime& ref, long idx)
    override {
        std::string text = match.str(0).substr(match.length(1));
        parse::ParsedResult result = parse::ParsedResult(ref, idx, text);
        gregorian::date tmp{ref.date()};
        int numberOfWeeks = 1;

        if(!match.str(3).empty()) {
            numberOfWeeks = std::stoi(match.str(3));
        }
        else if (!match.str(4).empty()) {
            numberOfWeeks = utils::INTEGER_WORDS[match.str(4)];
            if(numberOfWeeks < 0)
                return result;
        }
        if(match.str(2).find("last") != std::string::npos)
            tmp -= gregorian::weeks(numberOfWeeks);
        else if(match.str(2).find("next") != std::string::npos)
            tmp += gregorian::weeks(numberOfWeeks);

        result.startDate.implyComponent("year", tmp.year());
        result.startDate.implyComponent("month", tmp.month());
        result.startDate.implyComponent("mday", tmp.day());

        return result;
    }

};

#endif
