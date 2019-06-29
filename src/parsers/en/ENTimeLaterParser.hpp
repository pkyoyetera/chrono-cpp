
#pragma once

#include "src/parsers/parsers.hpp"

 namespace parser {
    class ENTimeLaterParser : public Parser {
    private:

    public:
        ENTimeLaterParser() = default;

    std::regex getPattern() const override {
        static const std::regex PATTERN {
            R"((\b))"
                R"(((?:)"
                    R"(((?:one|two|three|four|five|six|seven|eight|nine|ten|eleven|twelve)|)"
                    R"([0-9]+)"
                    R"(|an?(?:\s*few)?|half(?:\s+an?)?)"
                R"(")\s+)"
                R"((sec(?:onds?)?|min(?:ute)?s?|hours?|weeks?|days?|months?|years?)\\s*)+))"
                    R"((?:later|after|from now|henceforth|forward|out))"
            R"((\b))", std::regex::icase};
        return PATTERN;
    }

        parse::ParsedResult extract(std::string tx, std::smatch match, posix_time::ptime &ref, long idx)
        override {
            std::string text = match.str(FULL_MATCH).substr(match.length(1));
            parse::ParsedResult result = parse::ParsedResult(ref, idx, text);

            if (match.position(0) > 0 and std::regex_match(text.substr(match.position(0) - 1), std::regex("\\w")))
                return result;

            std::map<std::string, float> fragments = utils::extractDateTimeUnitFragments(match.str(2));

            // tricky here turning the floats to ints
            gregorian::date date{ref.date()};
            posix_time::ptime date_t{ref};

            // add each of the elements in fragments to the date/ptime object
            for(auto a : fragments) {
                if(a.first == "year") {
                    date += gregorian::years(static_cast<int> (a.second));
                }
                else if(a.first == "month") {
                    date += gregorian::months(static_cast<int> (a.second));
                }
                else if(a.first == "week") {            // this did not mean a literal week but rather a weekday
                    date += gregorian::weeks(a.second);
                }
                else if(a.first == "day") {
                    date += gregorian::days(static_cast<int> (a.second)); // you sure you wanna cast these bad boys?
                }
                else if(a.first == "hour") {
                    date_t += posix_time::hours(static_cast<int> (a.second));
                }
                else if(a.first == "minute") {
                    date_t += posix_time::minutes(static_cast<int> (a.second));
                }
                else if(a.first == "second") {
                    date_t += posix_time::seconds(static_cast<int> (a.second));
                }
            }

            if(fragments["hour"] > 0 or fragments["minute"] > 0 or fragments["second"] > 0) {
                result.startDate.setHour(date_t.time_of_day().hours());
                result.startDate.setMinute(date_t.time_of_day().minutes());
                result.startDate.setSeconds(date_t.time_of_day().seconds());
            }
            if(fragments["day"] > 0 or fragments["month"] > 0 or
                fragments["year"] > 0 or fragments["week"] > 0) {
                result.startDate.set_mDay(date.day());
                result.startDate.setMonth(date.month());
                result.startDate.setYear(date.year());
            }
            else {
                /*if (fragments["week"] > 0) {
                    result.startDate.implyComponent("wday", date.day_of_week());
                }*/
                result.startDate.implyComponent("mday", date_t.date().day());
                result.startDate.implyComponent("month", date_t.date().month());
                result.startDate.implyComponent("year", date_t.date().year());
            }

            result.setTag(utils::ENTimeLaterParser);
            return result;
        }
    };

}