#pragma once

#include "src/parsers/parsers.hpp"

 namespace parser {
    class ENDeadlineFormatParser : public Parser {
    private:

    public:
        ENDeadlineFormatParser() = default;

        std::regex getPattern() const override {
            static const std::regex PATTERN {
                    R"((\b))"
                        R"((within|in)\s+)"
                            R"(()"
                                R"((?:one|two|three|four|five|six|seven|eight|nine|ten|eleven|twelve)|)"
                                R"([0-9]+|)"
                                R"(an?(?:\s+few)?|)"
                                R"(half(?:\s+an?)?)"
                            R"())"
                            R"(\s+(sec(?:ond)?s?|min(?:ute)?s?|h(?:ou)?rs?|days?|w(?:ee)?ks?|months?|y(?:ea)?rs?))"
                    R"((\b))", std::regex::icase};
            return PATTERN;
        }

        parse::ParsedResult extract(std::string&, const std::smatch& match, const posix_time::ptime& ref, long idx)
        override {
            std::string text = match.str(FULL_MATCH);
            posix_time::ptime tmpTime{ref};
            gregorian::date tmpDate{ref.date()};

            parse::ParsedResult result = parse::ParsedResult(ref, idx, text);

            std::string tmp = match.str(3);
            float num;
            // check if integer is in integer_words
            if(utils::INTEGER_WORDS.find(tmp) != utils::INTEGER_WORDS.end()) {
                num = utils::INTEGER_WORDS[tmp];
            }
            else if(!tmp.compare("a") || !tmp.compare("an")) {
                num = 1;
            }
            else if(tmp.find("few") != std::string::npos) {
                num = 3;
            }
            else if(tmp.find("half") != std::string::npos) {
                num = 0.5;
            }
            else {
                try {
                    num = std::stoi(tmp);
                }
                catch (std::logic_error& e) {
                    std::cerr << e.what() << std::endl;
                    return result;
                }
            }

            /* boost::date_time functions deal with long ints,
             * so to support half (when num == 0.5) use lower time field
             * i.e: instead of hours(0.5) use minutes(30) e.t.c
             */
            std::regex dwmy(("day|week|month|year"));
            if(std::regex_search(match.str(4), dwmy)) {
                if(match.str(4).find("day") != std::string::npos) {
                    if(floor(num) == num) {
                        tmpDate += gregorian::days(num);
                    }
                    else {
                        tmpTime += posix_time::hours(12);
                        // can't add hours(12) to date object, so use tmptime,
                        // set everything here and return
                        result.startDate.implyComponent("year", tmpTime.date().year());
                        result.startDate.implyComponent("month", tmpTime.date().month());
                        result.startDate.implyComponent("mday", tmpTime.date().day());
                        result.startDate.implyComponent("hour", tmpTime.time_of_day().hours());
                        result.startDate.implyComponent("min", tmpTime.time_of_day().minutes());

                        return result;
                    }
                }
                else if(match.str(4).find("week") != std::string::npos) {
                    if(floor(num) == num) {
                        tmpDate += gregorian::weeks(num);
                    }
                    else {
                        tmpDate += gregorian::days(3);
                    }
                }
                else if(match.str(4).find("month") != std::string::npos) {
                    if(floor(num) == num) {
                        tmpDate += gregorian::months(num);
                    }
                    else{
                        tmpDate += gregorian::days(15);
                    }
                }
                else if(match.str(4).find("year") != std::string::npos) {
                    if(floor(num) == num) {
                        tmpDate += gregorian::years(num);
                    }
                    else {
                        tmpDate += gregorian::months(6);
                    }
                }

                result.startDate.implyComponent("year", tmpDate.year());
                result.startDate.implyComponent("month", tmpDate.month());
                result.startDate.implyComponent("mday", tmpDate.day());

                return result;
            }

            if(!match.str(4).find("hour")) {
                if(floor(num) == num) {
                    tmpTime += posix_time::hours(static_cast<int>(num));
                }
                else {
                    tmpTime += posix_time::minutes(30);
                }
            }
            else if(!match.str(4).find("minute")) {
                if(floor(num) == num)
                    tmpTime += posix_time::minutes(static_cast<int>(num));
                else
                    tmpTime += posix_time::seconds(30);
            } else if (!match.str(4).find("second")) {
                tmpTime += posix_time::seconds(static_cast<int>(ceil(num)));
            }
            result.startDate.implyComponent("year", tmpDate.year());
            result.startDate.implyComponent("month", tmpDate.month());
            result.startDate.implyComponent("mday", tmpDate.day());
            result.startDate.implyComponent("hour", tmpTime.time_of_day().hours());
            result.startDate.implyComponent("min", tmpTime.time_of_day().minutes());
            result.startDate.implyComponent("sec", tmpTime.time_of_day().seconds());

            result.setTag(utils::ENDeadlineFormatParser);

            return result;
        }

    };
}