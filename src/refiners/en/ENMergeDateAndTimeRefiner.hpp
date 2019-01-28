

#include "src/refiners/refiner.hpp"

class ENMergeDateAndTime : public Refiner {
private:
    std::regex pattern{"(T|at|on|of|,|-)?", std::regex::icase};

public:
    ENMergeDateAndTime( ) = default;
    ~ENMergeDateAndTime() = default;

    bool isDateOnly(parse::ParsedResult& r) {
        return !r.startDate.isCertain("hour");
    }

    bool isTimeOnly(parse::ParsedResult& r) {
        return !r.startDate.isCertain("month") and !r.startDate.isCertain("mday");
    }

    bool isAbleToMerge(std::string text, parse::ParsedResult& prev, parse::ParsedResult& curr) {
        std::string textBetween = text.substr(prev.getIndex() + prev.textLength(), curr.getIndex());
        return std::regex_search(textBetween, pattern);
    }

    parse::ParsedComponents mergeDateTimeComponent(parse::ParsedComponents dateComp,
                                                   parse::ParsedComponents timeComp) {
        /**
         * @brief: merge a date only and time only component into one date-time result
         * @param dateComp: the date only component
         * @param timeComp: the time only component
         * @return combined date-time component
         */
        parse::ParsedComponents dateTimeComp{dateComp};

        if (timeComp.isCertain("hour")) {
            dateTimeComp.setHour(timeComp.getHour());
            dateTimeComp.setMinute(timeComp.getMinute());

            if (timeComp.isCertain("sec"))
                dateTimeComp.setSeconds(timeComp.getSeconds());
            else
                dateTimeComp.implyComponent("sec", timeComp.getSeconds());
        } else {
            dateTimeComp.implyComponent("hour", timeComp.getHour());
            dateTimeComp.implyComponent("min", timeComp.getMinute());
            dateTimeComp.implyComponent("sec", timeComp.getSeconds());
        }
        return dateTimeComp;
    }

    parse::ParsedResult mergeResult(std::string& text,
            parse::ParsedResult dateResult, parse::ParsedResult timeResult) {
        /**
         * @brief: merge a date and time result
         * @param text: the whole input text
         * @param dateResult: a date only result
         * @param timeResult: a time only result
         */
        parse::ParsedComponents startDate{dateResult.startDate};
        parse::ParsedComponents startTime{timeResult.startDate};
        parse::ParsedComponents startDateTime{mergeDateTimeComponent(startDate, startTime)};

        if (dateResult.end() or timeResult.end()) {
            parse::ParsedComponents endDate, endTime, endDateTime;

            dateResult.end() ? endDate = dateResult.endDate : endDate = dateResult.startDate;
            timeResult.end() ? endTime = timeResult.endDate : endTime = timeResult.startDate;

            endDateTime = mergeDateTimeComponent(endDate, endTime);

            if (!dateResult.end() and endDateTime.date() < startDateTime.date()) {
                // Ex. 9pm - 1am
                if (endDateTime.isCertain("mday"))
                    endDateTime.set_mDay(endDateTime.get_mDay() + 1);
                else
                    endDateTime.implyComponent("mday", endDateTime.get_mDay() + 1);
            }
            dateResult.endDate = endDateTime;
            dateResult.makeEndDateValid();
        }

        dateResult.startDate = startDateTime;

        long startIndex = std::min(dateResult.getIndex(),  timeResult.getIndex());
        long endIndex   = std::max(dateResult.getIndex() + dateResult.textLength(),
                                   timeResult.getIndex() + timeResult.textLength());
        dateResult.setIndex(startIndex);
        dateResult.setText(text.substr(startIndex, endIndex - startIndex));

        for (auto &mod: utils::ModList) {
            if (timeResult.getTag(mod))
                dateResult.setTag(mod);
        }
        dateResult.setTag(utils::ENMergeDateAndTimeRefiner);
        return dateResult;
    }

    Result refine(Result r, std::string t) override {
        /**
         * @brief Merge a date-only result and a time-only result into one combined result
         * @param r: list of results from parsers
         * @param t: complete original text
         * @return list of parse::ParsedResults
         */
        if (r.size() < 2) // no second component to merge
            return r;

        parse::ParsedResult prev, curr, tmp;
        Result merged_result;

        for (unsigned i=1; i<r.size(); i++) {
            curr = r[i];
            prev = r[i-1];

            if (isDateOnly(prev) and isTimeOnly(curr) and isAbleToMerge(t, prev, curr)) {
                prev = mergeResult(t, prev, curr);
                try {
                    curr = r.at(i+1);
                }
                catch (std::out_of_range e) {
                    std::cerr << e.what() << " at ENMergeDateAndTimeRefiner" << std::endl;
                }
                i++;
            }
            else if (isDateOnly(curr) and isTimeOnly(prev) and isAbleToMerge(t, prev, curr)) {
                prev = mergeResult(t, curr, prev);
                try {
                    curr = r.at(i+1);
                }
                catch (std::out_of_range e) {
                    std::cerr << e.what() << " at ENMergeDateAndTimeRefiner" << std::endl;
                }
                i++;
            }
            merged_result.push_back(prev);
        }
        if (!curr.isEmpty())
            merged_result.push_back(curr);

        return merged_result;
    }
};