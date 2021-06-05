#pragma once

#include <ctime>
#include <iostream>
#include <string>
#include <unordered_map>

#include "src/utils/utils.hpp"

typedef std::unordered_map<std::string, std::pair<bool, int> > Components;

namespace parse {
    class ParsedComponents;
    class ParsedResult;
    ParsedComponents isAfter(ParsedComponents& );
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
    posix_time::ptime date();

    int getYear();
    int getMonth();
    int get_mDay();
    int get_wDay();
    int getHour();
    int getMinute();
    int getSeconds();
    int getTimeZoneOffset();
    // int getDaylightSavings();

    void setYear(int);
    void setMonth(int);
    void set_mDay(int);
    void set_wDay(int);
    void setHour(int);
    void setMinute(int);
    void setSeconds(int);
    void setTimeZoneOffset(int);

};


