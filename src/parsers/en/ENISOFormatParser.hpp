

#ifndef ENISOFORMATPARSER_HPP
#define ENISOFORMATPARSER_HPP

#define YEAR_NUMBER_GROUP        2
#define MONTH_NUMBER_GROUP       3
#define DATE_NUMBER_GROUP        4
#define HOUR_NUMBER_GROUP        5
#define MINUTE_NUMBER_GROUP      6
#define SECOND_NUMBER_GROUP      7
#define MILLISECOND_NUMBER_GROUP 8
#define TZD_HOUR_OFFSET_GROUP    9
#define TZD_MINUTE_OFFSET_GROUP  10

#define PATTERN "(\\W|^)([0-9]{4})\\-([0-9]{1,2})\\-([0-9]{1,2})(?:T([0-9]{1,2}):([0-9]{1,2})" \
                "(?::([0-9]{1,2})(?:\\.(\\d{1,4}))?)?(?:Z|([+-]\\d{2}):?(\\d{2})?)?)?(?=\\W|$)"

#include "src/parsers/parsers.hpp"

class ENISOFormatParser : public Parser {
public:
    ENISOFormatParser() : Parser(false, std::regex(PATTERN, std::regex::icase)) { }

    parse::ParsedResult extract(std::string tx, std::smatch match, posix_time::ptime& ref) override {
        std::string text = match[0].str().substr(match[1].length());
        int idx = match.position() + match[1].length();

        parse::ParsedResult result = parse::ParsedResult(ref, idx, text);

        int month = std::stoi(match[MONTH_NUMBER_GROUP].str());
        int day = std::stoi(match[DATE_NUMBER_GROUP].str());
        if(month > 12 or month < 1 or day > 31 or day < 1)
            return result;

        result.startDate.setYear(std::stoi(match[YEAR_NUMBER_GROUP].str()));
        result.startDate.setMonth(month);
        result.startDate.set_mDay(day);

        if(!match[HOUR_NUMBER_GROUP].str().empty()) {
            result.startDate.setHour(std::stoi(match[HOUR_NUMBER_GROUP].str()));
            result.startDate.setMinute(std::stoi(match[MINUTE_NUMBER_GROUP].str()));

            if (!match[SECOND_NUMBER_GROUP].str().empty()) {
                result.startDate.setSeconds(std::stoi(match[SECOND_NUMBER_GROUP].str()));
            }

            if (match[TZD_HOUR_OFFSET_GROUP].str().empty()) {

                // result.start.assign('timezoneOffset', 0);
            } else {
                int minuteOffset = 0;
                int hourOffset = std::stoi(match[TZD_HOUR_OFFSET_GROUP].str());
                if(!match[TZD_MINUTE_OFFSET_GROUP].str().empty())
                    minuteOffset = std::stoi(match[TZD_MINUTE_OFFSET_GROUP].str());

                int offset = hourOffset * 60;
                if (offset < 0) {
                    offset -= minuteOffset;
                } else {
                    offset += minuteOffset;
                }
                result.startDate.setTimeZoneOffset(offset);
            }
        }
        result.setTag(utils::ENISOFormatParser);
        return result;
    }

};

#undef YEAR_NUMBER_GROUP
#undef MONTH_NUMBER_GROUP
#undef DATE_NUMBER_GROUP
#undef HOUR_NUMBER_GROUP
#undef MINUTE_NUMBER_GROUP
#undef SECOND_NUMBER_GROUP
#undef MILLISECOND_NUMBER_GROUP
#undef TZD_HOUR_OFFSET_GROUP
#undef TZD_MINUTE_OFFSET_GROUP

#undef PATTERN

#endif
