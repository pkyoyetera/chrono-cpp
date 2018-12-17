

#ifndef RESULT_H
#define RESULT_H


#include <iostream>
#include <ctime>
#include <unordered_map>
#include <string>
#include "src/utils/utils.hpp"

// #include "../external/date/include/date/date.h"


/*
struct time_components {
    int timezone;
    unsigned month;
    unsigned year;
    unsigned day;
    int      hour;
    int      minute;
    int      second;

    time_components& operator= (const time_components& tc);
};
*/

typedef std::unordered_map<std::string, std::pair<bool, int> > Components;
/* ={
        {"year",  {false, 0}},
        {"month", {false, 0}},
        {"mday",  {false, 0}},      // day of the month
        {"wday",  {false, 0}},      // day of the week
        // {"dls",   {false, 0}},      // daylight savings
        {"hour",  {false, 0}},
        {"min",   {false, 0}},
        {"sec",   {false, 0}}
};
*/


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
    explicit ParsedComponents(Components& );
    ParsedComponents(Components&, Components&);   // we could use std::ctime to convert time_point to time objects
    ParsedComponents(const ParsedComponents&);
    ~ParsedComponents();

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
    std::string text;
    utils::Tags tags;

public:

    ParsedResult();
    ParsedResult(posix_time::ptime, unsigned, std::string /*default_tags, ParsedComponents*/);
    ParsedResult(const ParsedResult& pr);
    ~ParsedResult();

    ParsedComponents startDate;
    ParsedComponents endDate;       // todo: make protected & make accessors and mutators

    // todo: overloaded assignment operator
    void setTag(utils::Modifiers);
    bool hasPossibleDates();
    std::string toDate();
};

#endif
