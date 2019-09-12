
#pragma once

#include "src/parsers/parsers.hpp"


 namespace parser {

    class ENISOFormatParser : public Parser {
    private:
        static const unsigned short YEAR_NUMBER_GROUP        = 2,
                                    MONTH_NUMBER_GROUP       = 3,
                                    DATE_NUMBER_GROUP        = 4,
                                    HOUR_NUMBER_GROUP        = 5,
                                    MINUTE_NUMBER_GROUP      = 6,
                                    SECOND_NUMBER_GROUP      = 7,
                                    // MILLISECOND_NUMBER_GROUP = 8,
                                    TZD_HOUR_OFFSET_GROUP    = 9,
                                    TZD_MINUTE_OFFSET_GROUP  = 10;

        public:
        ENISOFormatParser()= default;

        std::regex getPattern() const override {
            static const std::regex PATTERN {
                    R"((\b))"
                        R"(([0-9]{4})\-([0-9]{1,2})\-([0-9]{1,2})(?:T([0-9]{1,2}):([0-9]{1,2}))"
                            R"((?::([0-9]{1,2})(?:\.(\d{1,4}))?)?(?:Z|([+-]\d{2}):?(\d{2})?)?)?)"
                    R"((\b))", std::regex::icase};
            return PATTERN;
        }

        parse::ParsedResult extract(std::string&, const std::smatch& match, const posix_time::ptime& ref, long idx)
        override {
            std::string text = match.str(FULL_MATCH);
            parse::ParsedResult result = parse::ParsedResult(ref, idx, text);

            int month = std::stoi(match.str(MONTH_NUMBER_GROUP));
            int day = std::stoi(match.str(DATE_NUMBER_GROUP));
            if(month > 12 or month < 1 or day > 31 or day < 1) {
                return result;
            }

            result.startDate.setYear(std::stoi(match.str(YEAR_NUMBER_GROUP)));
            result.startDate.setMonth(month);
            result.startDate.set_mDay(day);

            if(!match.str(HOUR_NUMBER_GROUP).empty()) {
                result.startDate.setHour(std::stoi(match.str(HOUR_NUMBER_GROUP)));
                result.startDate.setMinute(std::stoi(match.str(MINUTE_NUMBER_GROUP)));

                if(!match.str(SECOND_NUMBER_GROUP).empty()) {
                    result.startDate.setSeconds(std::stoi(match.str(SECOND_NUMBER_GROUP)));
                }

                if(match.str(TZD_HOUR_OFFSET_GROUP).empty()) {
                    result.startDate.setTimeZoneOffset(0);
                }
                else {
                    int minuteOffset = 0;
                    int hourOffset = std::stoi(match.str(TZD_HOUR_OFFSET_GROUP));
                    if(!match.str(TZD_MINUTE_OFFSET_GROUP).empty()) {
                        minuteOffset = std::stoi(match.str(TZD_MINUTE_OFFSET_GROUP));
                    }

                    int offset = hourOffset * 60;
                    if(offset < 0) {
                        offset -= minuteOffset;
                    }
                    else {
                        offset += minuteOffset;
                    }
                    result.startDate.setTimeZoneOffset(offset);
                }
            }
            result.setTag(utils::ENISOFormatParser);
            return result;
        }
    };
}