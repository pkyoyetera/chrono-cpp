/**
 *
 *
 */
#ifndef ENCASUALTIMEPARSER_H
#define ENCASUALTIMEPARSER_H

#include <chrono>

#include "src/parsers/parsers.hpp"

#define PATTERN "(\\W|^)((this)?\\s*(morning|afternoon|evening|noon|night))"


class ENCasualTimeParser : public Parser {

public:
    ENCasualTimeParser(): Parser(false, std::regex(PATTERN, std::regex::icase)) { }

    parse::ParsedResult extract(std::string tx, std::smatch match, posix_time::ptime& ref) override {
        std::string text = match.str(0).substr(match.length(1));
        long idx = match.position(0) + match.length(1);

        parse::ParsedResult result = parse::ParsedResult(ref, idx, text);

        unsigned time_match = 4;
        if(match.str(time_match).empty()) time_match = 3;

        std::string subs{utils::toLowerCase(match.str(time_match))};

        if(!subs.compare("afternoon")) {
            result.startDate.implyComponent("hour", 15);
        }
        else if(!subs.compare("evening") or !subs.compare("night")) {
            result.startDate.implyComponent("hour", 20);
            // time
        }
        else if(!subs.compare("morning")) {
            result.startDate.implyComponent("hour", 6);
        }
        else if (!subs.compare("noon")) {
            result.startDate.implyComponent("hour", 12);
        }

        result.startDate.implyComponent("year",  ref.date().year());
        result.startDate.implyComponent("month", ref.date().month());
        result.startDate.implyComponent("mday",  ref.date().day());
        result.startDate.implyComponent("wday",  ref.date().day_of_week());
        result.startDate.implyComponent( "min",  ref.time_of_day().minutes());

        // indicate parser used
        result.setTag(utils::ENCasualTimeParser);

        return result;
    }
};

#undef PATTERN
#endif
