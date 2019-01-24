#include "result.hpp"
#include "src/result.hpp"

#include <ctime>

using namespace parse;
using namespace std;


ParsedComponents::ParsedComponents() {
// todo: turn wday to correspond to gregorian_day_of_week in boost::gregorian
    knownValues.insert({"year",     {false, 0}});
    knownValues.insert({"month",    {false, 0}});
    knownValues.insert({"mday",     {false, 0}});      // day of the month
    knownValues.insert({"wday",     {false, 0}});      // day of the week
    // knownValues.insert({"isdst", {false, 0x0}});      // daylight savings
    knownValues.insert({"hour",     {false, 0}});
    knownValues.insert({"min",      {false, 0}});
    knownValues.insert({"sec",      {false, 0}});
    knownValues.insert({"tzoffset", {false, 0}});
}

ParsedComponents::ParsedComponents(const Components& comp) {
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

    setTimeZoneOffset(anchor["tzoffset"].second);
}

ParsedComponents::~ParsedComponents() = default;

ParsedComponents::ParsedComponents(const ParsedComponents& pc) {
    knownValues   = pc.knownValues;
    impliedValues = pc.impliedValues;
}

ParsedComponents& ParsedComponents::operator=(const ParsedComponents& obj) {
    knownValues   = obj.knownValues;
    impliedValues = obj.impliedValues;

    return *this;
}

