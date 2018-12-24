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
        struct tm local = posix_time::to_tm(ref);

        parse::ParsedResult result = parse::ParsedResult(ref, idx, text/*, tc*/);

        if(!text.compare("tonight")) {
            // implies coming midnight, normally
            result.startDate.implyComponent("hour", 22);
            result.startDate.set_wDay(local.tm_wday);
            // also make the meridian = 1/post-meridian
        }
        else if (!text.compare("tomorrow") or !text.compare("tmr")) {
            // checks not tomorrow on a late night
            if(local.tm_hour > 1) {
                result.startDate.set_mDay(local.tm_mday + 1);
                result.startDate.set_wDay(local.tm_wday + 1);
            }
        }

        else if(!text.compare("yesterday")) {
            result.startDate.set_mDay(local.tm_mday - 1);
        }

        else if(std::regex_search(text, ln)) {
            result.startDate.implyComponent("hour", 0);
            if(local.tm_hour > 6)
                result.startDate.set_mDay(local.tm_mday - 1);
        }

        else if(!text.compare("now")) {
            result.startDate.setHour(local.tm_hour);
            result.startDate.setMinute(local.tm_min);
            result.startDate.setSeconds(local.tm_sec);
            result.startDate.set_wDay(local.tm_wday);
        }

        else  {  } // this is not a good idea, dummy

        result.startDate.implyComponent("year", local.tm_year);
        result.startDate.implyComponent("month", local.tm_mon);
        result.startDate.implyComponent("mday", local.tm_mday);

        result.setTag(utils::ENCasualDateParser);

        return result;
    }
};


#endif