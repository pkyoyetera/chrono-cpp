#ifndef ENDEADLINEFORMATPARSER_HPP
#define ENDEADLINEFORMATPARSER_HPP

#include "src/parsers/parsers.h"

#define PATTERN "(\\W|^)(within|in)\\s*((?:one|two|three|four|five|six|seven|eight|nine|ten|eleven|twelve)|[0-9]+|an?(?:\\s*few)?|half(?:\\s*an?)?)\\s*(seconds?|min(?:ute)?s?|hour(?:s)?|days?|weeks?|months?|years?)\\s*(?=\\W|$)"


class ENDeadlineFormatParser : public Parser {
public:
    ENDeadlineFormatParser() : Parser(false, std::regex(PATTERN, std::regex::icase)) { }

    parse::ParsedResult extract(std::string tx, std::smatch match, posix_time::ptime& ref) {
        std::string text = match[0].str().substr(match[1].length());
        int idx = match.position(0) + match[1].length();
        posix_time::ptime tmpTime{ref};
        gregorian::date tmpDate{ ref.date() };

        parse::ParsedResult result = parse::ParsedResult(ref, idx, text);

        std::string tmp = match[3];
        float num;
        // check if integer is in integer_words
        if(utils::INTEGER_WORDS.find(tmp) != utils::INTEGER_WORDS.end()) {
            num = utils::INTEGER_WORDS[tmp];
        }
        else if(!tmp.compare("a") || !tmp.compare("an")) {
            num = 1;
        }
        else if(!tmp.compare("few")) {
            num = 3;
        }
        else if(tmp.find("half") != std::string::npos) {
            num = 0.5;
        }
        else {
            try {
                num = std::stoi(tmp);
            }
            catch (std::invalid_argument e) {
                std::cerr << e.what() << std::endl;
            }
            catch (std::out_of_range x) {
                std::cerr << x.what() << std::endl;
            }
        }

        /* boost::date_time functions deal with long ints,
         * so to support half (when num == 0.5) use lower time field
         * i.e: instead of hours(0.5) use minutes(30) e.t.c
         */
        std::regex dwmy(("day|week|month|year"));
        if( std::regex_search(match[4].str(), dwmy) ) {
            if(!match[4].str().find("day")) {
                if(floor(num) == num)
                    tmpDate += gregorian::days(num);
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
            else if (!match[4].str().find("week")) {
                if(floor(num) == num)
                    tmpDate += gregorian::weeks(num);
                else
                    tmpDate += gregorian::days(3);
            }
            else if(!match[4].str().find("month")) {
                if(floor(num) == num)
                    tmpDate += gregorian::months(num);
                else
                    tmpDate += gregorian::days(15);
            }
            else if(!match[4].str().find("year")) {
                if(floor(num) == num)
                    tmpDate += gregorian::years(num);
                else
                    tmpDate += gregorian::months(6);
            }

            result.startDate.implyComponent("year", tmpDate.year());
            result.startDate.implyComponent("month", tmpDate.month());
            result.startDate.implyComponent("mday", tmpDate.day());

            return result;
        }

        if(!match[4].str().find("hour")) {

            if(floor(num) == num)
                tmpTime += posix_time::hours(num);
            else
                tmpTime += posix_time::minutes(30);
        }
        else if(!match[4].str().find("minute")) {
            if(floor(num) == num)
                tmpTime += posix_time::minutes(num);
            else
                tmpTime += posix_time::seconds(30);
        }
        else if(!match[4].str().find("second")) {
            tmpTime += posix_time::seconds(ceil(num));
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

#undef PATTERN
#endif