posix_time::ptime ParsedComponents::date() {
    tm temp;
    temp.tm_year = getYear();
    temp.tm_mon  = getMonth();
    temp.tm_mday = get_mDay();
    temp.tm_wday = get_wDay();
    temp.tm_hour = getHour();
    temp.tm_min  = getMinute();
    temp.tm_sec  = getSeconds();
    // todo: integrate timezone offset in returned time

    return posix_time::ptime_from_tm(temp);
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

int ParsedComponents::getTimeZoneOffset() {
    if(knownValues["tzoffset"].first)
        return knownValues["tzoffset"].second;
    else
        return impliedValues["tzoffset"].second;
}

/*
int ParsedComponents::getDaylightSavings() {
    if (knownValues["dls"].first)
        return knownValues["dls"].second;
    else
        return impliedValues["dls"].second;
}
*/

int ParsedComponents::getHour()  {
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

void ParsedComponents::setTimeZoneOffset(int tzo) {
    knownValues["tzoffset"].first    = true;
    knownValues["tzoffset"].second   = tzo;
    impliedValues["tzoffset"].first  = false;
    impliedValues["tzoffset"].second = 0;
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
    else if(!comp.compare("tzoffset")) {
        if (!knownValues["tzoffset"].first) {
            impliedValues["tzoffset"].first = true;
            impliedValues["tzoffset"].second = value;
        }
        return;
    }
    else
        std::cerr << "Invalid option: " << comp << std::endl;
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

    else if(!comp.compare("tzoffset"))
        return knownValues["tzoffset"].first;

    else std::cerr << "Invalid component: " << comp << std::endl;

    return false;
}

bool ParsedComponents::isPossibleDate() {
    // needs implementation
    // std::time_t tmp = std::time(nullptr);
    std::tm tmp, tmp1; // = std::gmtime(&tmp);

    tmp1.tm_year = getYear();
    tmp1.tm_mon  = getMonth()-1;
    tmp1.tm_mday = get_mDay();
    tmp1.tm_hour = getHour();
    tmp1.tm_min  = getMinute();
    tmp1.tm_sec  = getSeconds();

    posix_time::ptime tmp2 = posix_time::ptime_from_tm(tmp1);
    tmp = posix_time::to_tm(tmp2);

    if(tmp.tm_year != getYear())    return false;
    if(tmp.tm_mon  != getMonth()-1) return false;
    if(tmp.tm_mday != get_mDay())   return false;
    if(tmp.tm_wday != get_wDay())   return false;
    if(tmp.tm_hour != getHour())    return false;
    if(tmp.tm_min  != getMinute())  return false;

    /*
    if(knownValues["year"].first  == false and
       knownValues["month"].first == false and
       knownValues["mday"].first  == false and
       knownValues["wday"].first  == false and
       knownValues["hour"].first  == false and
       knownValues["min"].first   == false and
       knownValues["sec"].first   == false)
        return false;*/

    return true;
}

/***************************************************************************************/

ParsedResult::ParsedResult() {
    index = 0;
    text = "";

    ParsedComponents tmp1; //, tmp2{};
    startDate = tmp1;
    __end = false;
    // endDate = tmp2;

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

ParsedResult::ParsedResult(posix_time::ptime an, long idx, std::string tx) {
    anchor = an;
    index = idx;
    text = tx;

    ParsedComponents tmp; //, _tmp{};
    startDate = tmp;
    __end = false;
    // endDate = tmp;
}

// need to implement use of endDate
ParsedResult::ParsedResult(const ParsedResult& pr) {
    anchor    = pr.anchor;
    index     = pr.index;
    text      = pr.text;
    tags      = pr.tags;
    startDate = pr.startDate;
    __end     = false;

    if(pr.end()) {
        endDate = pr.endDate;
        __end = true;
    }
}

bool ParsedResult::hasPossibleDates() {
    if(startDate.getYear()    == 0 and startDate.getMonth()   == 0 and
       startDate.get_mDay()   == 0 and// startDate.get_wDay()   == 0 and
       startDate.getHour()    == 0 and startDate.getMinute()  == 0 and
       startDate.getSeconds() == 0 and end() == false)
        return false;

    //return startDate.isPossibleDate() and (!end() or endDate.isPossibleDate());
    return true;
}

ParsedResult::~ParsedResult() { }

std::string ParsedResult::toDate() {
    // todo: take into account timezone offset
    struct tm date_start, date_end;
    std::string res;

    date_start.tm_year = startDate.getYear() - 1900;
    date_start.tm_mon  = startDate.getMonth() - 1;
    date_start.tm_mday = startDate.get_mDay();
    date_start.tm_hour = startDate.getHour();
    date_start.tm_min  = startDate.getMinute();
    date_start.tm_sec  = startDate.getSeconds();

    posix_time::ptime st = posix_time::ptime_from_tm(date_start);
    if(getTag(utils::ExtractTimeZoneAbbreviation))
        st += posix_time::minutes(startDate.getTimeZoneOffset());

    res = posix_time::to_simple_string(st);

    if(end()) {
        date_end.tm_year = endDate.getYear() - 1900;
        date_end.tm_mon  = endDate.getMonth() - 1;
        date_end.tm_mday = endDate.get_mDay();
        date_end.tm_hour = endDate.getHour();
        date_end.tm_min  = endDate.getMinute();
        date_end.tm_sec  = endDate.getSeconds();

        posix_time::ptime ed = posix_time::ptime_from_tm(date_end);
        if(getTag(utils::ExtractTimeZoneAbbreviation))
            ed += posix_time::minutes(startDate.getTimeZoneOffset());

        res += " - " + posix_time::to_simple_string(posix_time::ptime_from_tm(date_end));
    }

    return res;
}

unsigned ParsedResult::getIndex() const {
    return index;
}

void ParsedResult::setIndex(int idx) {
    index = idx;
    return;
}

void ParsedResult::setText(std::string tx) {
    text += " " + tx;
}

size_t ParsedResult::textLength() const {
    return text.length();
}

void ParsedResult::setTag(utils::Modifiers tag_name) {
    if(tags.count(tag_name) < 1)
        this->tags.insert({tag_name, true});
}

bool ParsedResult::isEmpty() const {
    if(index == 0 and text.empty())
        return true;
    return false;
} // better way to check for empty result

bool ParsedResult::end() const {
    return __end;
}

void ParsedResult::makeEndDateValid() {
    __end = true;
}

bool ParsedResult::getTag(utils::Modifiers m) {
    return tags[m];

}

ParsedResult& ParsedResult::operator=(ParsedResult pr) {
    anchor    = pr.anchor;
    endDate   = pr.endDate;
    startDate = pr.startDate;
    text      = pr.text;
    index     = pr.index;
    tags      = pr.tags;
    __end     = pr.end();

    return *(this);
}

/*ParsedResult& ParsedResult::operator=(ParsedResult &) {
    return <#initializer#>;
}
*/