
#pragma once

#include "src/parsers/parsers.hpp"

namespace  parser {
class ENTimeExpressionParser : public Parser {
private:
    static const unsigned short HOUR_GROUP        = 2,
                                MINUTE_GROUP      = 3,
                                SECOND_GROUP      = 4,
                                // MILLI_SECOND_GROUP = 5,
                                AM_PM_HOUR_GROUP  = 6,
                                TO_GROUP          = 7,
                                TO_HOUR_GROUP     = 10,
                                TO_MINUTE_GROUP   = 11,
                                TO_SECOND_GROUP   = 12,
                                TO_MERIDIEM_GROUP = 14;

public:
    ENTimeExpressionParser() = default;

    std::regex getPattern() const override {
        // psst...what even are trigraphs?
        static const std::regex PATTERN = std::regex(R"((^|\s|T)(?:(?:at|from)\s*)?)"
                             R"(?(\d{1,4}|noon|midnight)(?:(?:\.|\:|\：)(\d{1,2})(?:(?:\:|\：))"
                             R"((\d{2})(?:\.(\d{1,6}))?)?)?(?:\s*(A\.M\.?|P\.M\.?|AM?|PM?|)"
                             R"(O\W*CLOCK))?(?=\W|$))"
                             R"( ?((^|\s*)(\-|\–|\~|\〜|to|\?)\s*(\d{1,4})(?:(?:\.|\:|\：))"
                             R"((\d{1,2})(?:(?:\.|\:|\：)(\d{1,2})(?:\.(\d{1,6}))?)?)?(?:\s*)"
                             R"((A\.M\.?|P\.M\.?|AM?|PM?|O\W*CLOCK))?(\b))?)", std::regex::icase);
        return PATTERN;
    }

    parse::ParsedResult extract(std::string& tx, const std::smatch& match, const posix_time::ptime& ref, long idx)
    override {
        std::string text = match.str(0).substr(match.length(1));
        parse::ParsedResult result = parse::ParsedResult(ref, idx, text);

        if(match.position(0) > 0 and std::regex_match(tx.substr(match.position(0) - 1), std::regex("\\w"))) {
            return result;
        }

        int hour{0}, minute{0};// tz{-1}, meridian{-1};
        char ampm_s;

        //------------Seconds------------------
        if(!match.str(SECOND_GROUP).empty()) {
            unsigned short second = static_cast<unsigned short>(std::stoi(match.str(SECOND_GROUP)));
            if(second >= 60)
                return result;

            result.startDate.setSeconds(second);
        }

        //--------------Hour------------------
        if(match.str(HOUR_GROUP) == "noon") {
            // meridian = 1;
            hour = 12;
        }
        else if(match.str(HOUR_GROUP) == "midnight") {
            // meridian = 0;
            hour = 0;
        }
        else {
            hour = std::stoi(match.str(HOUR_GROUP));
        }

        //------------Minutes---------------
        if(!match.str(MINUTE_GROUP).empty()) {
            minute = std::stoi(match.str(MINUTE_GROUP));
        }
        else if(hour > 100) {
            minute = hour % 100;
            hour = hour / 100;
        }

        if(minute >= 60) {
            return result;
        }

        if(hour > 24) {
            return result;
        }

        // ----- AM & PM
        if(!match.str(AM_PM_HOUR_GROUP).empty()) {
            // if the text has am/pm provided then the logic follows
            // that the hour should not be greater than 12
            if(hour > 12) return result;
            ampm_s = match.str(AM_PM_HOUR_GROUP)[0];
            if(ampm_s == 'a') {
                //meridian = 0;
                if(hour == 12) hour = 0;
            }

            if(ampm_s == 'p') {
                //meridian = 1;
                if(hour != 12) hour += 12;
            }
        }

        /*  // no use for meridian yet
        if(meridiem >= 0) {
            result.start.assign('meridiem', meridiem);
        } else {
            if(hour < 12) {
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
        if(!match.str(TO_GROUP).empty()) {
            if(std::regex_match(match.str(TO_GROUP), std::regex(R"(^\s*(\+|\-)\s*\d{3,4}$)"))) {
                return result;
            }
            // "to" portion of match
            int to_hour{0}, to_minute{0}; //, to_tz{-1};

            if(match.str(TO_HOUR_GROUP) == "noon") {
                to_hour = 12;
            }
            else if(match.str(TO_HOUR_GROUP) == "midnight") {
                to_hour = 0;
            }
            else {
                to_hour = std::stoi(match.str(TO_HOUR_GROUP));
            }

            if(!match.str(TO_MINUTE_GROUP).empty()) {
                to_minute = std::stoi(match.str(TO_MINUTE_GROUP));
            }
            else if(to_hour > 100) {
                to_minute = to_hour % 100;
                to_hour = to_hour / 100;
            }

            if(to_minute >= 60 or to_hour > 24) {
                return result;
            }

            if(!match.str(TO_MERIDIEM_GROUP).empty()) {
                // if the text has am/pm provided then the logic follows
                // that the hour should not be greater than 12
                if(to_hour > 12)
                    return result;

                char &ampm = match.str(TO_MERIDIEM_GROUP)[0];
                if(ampm == 'a')
                    if(to_hour == 12)
                        to_hour = 0;

                if(ampm == 'p') {
                    if(to_hour != 12)
                        to_hour += 12;
                    if(match.str(AM_PM_HOUR_GROUP).empty()) {
                        if(hour != 12) {
                            hour += 12;
                            result.startDate.setHour(hour);
                        }
                    }
                }
            }
            else {
                // imply the meridiem if it wasn't provided on the to clause
                if(ampm_s == 'a')
                    if(to_hour == 12)
                        to_hour = 0;
                if(ampm_s == 'p')
                    if(to_hour != 12)
                        to_hour += 12;
            }

            if(!match.str(TO_SECOND_GROUP).empty()) {
                int to_second = std::stoi(match.str(TO_SECOND_GROUP));
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

}