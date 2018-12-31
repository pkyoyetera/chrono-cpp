/**
 *
 *
 */
#ifndef ENCASUALTIMEPARSER_H
#define ENCASUALTIMEPARSER_H

#include <chrono>

#include "src/parsers/parsers.h"

#define PATTERN "(\\W|^)((this)?\\s*(morning|afternoon|evening|noon|night))"


class ENCasualTimeParser : public Parser {

public:
    ENCasualTimeParser(): Parser(false, std::regex(PATTERN)) { }

    parse::ParsedResult extract(std::string tx, std::smatch match, posix_time::ptime& ref) {
        std::string text = match[0].str().substr(match[1].length());
        int idx = match.position() + match[1].length();
        struct tm local;

        parse::ParsedResult result = parse::ParsedResult(ref, idx, text);

        int time_match = 4;
        if(match[time_match] == "") time_match = 3;

        std::string subs = match[time_match].str();
        std::transform(subs.begin(), subs.end(), subs.begin(), ::tolower);

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
        else {
            // std::cerr << "invalid match" << std::endl; // this should be a loggable error
        }

        local = posix_time::to_tm(ref);

        result.startDate.implyComponent("year",  ref.date().year());
        result.startDate.implyComponent("month", ref.date().month());
        result.startDate.implyComponent("mday",  ref.date().day());
        result.startDate.implyComponent("wday",  ref.date().day_of_week());     /// todo: mappping between tm_wday and gregorian_day_of_the_week
        result.startDate.implyComponent( "min",  ref.time_of_day().minutes());

        // indicate parser used
        result.setTag(utils::ENCasualTimeParser);

        return result;
    }
};


#endif
