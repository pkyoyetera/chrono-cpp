
#pragma once

#include "src/parsers/parsers.hpp"

namespace time { namespace parser {
    class ENRelativeDateFormatParser : public Parser {
    private:
        static const unsigned short MODIFIER_WORD_GROUP   = 2;
        static const unsigned short MULTIPLIER_WORD_GROUP = 3;
        static const unsigned short RELATIVE_WORD_GROUP   = 4;
        static const unsigned short DAYS_OF_WEEK          = 6;

    public:
        ENRelativeDateFormatParser() = default;

        std::regex getPattern() const override {
            static const std::regex PATTERN = std::regex(
                    R"((\b)(this|next|last|past)\s*( (?:one|two|three|four|five|six|seven|)"
                    R"(eight|nine|ten|eleven|twelve)|[0-9]+|few|half(?:\s*an?)?)?\s*(seconds?|)"
                    R"(min(?:ute)?s?|hour(?:s)?|day(?:s)?|week(?:s)?|month(?:s)?|year(?:s)?)(?=\s*))"
                    R"((\b))", std::regex::icase);

            return PATTERN;
        }

        parse::ParsedResult extract(std::string &, const std::smatch &match, const posix_time::ptime &ref, long idx)
        override {
            std::string text = match.str(FULL_MATCH);
            posix_time::ptime tmpTime{ref};
            gregorian::date tmpDate{ref.date()};
            parse::ParsedResult result(ref, idx, text);
            std::regex dwmy(("day|week|month|year"), std::regex::icase);

            std::string num_label{match.str(MULTIPLIER_WORD_GROUP)};
            float num{-1};

            if(utils::INTEGER_WORDS.find(num_label) != utils::INTEGER_WORDS.end()) {
                num = utils::INTEGER_WORDS[num_label];
            }
            else if(num_label.empty()) {
                num = 1;
            }
            else if(num_label.find("few") != std::string::npos) {
                num = 3;
            }
            else if(num_label.find("half") != std::string::npos) {
                num = 0.5;
            }
            else {
                try {
                    num = std::stof(num_label);
                }
                catch (std::logic_error &l) {
                    std::cerr << l.what() << " :at ENRelativeDateFormatParser" << std::endl;
                    return result;
                }
            }

            // this ...
            if (!match.str(MODIFIER_WORD_GROUP).compare("this")) {
                if (!match.str(MULTIPLIER_WORD_GROUP).empty()) {
                    // "this three week" don't make no sense, return
                    return result;
                }
                result.setTag(utils::ENRelativeDateFormatParser);

                if (std::regex_search(match.str(RELATIVE_WORD_GROUP), dwmy)) {
                    // this week
                    if (match.str(RELATIVE_WORD_GROUP).find("week") != std::string::npos) {
                        short unsigned curr_dow = ref.date().day_of_week();
                        short unsigned diff_back = curr_dow;
                        short unsigned diff_forward = DAYS_OF_WEEK - curr_dow;

                        gregorian::date first_dow = ref.date() - gregorian::days(diff_back),
                                last_dow = ref.date() + gregorian::days(diff_forward);

                        result.startDate.implyComponent("mday", first_dow.day());
                        result.startDate.implyComponent("month", first_dow.month());
                        result.startDate.implyComponent("year", first_dow.year());
                        result.endDate.implyComponent("mday", last_dow.day());
                        result.endDate.implyComponent("month", last_dow.month());
                        result.endDate.implyComponent("year", last_dow.year());
                        result.makeEndDateValid();
                    }
                        // this month
                    else if (match.str(RELATIVE_WORD_GROUP).find("month") != std::string::npos) {
                        short last_day = gregorian::gregorian_calendar::end_of_month_day(
                                ref.date().year(), ref.date().month());
                        result.startDate.implyComponent("mday", 1);

                        result.startDate.setMonth(ref.date().month());
                        result.startDate.setYear(ref.date().year());
                        result.endDate.implyComponent("mday", last_day);
                        result.endDate.setMonth(ref.date().month());
                        result.endDate.setYear(ref.date().year());
                        result.makeEndDateValid();
                        /* we know the exact month and year in the situation above
                           so that's why we set them right away */
                    }
                        // this year
                    else if (match.str(RELATIVE_WORD_GROUP).find("year") != std::string::npos) {
                        result.startDate.implyComponent("mday", 1);
                        result.startDate.implyComponent("month", 1);
                        result.startDate.setYear(ref.date().year());
                        result.endDate.implyComponent("mday", 31);
                        result.endDate.implyComponent("month", 12);
                        result.endDate.setYear(ref.date().year());
                        // we know the year in this situation so just set it
                        result.makeEndDateValid();
                    }
                }
                return result;
            }

            // not this
            if (std::regex_search(match.str(RELATIVE_WORD_GROUP), dwmy)) {
                result.setTag(utils::ENRelativeDateFormatParser);
                // posix_time::ptime tmpTime{ref};
                /* NOTE: Since we used num as a float to support use of "few"
                 *       I'll check if it's a fraction and use the lower denomination
                 *       e.g: for half a day, since boost supports only whole numbers,
                 *       you'll see posix_time::hours(12) instead of gregorian::days(0.5).
                 *       It's unnecessarily long but I haven't figured out a way around it
                 */
                if (match.str(RELATIVE_WORD_GROUP).find("day") != std::string::npos) {
                    if (std::floor(num) == num) {
                        if (!match.str(MODIFIER_WORD_GROUP).compare("next")) {
                            tmpDate += gregorian::days(static_cast<int>(num));
                        } else {
                            tmpDate -= gregorian::days(static_cast<int>(num));
                        }
                    } else {
                        if (!match.str(MODIFIER_WORD_GROUP).compare("next")) {
                            tmpTime += posix_time::hours(12);
                        } else {
                            tmpTime -= posix_time::hours(12);
                        }
                        // use tmptime set everything here and return
                        result.startDate.implyComponent("year", tmpTime.date().year());
                        result.startDate.implyComponent("month", tmpTime.date().month());
                        result.startDate.implyComponent("mday", tmpTime.date().day());
                        result.startDate.implyComponent("hour", tmpTime.time_of_day().hours());
                        result.startDate.implyComponent("min", tmpTime.time_of_day().minutes());

                        return result;
                    }
                }
                    // weeks
                else if (match.str(RELATIVE_WORD_GROUP).find("week") != std::string::npos) {
                    if (std::floor(num) == num) {
                        if (!match.str(MODIFIER_WORD_GROUP).compare("next")) {
                            tmpDate += gregorian::weeks(static_cast<int>(num));
                        } else {
                            tmpDate -= gregorian::weeks(static_cast<int>(num));
                        }
                    } else {
                        if (!match.str(MODIFIER_WORD_GROUP).compare("next")) {
                            tmpDate += gregorian::days(3);
                        } else {
                            tmpDate -= gregorian::days(3);
                        }
                    }
                }
                    // months
                else if (match.str(RELATIVE_WORD_GROUP).find("month") != std::string::npos) {
                    if (std::floor(num) == num) {
                        if (!match.str(MODIFIER_WORD_GROUP).compare("next")) {
                            tmpDate += gregorian::months(static_cast<int>(num));
                        } else {
                            tmpDate -= gregorian::months(static_cast<int>(num));
                        }
                    } else {
                        if (!match.str(MODIFIER_WORD_GROUP).compare("next")) {
                            tmpDate += gregorian::days(15);
                        } else {
                            tmpDate -= gregorian::days(15);
                        }
                    }
                }
                    // years
                else if (match.str(RELATIVE_WORD_GROUP).find("year") != std::string::npos) {
                    if (std::floor(num) == num) {
                        if (!match.str(MODIFIER_WORD_GROUP).compare("next")) {
                            tmpDate += gregorian::years(static_cast<int>(num));
                        } else {
                            tmpDate -= gregorian::years(static_cast<int>(num));
                        }
                    } else {
                        if (!match.str(MODIFIER_WORD_GROUP).compare("next")) {
                            tmpDate += gregorian::months(6);
                        } else {
                            tmpDate -= gregorian::months(6);
                        }
                    }
                }
                result.startDate.implyComponent("year", tmpDate.year());
                result.startDate.implyComponent("month", tmpDate.month());
                result.startDate.implyComponent("mday", tmpDate.day());

                return result;
            }

            // time
            // starting with hours
            if (match.str(RELATIVE_WORD_GROUP).find("hour") != std::string::npos) {
                if (std::floor(num) == num) {
                    if (!match.str(MODIFIER_WORD_GROUP).compare("next")) {
                        tmpTime += posix_time::hours(static_cast<int>(num));
                    } else {
                        tmpTime -= posix_time::hours(static_cast<int>(num));
                    }
                } else {
                    if (!match.str(MODIFIER_WORD_GROUP).compare("next")) {
                        tmpTime += posix_time::minutes(30);
                    } else {
                        tmpTime -= posix_time::minutes(30);
                    }
                }
            } else if (match.str(RELATIVE_WORD_GROUP).find("minute") != std::string::npos) {
                if (std::floor(num) == num) {
                    if (!match.str(MODIFIER_WORD_GROUP).compare("next")) {
                        tmpTime += posix_time::minutes(static_cast<int>(num));
                    } else {
                        tmpTime -= posix_time::minutes(static_cast<int>(num));
                    }
                } else {
                    if (!match.str(MODIFIER_WORD_GROUP).compare("next")) {
                        tmpTime += posix_time::seconds(30);
                    } else {
                        tmpTime -= posix_time::seconds(30);
                    }
                }
            } else if (match.str(RELATIVE_WORD_GROUP).find("second") != std::string::npos) {
                if (!match.str(MODIFIER_WORD_GROUP).compare("next")) {
                    tmpTime += posix_time::seconds(static_cast<int>(num));
                } else {
                    tmpTime -= posix_time::seconds(static_cast<int>(num));
                }
            }

            result.startDate.implyComponent("year", tmpTime.date().year());
            result.startDate.implyComponent("month", tmpTime.date().month());
            result.startDate.implyComponent("mday", tmpTime.date().day());
            result.startDate.implyComponent("hour", tmpTime.time_of_day().hours());
            result.startDate.implyComponent("min", tmpTime.time_of_day().minutes());
            result.startDate.implyComponent("sec", tmpTime.time_of_day().seconds());

            result.setTag(utils::ENRelativeDateFormatParser);
            return result;
        }
    };

} }


