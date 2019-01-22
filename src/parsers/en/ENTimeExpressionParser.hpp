
#ifndef ENTIMEEXPRESSIONPARSER_HPP
#define ENTIMEEXPRESSIONPARSER_HPP

#define HOUR_GROUP         2
#define MINUTE_GROUP       3
#define SECOND_GROUP       4
#define MILLI_SECOND_GROUP 5
#define AM_PM_HOUR_GROUP   6
#define TO_GROUP           7
#define TO_HOUR_GROUP      10
#define TO_MINUTE_GROUP    11
#define TO_SECOND_GROUP    12
#define TO_MERIDIEM_GROUP  14


// psst...what even are trigraphs?
#define PATTERN "(^|\\s|T)(?:(?:at|from)\\s*)?" \
                "?(\\d{1,4}|noon|midnight)(?:(?:\\.|\\:|\\：)(\\d{1,2})(?:(?:\\:|\\：)" \
                "(\\d{2})(?:\\.(\\d{1,6}))?)?)?(?:\\s*(A\\.M\\.?|P\\.M\\.?|AM?|PM?|" \
                "O\\W*CLOCK))?(?=\\W|$)" \
                " ?((^|\\s*)(\\-|\\–|\\~|\\〜|to|\\?)\\s*(\\d{1,4})(?:(?:\\.|\\:|\\：)" \
                "(\\d{1,2})(?:(?:\\.|\\:|\\：)(\\d{1,2})(?:\\.(\\d{1,6}))?)?)?(?:\\s*" \
                "(A\\.M\\.?|P\\.M\\.?|AM?|PM?|O\\W*CLOCK))?(?=\\W|$))?"

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

        int hour{0}, minute{0};// tz{-1}, meridian{-1};
        char ampm_s;

        //------------Seconds------------------
        if(!match[SECOND_GROUP].str().empty()){
            short second = static_cast<short>(std::stoi(match[SECOND_GROUP].str()));
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

        // ----- AM & PM
        if(!match[AM_PM_HOUR_GROUP].str().empty()) {
            // if the text has am/pm provided then the logic follows
            // that the hour should not be greater than 12
            if(hour > 12) return result;
            ampm_s = match[AM_PM_HOUR_GROUP].str()[0];
            if(ampm_s == 'a'){
                //meridian = 0;
                if(hour == 12) hour = 0;
            }

            if(ampm_s == 'p'){
                //meridian = 1;
                if(hour != 12) hour += 12;
            }
        }

        /*  // no use for meridian yet
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

        /************************************************************
        //               if there exists a "to" expression          //
        //                e.g: from 5 a.m to 5 p.m                  //
        *************************************************************/
        if(!match[TO_GROUP].str().empty()) {
            if (std::regex_match(match[TO_GROUP].str(), std::regex(R"(^\s*(\+|\-)\s*\d{3,4}$)"))) {
                return result;
            }
            // "to" portion of match
            int to_hour{0}, to_minute{0}; //, to_tz{-1};

            if(match[TO_HOUR_GROUP].str() == "noon"){
                to_hour = 12;
            } else if (match[TO_HOUR_GROUP].str() == "midnight") {
                to_hour = 0;
            } else {
                to_hour = std::stoi(match[TO_HOUR_GROUP].str());
            }

            if(!match[TO_MINUTE_GROUP].str().empty()){
                to_minute = std::stoi(match[TO_MINUTE_GROUP].str());
            } else if(to_hour > 100) {
                to_minute = to_hour%100;
                to_hour   = to_hour/100;
            }

            if(to_minute >= 60 or to_hour > 24) {
                return result;
            }

            if(!match[TO_MERIDIEM_GROUP].str().empty()) {
                // if the text has am/pm provided then the logic follows
                // that the hour should not be greater than 12
                if(to_hour > 12)
                    return result;

                char& ampm = match[TO_MERIDIEM_GROUP].str()[0];
                if(ampm == 'a')
                    if(to_hour == 12)
                        to_hour = 0;

                if(ampm == 'p') {
                    if (to_hour != 12)
                        to_hour += 12;
                    if(match[AM_PM_HOUR_GROUP].str().empty()) {
                        if (hour != 12) {
                            hour += 12;
                            result.startDate.setHour(hour);
                        }
                    }
                }
            } else {
                // imply the meridiem if it wasn't provided on the to clause
                if(ampm_s == 'a')
                    if(to_hour == 12)
                        to_hour = 0;
                if(ampm_s == 'p')
                    if(to_hour != 12)
                        to_hour += 12;
            }

            if(!match[TO_SECOND_GROUP].str().empty()){
                int to_second = std::stoi(match[TO_SECOND_GROUP].str());
                if(to_second >= 60 or to_second < 0)
                    return result;

                result.endDate.setSeconds(to_second);
            }
            result.makeEndDateValid();
            result.endDate.setHour(to_hour);
            result.endDate.setMinute(to_minute);

            result.endDate.implyComponent("mday", ref.date().day());
            result.endDate.implyComponent("month", ref.date().month());
            result.endDate.implyComponent("year", ref.date().year());
        }
        return result;
    }
};

// todo: if miridiem is provided on only one part of the range,
// it follows that the other should correspond to the meridiem
// e.g: 4-6pm: 16:00 - 18:00 not 4:00 - 18:00

#undef HOUR_GROUP
#undef MINUTE_GROUP
#undef SECOND_GROUP
#undef MILLI_SECOND_GROUP
#undef AM_PM_HOUR_GROUP

#undef TO_SECOND_GROUP
#undef TO_MINUTE_GROUP
#undef TO_MERIDIEM_GROUP
#undef TO_HOUR_GROUP
#undef TO_GROUP
#undef PATTERN

#endif

