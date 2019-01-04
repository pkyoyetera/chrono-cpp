#ifndef ENCASUALDATEPARSER_H
#define ENCASUALDATEPARSER_H

#include "src/parsers/parsers.h"

#define PATTERN "(\\W|^)(now|today|tonight|last\\s*night|(?:tomorrow|tmr|yesterday)\\s*|tomorrow|tmr|yesterday)(?=\\W|$)"

class ENCasualDateParser : public Parser {

public:
    ENCasualDateParser() : Parser(false, std::regex(PATTERN)) { }

    parse::ParsedResult extract(std::string tx, std::smatch match, posix_time::ptime& ref) {
        std::regex ln(("last\\s*night"));

        std::string text = match[0].str().substr(match[1].length());
        int idx = match.position(0) + match[1].length();
        posix_time::ptime tmp{ref};

        parse::ParsedResult result = parse::ParsedResult(ref, idx, text/*, tc*/);

        if(!text.compare("tonight")) {
            // implies coming midnight, normally
            result.startDate.implyComponent("hour", 22);
            result.startDate.set_wDay(ref.date().day_of_week());
            // also make the meridian = 1/post-meridian
        }
        else if (!text.compare("tomorrow") or !text.compare("tmr")) {
            // checks not tomorrow on a late night
            if(ref.time_of_day().hours() > 1) {
                tmp += gregorian::days(1);
                result.startDate.set_mDay(tmp.date().day());
                result.startDate.set_wDay(tmp.date().day_of_week());
            }
        }

        else if(!text.compare("yesterday")) {
            tmp -= gregorian::days(1);
            result.startDate.set_mDay(tmp.date().day());
        }
        else if(std::regex_search(text, ln)) {
            result.startDate.implyComponent("hour", 0);
            if(ref.time_of_day().hours() > 6) {
                tmp -= gregorian::days(1);
                result.startDate.set_mDay(tmp.date().day());
            }
        }
        else if(!text.compare("now")) {
            result.startDate.setHour(ref.time_of_day().hours());
            result.startDate.setMinute(ref.time_of_day().minutes());
            result.startDate.setSeconds(ref.time_of_day().seconds());
            result.startDate.set_wDay(ref.date().day_of_week());
        }
        //else  {  } // this is not a good idea, dummy

        result.startDate.implyComponent("year", ref.date().year());
        result.startDate.implyComponent("month", ref.date().month());
        result.startDate.implyComponent("mday", ref.date().day());
        result.startDate.implyComponent("hour", ref.time_of_day().hours());

        result.setTag(utils::ENCasualDateParser);

        return result;
    }
};

#undef PATTERN
#endif