#pragma once

#include "src/parsers/parsers.hpp"

namespace parser {

class ENWeekExpressionParser : public Parser {
public:
    ENWeekExpressionParser() = default;

    std::regex getPattern() const override {
        static const std::regex PATTERN {
                R"((\b))"
                    R"(((?:last|next)\s+)"
                        R"((?:(\d+)|(\w{3,9}))?\s*)"
                            R"(weeks?|this\s*week))"
                R"((\b))", std::regex::icase};

        return PATTERN;
    }

    parse::ParsedResult extract(std::string&, const std::smatch& match, const posix_time::ptime& ref, long idx)
    override {
        std::string text = match.str(FULL_MATCH);
        parse::ParsedResult result = parse::ParsedResult(ref, idx, text);
        gregorian::date tmp{ref.date()};
        int numberOfWeeks = 1;

        if(!match.str(3).empty()) {
            numberOfWeeks = std::stoi(match.str(3));
        }
        else if(!match.str(4).empty()) {
            numberOfWeeks = utils::INTEGER_WORDS[match.str(4)];
            if(numberOfWeeks < 0) {
                return result;
            }
        }
        if(match.str(2).find("last") != std::string::npos) {
            tmp -= gregorian::weeks(numberOfWeeks);
        }
        else if(match.str(2).find("next") != std::string::npos) {
            tmp += gregorian::weeks(numberOfWeeks);
        }

        result.startDate.implyComponent("year", tmp.year());
        result.startDate.implyComponent("month", tmp.month());
        result.startDate.implyComponent("mday", tmp.day());

        return result;
    }
};

}