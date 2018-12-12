/**
 *
 *
 */

#include <chrono>

#include "src/parsers/parsers.h"
// #include "../../../external/date/include/date/date.h"

#define PATTERN "(\\W|^)((this)?\\s*(morning|afternoon|evening|noon|night))"


class ENCasualTimeParser : public Parser {

public:
    ENCasualTimeParser(): Parser(false, std::regex(PATTERN)) { }
    //  ENCasualTimeParser() {    }

    parse::ParsedResult extract(std::string tx, std::smatch match, parse::ParsedComponents& ref) {
        std::string text = match[0].str().substr(match[1].length());
        int idx = match.position() + match[1].length();  /// match.position does not give the required index of the foirst match, review
        //struct tm* local;

        /*
         if ref date is a valid date, then use it to create the correct date
        */

        parse::ParsedResult result = parse::ParsedResult(ref, idx, text/*, tc*/);

        int time_match = 4;
        if(match[time_match] == "") time_match = 3;

        std::string subs = match[time_match].str();
        std::transform(subs.begin(), subs.end(), subs.begin(), ::tolower);

        if(!subs.compare("afternoon")) {
            result.startDate.implyComponent("hour", 15);
            // also imply pm or am for each and everyone of them
        }
        else if(!subs.compare("evening") or !subs.compare("night")) {
            result.startDate.implyComponent("hour", 20);
            // time
        }
        else if(!subs.compare("morning")) {
            result.startDate.implyComponent("hour", 6);
            // pm or am
        }
        else if (!subs.compare("noon")) {
            result.startDate.implyComponent("hour", 12);
            // pm or am
        }
        else {
            // std::cerr << "invalid match" << std::endl; // this should be a loggable error
        }

        /*
        time_t tmp;
        time(&tmp);
        local = localtime(&tmp);
        */

        result.startDate.implyComponent("year",  ref.getYear());
        result.startDate.implyComponent("month", ref.getMonth());
        result.startDate.implyComponent("mday",  ref.get_mDay());
        result.startDate.implyComponent("wday",  ref.get_wDay());

        // result.tags indicate that this parser was used
        return result;
    }
};




