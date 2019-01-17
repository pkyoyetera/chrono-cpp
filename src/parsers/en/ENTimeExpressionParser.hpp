
#ifndef ENTIMEEXPRESSIONPARSER_HPP
#define ENTIMEEXPRESSIONPARSER_HPP

#define HOUR_GROUP         2
#define MINUTE_GROUP       3
#define SECOND_GROUP       4
#define MILLI_SECOND_GROUP 5
#define AM_PM_HOUR_GROUP   6

// psst...what even are trigraphs?
#define PATTERN "(^|\\s|T)(?:(?:at|from)\\s*)?" \
                "?(\\d{1,4}|noon|midnight)(?:(?:\\.|\\:|\\：)(\\d{1,2})(?:(?:\\:|\\：)" \
                "(\\d{2})(?:\\.(\\d{1,6}))?)?)?(?:\\s*(A\\.M\\.?|P\\.M\\.?|AM?|PM?|" \
                "O\\W*CLOCK))?(?=\\W|$)"

#include "src/parsers/parsers.hpp"


class ENTimeExpressionParser : public Parser {
public:
    ENTimeExpressionParser() : Parser(false, std::regex(PATTERN, std::regex::icase)) { }

    parse::ParsedResult extract(std::string tx, std::smatch match, posix_time::ptime& ref) override {
        std::string text = match[0].str().substr(match[1].length());
        int idx = match.position() + match[1].length();

        parse::ParsedResult result = parse::ParsedResult(ref, idx, text);

        if (match.position() > 0 and std::regex_match(tx.substr(match.position() - 1), std::regex("\\w")))
            return result;

        int hour{0}, minute{0}, tz{-1}, meridian{-1};

        //------------Seconds------------------
        if(!match[SECOND_GROUP].str().empty()){
            short second = std::stoi(match[SECOND_GROUP].str());
            if(second >= 60) return result;

            result.startDate.setSeconds(second);
        }

        //--------------Hour------------------
        if(match[HOUR_GROUP].str() == "noon"){
            // meridian = 1;
            hour = 12;
        } else if (match[HOUR_GROUP].str() == "midnight") {
            // meridian = 0;
            hour = 0;
        } else {
            hour = std::stoi(match[HOUR_GROUP].str());
        }

        //------------Minutes---------------
        if(!match[MINUTE_GROUP].str().empty()){
            minute = std::stoi(match[MINUTE_GROUP].str());
        } else if(hour > 100) {
            minute = hour%100;
            hour   = hour/100;
        }

        if(minute >= 60) {
            return result;
        }

        if(hour > 24) {
            return result;
        }
        /*if (hour >= 12) {
            meridian = 1;
        }*/

        // ----- AM & PM
        if(!match[AM_PM_HOUR_GROUP].str().empty()) {
            if(hour > 12) return result;
            char& ampm = match[AM_PM_HOUR_GROUP].str()[0];
            if(ampm == 'a'){
                //meridian = 0;
                if(hour == 12) hour = 0;
            }

            if(ampm == 'p'){
                //meridian = 1;
                if(hour != 12) hour += 12;
            }
        }

        /*  // commented out because I do not use the meridian
        if (meridiem >= 0) {
            result.start.assign('meridiem', meridiem);
        } else {
            if (hour < 12) {
                result.start.imply('meridiem', 0);
            } else {
                result.start.imply('meridiem', 1);
            }
        }
        */

        result.startDate.setHour(hour);
        result.startDate.setMinute(minute);

        result.startDate.implyComponent("mday", ref.date().day());
        result.startDate.implyComponent("month", ref.date().month());
        result.startDate.implyComponent("year", ref.date().year());

        result.setTag(utils::ENTimeExpressionParser);
        return result;
    }
};


#undef HOUR_GROUP
#undef MINUTE_GROUP
#undef SECOND_GROUP
#undef MILLI_SECOND_GROUP
#undef AM_PM_HOUR_GROUP

#undef PATTERN

#endif

