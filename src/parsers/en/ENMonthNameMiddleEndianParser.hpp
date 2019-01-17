/*
 *          Author: Patrick Kyoyetera
 *                  2019
 */


#ifndef ENMONTHNAMEMIDDLEENDIAN_PARSER
#define ENMONTHNAMEMIDDLEENDIAN_PARSER

#include "src/parsers/parsers.hpp"

#define WEEKDAY__GROUP    2
#define MONTH_NAME_GROUP  3
#define DATE_GROUP        4
#define DATE_NUM_GROUP    5
#define DATE_TO_GROUP     6
#define DATE_TO_NUM_GROUP 7
#define YEAR_GROUP        8
#define YEAR_BE_GROUP     9
#define YEAR_GROUP2       10
#define YEAR_BE_GROUP2    11

#define PATTERN "(\\W|^)(?:(?:on\\s*?)?(Sunday|Monday|Tuesday|Wednesday|Thursday|Friday|" \
                "Saturday|Sun\\.?|Mon\\.?|Tue\\.?|Wed\\.?|Thu\\.?|Fri\\.?|Sat\\.?)\\s*,?\\s*)?" \
                "(Jan\\.?|January|Feb\\.?|February|Mar\\.?|March|Apr\\.?|April|May\\.?|Jun\\.?|" \
                "June|Jul\\.?|July|Aug\\.?|August|Sep\\.?|Sept\\.?|September|Oct\\.?|October|" \
                "Nov\\.?|November|Dec\\.?|December)(?:-|\\/|\\s*,?\\s*)(([0-9]{1,2})" \
                "(?:st|nd|rd|th)?|eighteenth|eighth|eleventh|fifteenth|fifth|first|fourteenth|" \
                "fourth|nineteenth|ninth|second|seventeenth|seventh|sixteenth|sixth|tenth|" \
                "third|thirteenth|thirtieth|thirty[ -]first|twelfth|twentieth|twenty[ -]eighth|" \
                "twenty[ -]fifth|twenty[ -]first|twenty[ -]fourth|twenty[ -]ninth|" \
                "twenty[ -]second|twenty[ -]seventh|twenty[ -]sixth|twenty[ -]third)" \
                "(?!\\s*(?:am|pm))\\s*(?:(?:to|\\-)\\s*(([0-9]{1,2})(?:st|nd|rd|th)?|eighteenth" \
                "|eighth|eleventh|fifteenth|fifth|first|fourteenth|fourth|nineteenth|ninth|second|" \
                "seventeenth|seventh|sixteenth|sixth|tenth|third|thirteenth|thirtieth|thirty[ -]first|" \
                "twelfth|twentieth|twenty[ -]eighth|twenty[ -]fifth|twenty[ -]first|twenty[ -]fourth|" \
                "twenty[ -]ninth|twenty[ -]second|twenty[ -]seventh|twenty[ -]sixth|twenty[ -]third)\\s*)?" \
                "(?:(?:-|\\/|\\s*,?\\s*)(?:([0-9]{4})\\s*(BE|AD|BC)?|([0-9]{1,4})\\s*(AD|BC))" \
                "\\s*)?(?=\\W|$)(?!\\:\\d)"
// #define PATTERN "(?<=\\W|^)(?:(?:Sunday|Monday|Tuesday|Wednesday|Thursday|Friday|Saturday|Sun|Mon|Tue|Wed|Thu|Fri|Sat)\\s*,?\\s*)?(Jan(?:uary\\.)?|Feb(?:ruary|\\.)?|Mar(?:ch|\\.)?|Apr(?:il|\\.)?|May|Jun(?:e|\\.)?|Jul(?:y|\\.)?|Aug(?:ust|\\.)?|Sep(?:tember|\\.)?|Oct(?:ober|\\.)?|Nov(?:ember|\\.)?|Dec(?:ember|\\.)?)\\s*(?:([0-9]{1,2})(?:st|nd|rd|th)?\\s*(?:to|\\-|~)\\s*)?([0-9]{1,2})(?:st|nd|rd|th)?(?:,?(\\s*[0-9]{4})(\\s*BE)?)?(?=\\W|$)"

