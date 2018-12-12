
#include "result.h"
#include <ctime>

using namespace parse;
using namespace std;


/*
components& time_components::operator=(const time_components& tc) {
    timezone     = tc.timezone;
    year         = tc.year;
    month        = tc.month;
    day          = tc.day;
    hour         = tc.hour;
    minute       = tc.minute;
    second       = tc.second;
}
*/

ParsedComponents::ParsedComponents() {
    /*
    for(auto val: knownValues) {
        val->second().first() = false;
        val->second().second() = 0;
    }

    for(auto val: impliedValues) {
        val->second().first() = false;
        val->second().second() = 0;
    }

    */
    knownValues.insert({"year",  {false, 0}});
    knownValues.insert({"month", {false, 0}});
    knownValues.insert({"mday",  {false, 0}});      // day of the month
    knownValues.insert({"wday",  {false, 0}});      // day of the week
    // knownValues.insert({"dls",   {false, 0}});      // daylight savings
    knownValues.insert({"hour",  {false, 0}});
    knownValues.insert({"min",   {false, 0}});
    knownValues.insert({"sec",   {false, 0}});

}

ParsedComponents::ParsedComponents(Components& comp) {
    knownValues = comp;
}

ParsedComponents::ParsedComponents(Components& cmp, Components& anchor) {
    knownValues = cmp;

    set_mDay(anchor["mday"].second);
    set_wDay(anchor["wday"].second);
    setMonth(anchor["month"].second + 1);
    setYear(anchor["year"].second);

    setHour(anchor["hour"].second);
    setMinute(anchor["min"].second);
    setSeconds(anchor["sec"].second);
}

ParsedComponents::~ParsedComponents() = default;

ParsedComponents::ParsedComponents(const ParsedComponents& pc) {
    knownValues   = pc.knownValues;
    impliedValues = pc.impliedValues;
}

int ParsedComponents::getYear() {
     if (knownValues["year"].first)
         return knownValues["year"].second;
     else
         return impliedValues["year"].second;
}

int ParsedComponents::getMonth() {
    if(knownValues["month"].first)
        return knownValues["month"].second;
    else
        return impliedValues["month"].second;
}

int ParsedComponents::get_mDay() {
    if(knownValues["mday"].first)
        return knownValues["mday"].second;
    else
        return impliedValues["mday"].second;
}

int ParsedComponents::get_wDay() {
    if(knownValues["wday"].first)
        return knownValues["wday"].second;
    else
        return impliedValues["wday"].second;
}

int ParsedComponents::getMinute() {
    if(knownValues["min"].first)
        return knownValues["min"].second;
    else
        return impliedValues["min"].second;
}

int ParsedComponents::getSeconds() {
    if(knownValues["sec"].first)
        return knownValues["sec"].second;
    else
        return impliedValues["sec"].second;
}

/*
int ParsedComponents::getDaylightSavings() {
    if (knownValues["dls"].first)
        return knownValues["dls"].second;
    else
        return impliedValues["dls"].second;
}
*/

int ParsedComponents::getHour() {
    if(knownValues["hour"].first)
        return knownValues["hour"].second;
    else
        return impliedValues["hour"].second;
}

void ParsedComponents::setYear(int year) {
    knownValues["year"].first    = true;
    knownValues["year"].second   = year;
    impliedValues["year"].first  = false;
    impliedValues["year"].second = 0;
}

void ParsedComponents::setMonth(int month) {
    knownValues["month"].first    = true;
    knownValues["month"].second   = month;
    impliedValues["month"].first  = false;
    impliedValues["month"].second = 0;
}

void ParsedComponents::set_mDay(int day) {
    knownValues["mday"].first    = true;
    knownValues["mday"].second   = day;
    impliedValues["mday"].first  = false;
    impliedValues["mday"].second = 0;
}

void ParsedComponents::set_wDay(int day) {
    knownValues["wday"].first    = true;
    knownValues["wday"].second   = day;
    impliedValues["wday"].first  = false;
    impliedValues["wday"].second = 0;
}

void ParsedComponents::setHour(int hour) {
    knownValues["hour"].first    = true;
    knownValues["hour"].second   = hour;
    impliedValues["hour"].first  = false;
    impliedValues["hour"].second = 0;
}

void ParsedComponents::setMinute(int min) {
    knownValues["min"].first    = true;
    knownValues["min"].second   = min;
    impliedValues["min"].first  = false;
    impliedValues["min"].second = 0;
}

void ParsedComponents::setSeconds(int sec) {
    knownValues["sec"].first    = true;
    knownValues["sec"].second   = sec;
    impliedValues["sec"].first  = false;
    impliedValues["sec"].second = 0;
}
/*
void ParsedComponents::setTimeZone(int tz) {
    knownValues.timezone   = tz;
    impliedValues.timezone = -1;
}
*/

