#ifndef ENCASUALDATEPARSER_H
#define ENCASUALDATEPARSER_H

#include "src/parsers/parsers.hpp"

#define PATTERN "(\\W|^)(now|today|tonight|last\\s*night|(?:tomorrow|tmr|yesterday)\\s*" \
                "|tomorrow|tmr|yesterday)(?=\\W|$)"

class ENCasualDateParser : public Parser {

public:
    ENCasualDateParser() : Parser(false, std::regex(PATTERN, std::regex::icase)) { }

    parse::ParsedResult extract(std::string tx, std::smatch match, posix_time::ptime& ref, long idx)
    override {
        std::string text = match.str(0).substr(match.length(1));
        // long idx = match.position(0) + match.length(1);
        posix_time::ptime tmp{ref};
        parse::ParsedResult result = parse::ParsedResult(ref, idx, text);

        if(!text.compare("tonight")) {
            // implies coming night, normally
            result.startDate.implyComponent("hour", 22);
        }
        else if (!text.compare("tomorrow") or !text.compare("tmr")) {
            // checks not tomorrow on a late night
            if(ref.time_of_day().hours() > 1) {
                tmp += gregorian::days(1);
            }
        }

        else if(!text.compare("yesterday")) {
            tmp -= gregorian::days(1);
            result.startDate.implyComponent("hour", ref.time_of_day().hours());
        }
        else if(std::regex_search(text, std::regex("last\\s*night", std::regex::icase))) {
            result.startDate.implyComponent("hour", 0);
            if(ref.time_of_day().hours() > 6) {
                tmp -= gregorian::days(1);
            }
        }
        else if(!text.compare("now")) {
            result.startDate.setHour(ref.time_of_day().hours());
            result.startDate.setMinute(ref.time_of_day().minutes());
            result.startDate.setSeconds(ref.time_of_day().seconds());
            // result.startDate.set_wDay(ref.date().day_of_week());
        }

        result.startDate.setYear(tmp.date().year());
        result.startDate.setMonth(tmp.date().month());
        result.startDate.set_mDay(tmp.date().day());
        // result.startDate.implyComponent("hour", ref.time_of_day().hours());

        result.setTag(utils::ENCasualDateParser);

        return result;
    }
};

#undef PATTERN
#endif