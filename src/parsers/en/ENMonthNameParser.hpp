
/*
 *          Author: Patrick Kyoyetera
 *                  2019
 */

#ifndef ENMONTHNAMEPARSER_HPP
#define ENMONTHNAMEPARSER_HPP

#include "src/parsers/parsers.hpp"

#define MONTH__NAME_GROUP 2
#define YEAR__GROUP       3
#define YEAR_BE_GROUP_    4

#define PATTERN "(^|\\D\\s+|[^\\w\\s])(Jan\\.?|January|Feb\\.?|February|Mar\\.?|March|" \
                "Apr\\.?|April|May\\.?|Jun\\.?|June|Jul\\.?|July|Aug\\.?|August|Sep\\.?|" \
                "Sept\\.?|September|Oct\\.?|October|Nov\\.?|November|Dec\\.?|December)\\s*" \
                "(?:[,-]?\\s*([0-9]{4})(\\s*BE|AD|BC)?)?(?=[^\\s\\w]|\\s+[^0-9]|\\s+$|$)"


class ENMonthNameParser : public Parser {
public:
    ENMonthNameParser() : Parser(false, std::regex(PATTERN, std::regex::icase)) { }

    parse::ParsedResult extract(std::string tx, std::smatch match, posix_time::ptime &ref, long idx)
    override {
        std::string text = match.str(0).substr(match.length(1));
        // long idx = match.position(0) + match.length(1);
        parse::ParsedResult result = parse::ParsedResult(ref, idx, text);

        int month = utils::MONTH_CONSTANTS[match.str(MONTH__NAME_GROUP)];
        unsigned day = 1;

        int year{-1};
        if(!match.str(YEAR__GROUP).empty()) {
            try {
                year = std::stoi(match[YEAR__GROUP].str());
            }
            catch(std::out_of_range& e) {
                std::cerr << e.what() << " at ENMonthNameParser" << std::endl;
            }
            catch(std::invalid_argument& x) {
                std::cerr << x.what() << " at ENMonthNameParser" << std::endl;
            }

            if(!match.str(YEAR_BE_GROUP_).empty()) {
                if (std::regex_search(match.str(YEAR_BE_GROUP_), std::regex("BE", std::regex::icase))) {
                    // Buddhist era
                    year -= 543;
                } else if (std::regex_search(match.str(YEAR_BE_GROUP_), std::regex("BC", std::regex::icase))) {
                    // Before Christ
                    year = -year;
                }
            } else if (year < 100) {
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


#undef MONTH__NAME_GROUP
#undef YEAR__GROUP
#undef YEAR_BE_GROUP_
#undef PATTERN

#endif