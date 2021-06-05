#include "parsed_result.hpp"

using namespace parse;

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
    /**
     * @brief checks if (*this) result contains a valid date
     */
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
    // temporary timezone adjustment to UTC.
    // NOTE: this is machine dependent. Be very careful where/when its used
    // typedef date_time::local_adjustor<posix_time::ptime, -5, posix_time::us_dst> us_eastern;
    struct tm date_start, date_end;
    std::string res;

    date_start.tm_year = startDate.getYear() - 1900;
    date_start.tm_mon  = startDate.getMonth() - 1;
    date_start.tm_mday = startDate.get_mDay();
    date_start.tm_hour = startDate.getHour();
    date_start.tm_min  = startDate.getMinute();
    date_start.tm_sec  = startDate.getSeconds();

    posix_time::ptime st = posix_time::ptime_from_tm(date_start);
    // posix_time::ptime st =  us_eastern::local_to_utc(posix_time::ptime_from_tm(date_start));

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
    text = tx;
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