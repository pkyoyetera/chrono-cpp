#ifndef RESULT_H
#define RESULT_H

#include <iostream>
#include <ctime>
#include <unordered_map>
#include <string>
#include "src/utils/utils.hpp"

typedef std::unordered_map<std::string, std::pair<bool, int> > Components;

namespace parse {
    class ParsedComponents;
    class ParsedResult;
}

class parse::ParsedComponents {
private:
    Components knownValues;
    Components impliedValues;

public:
    ParsedComponents();
    explicit ParsedComponents(const Components& );
    ParsedComponents(Components&, Components&);
    ParsedComponents(const ParsedComponents&);
    ~ParsedComponents();
    ParsedComponents& operator=(const ParsedComponents& );

    void implyComponent(std::string, int);
    bool isCertain(std::string);
    bool isPossibleDate();

    int getYear();
    int getMonth();
    int get_mDay();
    int get_wDay();
    int getHour();
    int getMinute();
    int getSeconds();
    // int getDaylightSavings();

    void setYear(int);
    void setMonth(int);
    void set_mDay(int);
    void set_wDay(int);
    void setHour(int);
    void setMinute(int);
    void setSeconds(int);
    // void setTimeZone(int);
};


class parse::ParsedResult {
protected:
    posix_time::ptime anchor;
    unsigned index;
    std::string text; /// todo: add accessor for text to aid testing
    utils::Tags tags;

public:
    ParsedResult();
    ParsedResult(posix_time::ptime, unsigned, std::string);
    ParsedResult(const ParsedResult& pr);
    ~ParsedResult();

    ParsedComponents startDate, endDate;    // todo: make protected & make accessors and mutators

    bool isEmpty() const;
    void setTag(utils::Modifiers);
    bool hasPossibleDates();
    std::string toDate();
    unsigned getIndex() const;
    size_t textLength() const;
    parse::ParsedResult& operator=(const parse::ParsedResult&);
};

typedef std::vector<parse::ParsedResult> Result;

#endif
