#ifndef UTILS_HPP
#define UTILS_HPP


#include <unordered_map>
#include <iostream>
// #include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

using namespace boost;


namespace utils {
    typedef enum modifiers {
        ENCasualTimeParser,
        ENCasualDateParser,
    } Modifiers;


    typedef std::unordered_map<Modifiers, bool > Tags;

    // ptime to date
    // date to ptime
    // struct tm tp ptime
    // ptime to struct tm
    // gregorian to struct tm
    // struct tm to gregorian

}


#endif