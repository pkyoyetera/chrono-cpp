/*
 *          Author: Patrick Kyoyetera
 *                  2019
 */

#pragma once

#include "src/parsers/parsers.hpp"

 namespace parser {

    class ENMonthNameMiddleEndianParser : public Parser {
    private:

        static const unsigned short WEEKDAY__GROUP    = 2,
                                    MONTH_NAME_GROUP  = 3,
                                    DATE_GROUP        = 4,
                                    DATE_NUM_GROUP    = 5,
                                    DATE_TO_GROUP     = 6,
                                    DATE_TO_NUM_GROUP = 7,
                                    YEAR_GROUP        = 8,
                                    YEAR_BE_GROUP     = 9,
                                    YEAR_GROUP2       = 10,
                                    YEAR_BE_GROUP2    = 11;

    public:
        ENMonthNameMiddleEndianParser() = default;

        std::regex getPattern() const override {
            std::regex PATTERN {
                    R"((\b))"
                        R"((?:)"
                            R"((?:on\s*?)?)"
                                R"((Sun(?:day)?|Mon(?:day)?|Tue(?:s(?:day)?)?|Wed(?:nesday)?|Thu(?:r(?:s(?:day)?)?)?|Fri(?:day)?|Sat(?:urday)?)\s*,?\s*)?)"
                                    R"((Jan(?:uary|\.)?|Feb(?:ruary|\.)?|Mar(?:ch|\.)?|Apr(?:il|\.)?|May\.?|Jun(?:e|\.)?|)"
                                    R"(Jul(?:\.|y)?|Aug(?:ust|\.)?|Sep(?:\.|t(?:\.|ember)?)?|Oct(?:ober|\.)?|Nov(?:\.|ember)?|Dec(?:ember|\.)?))"
                                        R"((?:-|\/|\s*,?\s*)(([0-9]{1,2}))"
                                            R"((?:st|nd|rd|th)?|)"
                                            R"(first|second|third|fourth|fifth|sixth|seventh|eighth|ninth|tenth|)"
                                            R"(eleventh|twelfth|thirteenth|fourteenth|fifteenth|sixteenth|seventeenth|eighteenth|nineteenth|twentieth|)"
                                            R"(twenty[ -]first|twenty[ -]second|twenty[ -]third|twenty[ -]fourth|twenty[ -]fifth|twenty[ -]sixth|twenty[ -]seventh|twenty[ -]eighth|twenty[ -]ninth|thirtieth)"
                                            R"(|thirty[ -]first)"
                                        R"())"
                                        R"((?!\s*)"
                                        R"((?:am|pm))\s+)"
                            R"((?:)"
                                R"((?:to|\-)\s*)"
                                    R"((([0-9]{1,2}))"
                                        R"((?:st|nd|rd|th)?|)"
                                            R"(first|second|third|fourth|fifth|sixth|seventh|eighth|ninth|tenth|)"
                                            R"(eleventh|twelfth|thirteenth|fourteenth|fifteenth|sixteenth|seventheenth|eighteenth|nineteenth|twentieth|)"
                                            R"(twenty[ -]first|twenty[ -]second|twenty[ -]third|twenty[ -]fourth|twenty[ -]fifth|twenty[ -]sixth|twenty[ -]seventh|twenty[ -]eighth|twenty[ -]ninth|)"
                                            R"(thirtieth|thirty[ -]first)\s*)"
                                        R"()?)"
                                        R"((?:)"
                                            R"((?:-|\/|\s*,?\s*)(?:([0-9]{4})\s*(BE|AD|BC)?|([0-9]{1,4})\s*(AD|BC))\s+)"
                                        R"()?)"
                                    R"((\b)(?!\:\d))" , std::regex::icase};
            return PATTERN;
        }

        parse::ParsedResult extract(std::string&, const std::smatch& match, const posix_time::ptime& ref, long idx)
        override {
            std::string text = match.str(FULL_MATCH);
            parse::ParsedResult result = parse::ParsedResult(ref, idx, text);

            short unsigned month = utils::MONTH_CONSTANTS[match.str(MONTH_NAME_GROUP)];
            short unsigned day = match.str(DATE_NUM_GROUP).empty() ?
                                            utils::argToOrdinalValue(match.str(DATE_GROUP)) :
                                            std::stoi(match.str(DATE_NUM_GROUP));

            // sanity check
            if(month == 0 or month > 12 or day == 0 or day > 31) {
                return result;
            }

            std::string year_s{"-1"};
            int year{-1};
            if(!match.str(YEAR_GROUP).empty() or !match.str(YEAR_GROUP2).empty()) {
                if(!match.str(YEAR_GROUP).empty())
                    year_s = match.str(YEAR_GROUP);
                if(!match.str(YEAR_GROUP2).empty())
                    year_s = match.str(YEAR_GROUP2);

                try {
                    year = std::stoi(year_s);
                }
                catch (std::logic_error& e) {
                    std::cerr << e.what() << " at ENMonthNameMiddleEndianParser()" << std::endl;
                    return result;
                }

                std::string yearBE;
                if(!match.str(YEAR_BE_GROUP).empty())
                    yearBE = match.str(YEAR_BE_GROUP);
                if(!match.str(YEAR_BE_GROUP2).empty())
                    yearBE = match.str(YEAR_BE_GROUP2);

                if(!yearBE.empty()) {
                    if(std::regex_search(yearBE, std::regex("BE", std::regex::icase))) {
                        // Buddhist era
                        year -= 543;
                    }
                    else if(std::regex_search(yearBE, std::regex("BC", std::regex::icase))) {
                        // Before Christ
                        year = -year;
                    }
                }
                else if(year < 100) {
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
            if(!match.str(WEEKDAY__GROUP).empty()) {
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

}