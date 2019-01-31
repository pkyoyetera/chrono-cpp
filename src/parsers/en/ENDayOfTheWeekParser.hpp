#ifndef ENDATEOFWEEKPARSER_HPP
#define ENDATEOFWEEKPARSER_HPP

#include "src/parsers/parsers.hpp"

#define TIMING_GROUP  2
#define PREFIX_GROUP  3
#define WEEKDAY_GROUP 4
#define POSTFIX_GROUP 5
#define WEEK_GROUP    6

#define PATTERN "(\\W|^)(earl(?:ier|y)|late)?(?:\\s*(this|last|next)\\s*)?" \
                "(Sunday|Sun|Monday|Mon|Tuesday|Tues|Tue|Wednesday|Wed|" \
                "Thursday|Thu(?:rs|r)?|Friday|Fri|Saturday|Sat)(?:\\s*" \
                "(this|last|next)\\s*)?(?:(week)\\s*)?(?=\\W|$)"
/* part of pattern (on\s*)?*/

/**
 * @brief: relative day_of_week pattern parser
 * Works with first day of the week being Monday
 * */
class ENDayOfWeekParser : public Parser {

public:
    ENDayOfWeekParser() : Parser(false, std::regex(PATTERN, std::regex::icase)) { }

    parse::ParsedResult updateParsedComponent(parse::ParsedResult res, posix_time::ptime& ref,
            short unsigned offset, std::string modifier, std::string timing) {

        gregorian::date resOffset, start{ref.date()};
        bool start_fixed{false};
        // int count = 0;
        gregorian::first_day_of_the_week_before fdbf{offset};
        gregorian::first_day_of_the_week_after  fdaf{offset};

        if(!modifier.compare("last") or !modifier.compare("past")) {
            gregorian::date tmp = start - gregorian::weeks(1);
            if(offset < start.day_of_week().as_number()) {
                resOffset = fdbf.get_date(tmp);
            }
            else if(offset > start.day_of_week().as_number()) {
                resOffset = fdaf.get_date(tmp);
            }
            else
                resOffset = tmp;
            start_fixed = true;
        }
        else if(!modifier.compare("next")) {
            gregorian::date tmp = start + gregorian::weeks(1);
            if(offset < start.day_of_week().as_number()) {
                resOffset = fdbf.get_date(tmp);
            }
            else if(offset > start.day_of_week().as_number()) {
                resOffset = fdaf.get_date(tmp);
            }
            else
                resOffset = tmp;
            start_fixed = true;

        }
        else if(!modifier.compare("this") or modifier.empty()) {
            if(offset == ref.date().day_of_week().as_number()){
                resOffset = start;
            }
            else if(offset < ref.date().day_of_week().as_number() ) {
                resOffset = fdbf.get_date(ref.date());
            }
            else {
                resOffset = fdaf.get_date(ref.date());
            }
        }
        else {
            // modifier is probably a day_of_week
            // create date with day_of_week = modifier within ref.week_number()
            int daySpace = start.day_of_week() - utils::WEEKDAY_OFFSET[modifier];
            if(daySpace == 0)
                resOffset = start;
            else {
                if(daySpace < 0) {
                    // day is after ref day
                    gregorian::first_day_of_the_week_after fda{utils::WEEKDAY_OFFSET[modifier]};
                    resOffset = fda.get_date(start);
                }
                else {
                    // day is before ref day
                    gregorian::first_day_of_the_week_before fdb{utils::WEEKDAY_OFFSET[modifier]};
                    resOffset = fdb.get_date(start);
                }
            }
        }

        if(resOffset.is_special()) {
            return res;
        }
        else if(start_fixed){
            res.startDate.set_mDay(resOffset.day());
            res.startDate.setMonth(resOffset.month());
            res.startDate.setYear(resOffset.year());
        }
        else {
            res.startDate.implyComponent("mday", resOffset.day());
            res.startDate.implyComponent("month", resOffset.month());
            res.startDate.implyComponent("year", resOffset.year());
        }
        if(timing == "earlier" or timing == "early") {
            res.startDate.implyComponent("hour", 6);
            res.startDate.implyComponent("min", 1);
        }
        else if(timing == "late") {
            res.startDate.implyComponent("hour", 20);
            res.startDate.implyComponent("min", 1);
        }

        res.setTag(utils::ENDayOfTheWeekParser);

        return res;
    }

    parse::ParsedResult extract(std::string tx, std::smatch match, posix_time::ptime& ref, long idx)
    override {
        std::string text      = match.str(0).substr(match.length(1)),
                    timing    = match.str(TIMING_GROUP),
                    prefix    = match.str(PREFIX_GROUP),
                    postfix   = match.str(POSTFIX_GROUP),
                    dayOfWeek = match.str(WEEKDAY_GROUP),
                    week      = match.str(WEEK_GROUP),
                    norm;
        // long idx = match.position(0) + match.length(1);

        parse::ParsedResult result = parse::ParsedResult(ref, idx, text);

	    short unsigned offset = utils::WEEKDAY_OFFSET[dayOfWeek];
        if(offset < 0 or offset > 6) {
            return result;
        }

        if(prefix.empty())
            norm = postfix;
        else if(postfix.empty())
            norm = prefix;
        else
            norm = "";

        return updateParsedComponent(result, ref, offset, norm, timing);
    }
};

#undef TIMING_GROUP
#undef PATTERN
#undef PREFIX_GROUP
#undef WEEKDAY_GROUP
#undef POSTFIX_GROUP
#undef WEEK_GROUP


#endif