void ParsedComponents::implyComponent(std::string comp, int value) {
    /**
     * @param comp: string name of component to check
     * @param value: value to assign to component
     * @return null
     * */
    if(!comp.compare("year")) {
        if (!knownValues["year"].first) {
            impliedValues["year"].first = true;
            impliedValues["year"].second = value;
        }
        return;
    }
    else if (!comp.compare("month")) {
        if (!knownValues["month"].first) {
            impliedValues["month"].first = true;
            impliedValues["month"].second = value;
        }
        return;
    }
    else if(!comp.compare("mday")) {
        if (!knownValues["mday"].first) {
            impliedValues["mday"].first = true;
            impliedValues["mday"].second = value;
        }
        return;
    }
    else if(!comp.compare("wday")) {
        if (!knownValues["wday"].first) {
            impliedValues["wday"].first = true;
            impliedValues["wday"].second = value;
        }
        return;
    }
    else if (!comp.compare("hour")) {
        if (!knownValues["hour"].first) {
            impliedValues["hour"].first = true;
            impliedValues["hour"].second = value;
        }
        return;
    }
    else if(!comp.compare("min")) {
        if (!knownValues["min"].first) {
            impliedValues["min"].first = true;
            impliedValues["min"].second = value;
        }
        return;
    }
    else if(!comp.compare("sec")) {
        if (!knownValues["sec"].first) {
            impliedValues["sec"].first = true;
            impliedValues["sec"].second = value;
        }
        return;
    }
/* ue for daylight savings
    else if(!comp.compare("timezone")) {
        if(knownValues.timezone == -1)
            impliedValues.timezone = value;
        return;
    }
*/
    else
        std::cerr << "Invalid option: " << comp << std::endl;

    return;
}

bool ParsedComponents::isCertain(std::string comp) {
    // also requires implementation
    if (!comp.compare("year"))
        return knownValues["year"].first;

    else if(!comp.compare("month") )
            return knownValues["month"].first;

    else if(!comp.compare("mday"))
        return knownValues["mday"].first;

    else if(!comp.compare("wday"))
        return knownValues["wday"].first;

    else if(!comp.compare("hour"))
        return knownValues["hour"].first;

    else if(!comp.compare("min"))
        return knownValues["minute"].first;

    else if(!comp.compare("sec"))
        return knownValues["sec"].first;

    /*
    else if(!comp.compare("timezone"))
        if(knownValues.timezone != -1)
            return true;
    */
    else std::cerr << "Invalid component: " << comp << std::endl;

    return false;
}

bool ParsedComponents::isPossibleDate() {
    // needs implementation
    std::time_t tmp = std::time(nullptr);
    std::tm* tmp1 = std::gmtime(&tmp);

    if(tmp1->tm_year != getYear())    return false;
    if(tmp1->tm_mon  != getMonth()-1) return false;
    if(tmp1->tm_mday != get_mDay())   return false;
    if(tmp1->tm_wday != get_wDay())   return false;
    if(tmp1->tm_hour != getHour())    return false;
    if(tmp1->tm_min  != getMinute())  return false;

    return true;
}

/***************************************************************************************/

ParsedResult::ParsedResult() {
    index = 0;
    text = "";

    ParsedComponents tmp;
    startDate = tmp;
    endDate = tmp;

    /*
    for(auto val: startDate.knownValues) {
        val->second().first() = false;
        val->second().second() = 0;
    }

    for(auto val: startDate.impliedValues) {
        val->second().first() = false;
        val->second().second() = 0;
    }

    for(auto val: endDate.knownValues) {
        val->second().first() = false;
        val->second().second() = 0;
    }

    for(auto val: endDate.impliedValues) {
        val->second().first() = false;
        val->second().second() = 0;
    }
    */
}

ParsedResult::ParsedResult(ParsedComponents an, unsigned idx, std::string tx) { /*default_tags tg,, ParsedComponents st*/
    anchor = an;
    index = idx;
    text = tx;

    ParsedComponents tmp;
    startDate = tmp;
    endDate = tmp;
}

// need to implement use of endDate
ParsedResult::ParsedResult(const ParsedResult& pr) {
    anchor = pr.anchor;
    index = pr.index;
    text = pr.text;
    startDate = pr.startDate;
    endDate = pr.endDate;
}

bool ParsedResult::hasPossibleDates() {
    return startDate.isPossibleDate() or endDate.isPossibleDate();
}

ParsedResult::~ParsedResult() { }

std::string ParsedResult::toDate() {
    struct tm date;

    date.tm_year = startDate.getYear();
    date.tm_mon  = startDate.getMonth();
    date.tm_mday = startDate.get_mDay();
    date.tm_wday = startDate.get_wDay();
    date.tm_hour = startDate.getHour();
    date.tm_min  = startDate.getMinute();
    date.tm_isdst = 1;
    date.tm_gmtoff = -5*60*60;      // default for Eastern Standard Time

    std::cout << asctime(&date);
    // time_t t = mktime(&date);
    return asctime(&date);
}