
/*
 *          Author: Patrick Kyoyetera
 *                  2019
 */
#pragma once

#include "src/parsers/parsers.hpp"

 namespace parse {

    class ENMonthNameParser : public Parser {
    private:

        static const unsigned short MONTH__NAME_GROUP = 2;
        static const unsigned short YEAR__GROUP       = 3;
        static const unsigned short YEAR_BE_GROUP_    = 4;

    public:
        ENMonthNameParser() = default;

        std::regex getPattern() const override {
            std::regex PATTERN = std::regex(R"((^|\D\s+|[^\w\s])(Jan\.?|January|Feb\.?|February|Mar\.?|March|)"
                                    R"(Apr\.?|April|May\.?|Jun\\.?|June|Jul\.?|July|Aug\.?|August|Sep\.?|)"
                                    R"(Sept\.?|September|Oct\.?|October|Nov\.?|November|Dec\.?|December)\s*)"
                                    R"((?:[,-]?\s*([0-9]{4})(\s*BE|AD|BC)?)?(?=[^\s\w]|\s+[^0-9]|\s+$|$))",
                                    std::regex::icase);
        }

        parse::ParsedResult extract(std::string&, const std::smatch& match, const posix_time::ptime& ref, long idx)
        override {
            std::string text = match.str(0).substr(match.length(1));
            parse::ParsedResult result = parse::ParsedResult(ref, idx, text);

            short unsigned month = utils::MONTH_CONSTANTS[match.str(MONTH__NAME_GROUP)];
            short unsigned day = 1;

            int year{-1};
            if(!match.str(YEAR__GROUP).empty()) {
                try {
                    year = std::stoi(match[YEAR__GROUP].str());
                }
                catch(std::logic_error& e) {
                    std::cerr << e.what() << " at ENMonthNameParser" << std::endl;
                    return result;
                }

                if(!match.str(YEAR_BE_GROUP_).empty()) {
                    if(std::regex_search(match.str(YEAR_BE_GROUP_), std::regex("BE", std::regex::icase))) {
                        // Buddhist era
                        year -= 543;
                    }
                    else if(std::regex_search(match.str(YEAR_BE_GROUP_), std::regex("BC", std::regex::icase))) {
                        // Before Christ
                        year = -year;
                    }
                }
                else if(year < 100) {
                    year += 2000;
                }
            }
            if(year != -1) {
                result.startDate.implyComponent("mday", day);
                result.startDate.setMonth(month);
                result.startDate.setYear(year);
            }
            else {
                gregorian::date dRef{ref.date().year(), month, day};
                gregorian::date prevY{dRef - gregorian::years(1)},
                        nextY{dRef + gregorian::years(1)};

                if(std::abs((nextY - ref.date()).days()) < std::abs((dRef - ref.date()).days())) {
                    dRef = nextY;
                }
                else if(std::abs((prevY - ref.date()).days()) < std::abs((dRef - ref.date()).days())) {
                    dRef = prevY;
                }

                result.startDate.implyComponent("mday", day);
                result.startDate.setMonth(month);
                result.startDate.implyComponent("year", dRef.year());
            }

            result.setTag(utils::ENMonthNameParser);
            return result;
        }
    };
}