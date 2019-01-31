/*
 *          Author: Patrick Kyoyetera
 *                  2019
 */


#ifndef ENMONTHNAMEMIDDLEENDIAN_PARSER
#define ENMONTHNAMEMIDDLEENDIAN_PARSER

// ##include "src/parsers/parsers.hpp"
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
    ENMonthNameMiddleEndianParser() : Parser(false, std::regex(PATTERN, std::regex::icase)) { }

    parse::ParsedResult extract(std::string tx, std::smatch match, posix_time::ptime &ref, long idx)
    override {
        std::string text = match.str(0).substr(match.length(1));
        // long idx = match.position(0) + match.length(1);

        parse::ParsedResult result = parse::ParsedResult(ref, idx, text);
        // gregorian::date d;

        short unsigned month = utils::MONTH_CONSTANTS[match.str(MONTH_NAME_GROUP)];

	    short unsigned day;
        match.str(DATE_NUM_GROUP).empty() ? day = utils::argToOrdinalValue(match.str(DATE_GROUP)) :
                                            day = std::stoi(match.str(DATE_NUM_GROUP));

        // sanity check
        if (month==0 or month>12 or day==0 or day>31)
            return result;

        std::string year_s{"-1"};
        int year{-1};
        if (!match.str(YEAR_GROUP).empty() or !match.str(YEAR_GROUP2).empty()) {
            if (!match.str(YEAR_GROUP).empty())
                year_s = match.str(YEAR_GROUP);
            if (!match.str(YEAR_GROUP2).empty())
                year_s = match.str(YEAR_GROUP2);

            try {
                year = std::stoi(year_s);
            }
            catch (std::invalid_argument& e) {
                std::cerr << e.what() << std::endl;
            }
            catch (std::out_of_range& x) {
                std::cerr << x.what() << std::endl;
            }

            std::string yearBE;
            if (!match.str(YEAR_BE_GROUP).empty())
                yearBE = match.str(YEAR_BE_GROUP);
            if (!match.str(YEAR_BE_GROUP2).empty())
                yearBE = match.str(YEAR_BE_GROUP2);

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

            result.startDate.set_mDay(day);
            result.startDate.setMonth(month);
            result.startDate.setYear(dRef.year());
        }

        // Weekday component
        if (!match.str(WEEKDAY__GROUP).empty()) {
            int weekday_int = utils::WEEKDAY_OFFSET[utils::toLowerCase(match.str(WEEKDAY__GROUP))];
            result.startDate.set_wDay(weekday_int);
        }

        // if text was a range value like 'Dec 25 - 26'
        if(!match.str(DATE_TO_GROUP).empty()) {
            int endDate;
            match.str(DATE_TO_NUM_GROUP).empty() ?
                endDate = utils::argToOrdinalValue(match.str(DATE_TO_NUM_GROUP)) :
                endDate = std::stoi(match.str(DATE_TO_NUM_GROUP));

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