class ENMonthNameMiddleEndianParser : public Parser {
public:
    ENMonthNameMiddleEndianParser() : Parser(false, std::regex(PATTERN, std::regex::icase)) {}

    parse::ParsedResult extract(std::string tx, std::smatch match, posix_time::ptime &ref) override {

        parse::ParsedResult result;
        // gregorian::date d;

        std::string month_s = match[MONTH_NAME_GROUP];
        short unsigned month = utils::MONTH_CONSTANTS[month_s];

        short unsigned day;
        match[DATE_NUM_GROUP].str().empty() ? day = utils::argToOrdinalValue(match[DATE_GROUP].str()) :
                                              day = std::stoi(match[DATE_NUM_GROUP].str());

        // sanity check
        if (month==0 or day==0)
            return result;

        std::string year_s{"-1"};
        int year{-1};
        if (!match[YEAR_GROUP].str().empty() or !match[YEAR_GROUP2].str().empty()) {
            if (!match[YEAR_GROUP].str().empty())
                year_s = match[YEAR_GROUP];
            if (!match[YEAR_GROUP2].str().empty())
                year_s = match[YEAR_GROUP2];
            year = std::stoi(year_s);

            std::string yearBE;
            if (!match[YEAR_BE_GROUP].str().empty())
                yearBE = match[YEAR_BE_GROUP];
            if (!match[YEAR_BE_GROUP2].str().empty())
                yearBE = match[YEAR_BE_GROUP2];

            if (!yearBE.empty()) {
                if (std::regex_search(yearBE, std::regex("BE", std::regex::icase))) {
                    // Buddhist era
                    year -= 543;
                } else if (std::regex_search(yearBE, std::regex("BC", std::regex::icase))) {
                    // Before Christ
                    year = -year;
                }
            } else if (year < 100) {
                year += 2000;
            }
        }

        if(year != -1) {
            result.startDate.set_mDay(day);
            result.startDate.setMonth(month);
            result.startDate.setYear(year);
        }
        else {
            gregorian::date dRef{ref.date().year(), month, day};
            gregorian::date prevY{dRef - gregorian::years(1)},
                            nextY{dRef + gregorian::years(1)};

            if(std::abs((nextY - ref.date()).days()) < std::abs((dRef - ref.date()).days())) {
                // compare the durations returned
                dRef = nextY;
            }
            else if(std::abs((prevY - ref.date()).days()) < std::abs((dRef - ref.date()).days())) {
                dRef = prevY;
            }
            else {
                // what now?
            }

            result.startDate.set_mDay(day);
            result.startDate.setMonth(month);
            result.startDate.setYear(dRef.year());
        }

        // Weekday component
        if (!match[WEEKDAY__GROUP].str().empty()) {
            std::string weekday = match[WEEKDAY__GROUP].str();
            std::transform(weekday.begin(), weekday.end(), weekday.begin(), ::tolower);
            int weekday_int = utils::WEEKDAY_OFFSET[weekday];
            result.startDate.set_wDay(weekday_int);
        }

        // if text was a range value like 'Dec 25 - 26, 2025'
        if(!match[DATE_TO_GROUP].str().empty()) {
            int endDate;
            match[DATE_TO_NUM_GROUP].str().empty() ?
                endDate = utils::argToOrdinalValue(match[DATE_TO_NUM_GROUP].str()) :
                endDate = std::stoi(match[DATE_TO_NUM_GROUP].str());

            parse::ParsedComponents tmp{result.startDate};
            tmp.set_mDay(endDate);
            tmp.setHour(23);
            tmp.setMinute(59);
            result.endDate = tmp;
            result.makeEndDateValid();
        }

        result.setTag(utils::ENMonthNameMiddleEndianParser);
        return result;
    }
};


#undef WEEKDAY__GROUP
#undef MONTH_NAME_GROUP
#undef DATE_GROUP
#undef DATE_NUM_GROUP
#undef DATE_TO_GROUP
#undef DATE_TO_NUM_GROUP
#undef YEAR_GROUP
#undef YEAR_BE_GROUP
#undef YEAR_GROUP2
#undef YEAR_BE_GROUP2

#undef PATTERN

#endif