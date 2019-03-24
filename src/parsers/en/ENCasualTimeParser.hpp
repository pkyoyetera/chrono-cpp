/**
 * Author: Patrick Kyoyetera
 *              2019
 */
#pragma once

#include "src/parsers/parsers.hpp"

namespace time{ namespace parser {
    class ENCasualTimeParser : public Parser {

    public:
        ENCasualTimeParser() = default;

        std::regex getPattern() const override {
            static const std::regex PATTERN =
                    std::regex(R"((\b))"
                                   R"(((this)?\s*(morning|afternoon|evening|noon|night)))"
                               R"((\b))", std::regex::icase);
            return PATTERN;
        }

        parse::ParsedResult extract(std::string&, const std::smatch& match, const posix_time::ptime& ref, long idx)
        override {
            std::string text = match.str(FULL_MATCH);
            parse::ParsedResult result = parse::ParsedResult(ref, idx, text);

            unsigned time_match = 4;
            if (match.str(time_match).empty()) time_match = 3;
            // todo: clean up above lines

            std::string subs{utils::toLowerCase(match.str(time_match))};

            if(!subs.compare("afternoon")) {
                result.startDate.implyComponent("hour", 15);
            }
            else if(!subs.compare("evening") or !subs.compare("night")) {
                result.startDate.implyComponent("hour", 20);
                // time
            }
            else if(!subs.compare("morning")) {
                result.startDate.implyComponent("hour", 6);
            }
            else if(!subs.compare("noon")) {
                result.startDate.implyComponent("hour", 12);
            }

            result.startDate.implyComponent("year", ref.date().year());
            result.startDate.implyComponent("month", ref.date().month());
            result.startDate.implyComponent("mday", ref.date().day());
            result.startDate.implyComponent("wday", ref.date().day_of_week());
            result.startDate.implyComponent("min", ref.time_of_day().minutes());

            // indicate parser used
            result.setTag(utils::ENCasualTimeParser);

            return result;
        }
    };
} }
