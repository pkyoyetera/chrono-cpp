/*
 *          Author: Patrick Kyoyetera
 *                  2019
 */

#pragma once

#include "src/parsers/parsers.hpp"

namespace time { namespace parser {

    class ENMonthNameMiddleEndianParser : public Parser {
    private:

        static const unsigned short  WEEKDAY__GROUP   = 2;
        static const unsigned short  MONTH_NAME_GROUP = 3;
        static const unsigned short  DATE_GROUP       = 4;
        static const unsigned short  DATE_NUM_GROUP   = 5;
        static const unsigned short  DATE_TO_GROUP    = 6;
        static const unsigned short  DATE_TO_NUM_GROUP= 7;
        static const unsigned short  YEAR_GROUP       = 8;
        static const unsigned short  YEAR_BE_GROUP    = 9;
        static const unsigned short  YEAR_GROUP2      = 10;
        static const unsigned short  YEAR_BE_GROUP2   = 11;

    public:
        ENMonthNameMiddleEndianParser() = default;

        std::regex getPattern() const override {
            std::regex PATTERN = std::regex(R"((\W|^)(?:(?:on\\s*?)?(Sunday|Monday|Tuesday|Wednesday|Thursday|Friday|)" \
                R"(Saturday|Sun\.?|Mon\.?|Tue\.?|Wed\.?|Thu\.?|Fri\.?|Sat\.?)\s*,?\s*)?)" \
                R"((Jan\.?|January|Feb\.?|February|Mar\.?|March|Apr\.?|April|May\.?|Jun\.?|)" \
                R"(June|Jul\.?|July|Aug\.?|August|Sep\.?|Sept\.?|September|Oct\.?|October|)" \
                R"(Nov\.?|November|Dec\.?|December)(?:-|\/|\s*,?\s*)(([0-9]{1,2}))" \
                R"((?:st|nd|rd|th)?|eighteenth|eighth|eleventh|fifteenth|fifth|first|fourteenth|)" \
                R"(fourth|nineteenth|ninth|second|seventeenth|seventh|sixteenth|sixth|tenth|)" \
                R"(third|thirteenth|thirtieth|thirty[ -]first|twelfth|twentieth|twenty[ -]eighth|)" \
                R"(twenty[ -]fifth|twenty[ -]first|twenty[ -]fourth|twenty[ -]ninth|)" \
                R"(twenty[ -]second|twenty[ -]seventh|twenty[ -]sixth|twenty[ -]third))" \
                R"((?!\s*(?:am|pm))\s*(?:(?:to|\-)\s*(([0-9]{1,2})(?:st|nd|rd|th)?|eighteenth)" \
                R"(|eighth|eleventh|fifteenth|fifth|first|fourteenth|fourth|nineteenth|ninth|second|)" \
                R"(seventeenth|seventh|sixteenth|sixth|tenth|third|thirteenth|thirtieth|thirty[ -]first|)" \
                R"(twelfth|twentieth|twenty[ -]eighth|twenty[ -]fifth|twenty[ -]first|twenty[ -]fourth|)" \
                R"(twenty[ -]ninth|twenty[ -]second|twenty[ -]seventh|twenty[ -]sixth|twenty[ -]third)\s*)?)" \
                R"((?:(?:-|\/|\s*,?\s*)(?:([0-9]{4})\s*(BE|AD|BC)?|([0-9]{1,4})\s*(AD|BC)))" \
                R"(\s*)?(?=\W|$)(?!\:\d))" , std::regex::icase);

            return PATTERN;
        }

        parse::ParsedResult extract(std::string&, const std::smatch& match, const posix_time::ptime& ref, long idx)
        override {
            std::string text = match.str(0).substr(match.length(1));
            parse::ParsedResult result = parse::ParsedResult(ref, idx, text);

            short unsigned month = utils::MONTH_CONSTANTS[match.str(MONTH_NAME_GROUP)];
            short unsigned day = match.str(DATE_NUM_GROUP).empty() ?
                                            utils::argToOrdinalValue(match.str(DATE_GROUP)) :
                                            std::stoi(match.str(DATE_NUM_GROUP));

            // sanity check
            if (month == 0 or month > 12 or day == 0 or day > 31) {
                return result;
            }

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
                catch (std::logic_error& e) {
                    std::cerr << e.what() << " at ENMonthNameMiddleEndianParser()" << std::endl;
                    return result;
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

            if (year != -1) {
                result.startDate.set_mDay(day);
                result.startDate.setMonth(month);
                result.startDate.setYear(year);
            } else {
                gregorian::date dRef{ref.date().year(), month, day};
                gregorian::date prevY{dRef - gregorian::years(1)},
                        nextY{dRef + gregorian::years(1)};

                if (std::abs((nextY - ref.date()).days()) < std::abs((dRef - ref.date()).days())) {
                    // compare the durations returned
                    dRef = nextY;
                } else if (std::abs((prevY - ref.date()).days()) < std::abs((dRef - ref.date()).days())) {
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
            if (!match.str(DATE_TO_GROUP).empty()) {
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

} }
