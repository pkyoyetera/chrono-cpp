#ifndef UTILS_HPP
#define UTILS_HPP

#include <map>
#include <list>
#include <regex>
#include <iostream>
#include <unordered_map>
#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
// temporary timezone adjustments
#include "boost/date_time/local_time_adjustor.hpp"
#include "boost/date_time/c_local_time_adjustor.hpp"

using namespace boost;

namespace utils {
    typedef enum modifiers {
        ENCasualTimeParser,
        ENCasualDateParser,
        ENDeadlineFormatParser,
        ENDayOfTheWeekParser,
        ENMonthNameMiddleEndianParser,
        ENTimeLaterParser,
        ENMonthNameParser,
        ENTimeAgoFormatParser,
        ENISOFormatParser,
        ENTimeExpressionParser,
        ENUSHolidaysParser,

        ENMergeDateRangeRefiner,
        ENMergeDateAndTimeRefiner,
        ExtractTimeZoneAbbreviation,

    } Modifiers;
    typedef std::unordered_map<Modifiers, bool> Tags;

    static std::list<modifiers> ModList{ENCasualTimeParser, ENCasualDateParser,
                                        ENDeadlineFormatParser, ENDayOfTheWeekParser,
                                        ENMonthNameMiddleEndianParser, ENTimeLaterParser,
                                        ENMonthNameParser, ENTimeAgoFormatParser,
                                        ENISOFormatParser, ENTimeExpressionParser,
                                        ENUSHolidaysParser,

                                        ENMergeDateRangeRefiner, ENMergeDateAndTimeRefiner,
                                        ExtractTimeZoneAbbreviation,};

    static std::map<std::string, int> INTEGER_WORDS{
            {"one",    1},
            {"two",    2},
            {"three",  3},
            {"four",   4},
            {"five",   5},
            {"six",    6},
            {"seven",  7},
            {"eight",  8},
            {"nine",   9},
            {"ten",    10},
            {"eleven", 11},
            {"twelve", 12}
    };

    static std::map<std::string, int> ORDINAL_WORDS{
            {"first",           1},
            {"second",          2},
            {"third",           3},
            {"fourth",          4},
            {"fifth",           5},
            {"sixth",           6},
            {"seventh",         7},
            {"eighth",          8},
            {"ninth",           9},
            {"tenth",          10},
            {"eleventh",       11},
            {"twelfth",        12},
            {"thirteenth",     13},
            {"fourteenth",     14},
            {"fifteenth",      15},
            {"sixteenth",      16},
            {"seventeenth",    17},
            {"eighteenth",     18},
            {"nineteenth",     19},
            {"twentieth",      20},
            {"twenty first",   21},
            {"twenty second",  22},
            {"twenty third",   23},
            {"twenty fourth",  24},
            {"twenty fifth",   25},
            {"twenty sixth",   26},
            {"twenty seventh", 27},
            {"twenty eighth",  28},
            {"twenty ninth",   29},
            {"thirtieth",      30},
            {"thirty first",   31}
    };


    static std::map<std::string, int> MONTH_CONSTANTS{
            {"january",    1}, {"jan", 1},  {"jan.",  1},
            {"february",   2}, {"feb", 2},  {"feb.",  2},
            {"march",      3}, {"mar", 3},  {"mar.",  3},
            {"april",      4}, {"apr", 4},  {"apr.",  4},
            {"may",        5},
            {"june",       6}, {"jun", 6},  {"jun.",  6},
            {"july",       7}, {"jul", 7},  {"jul.",  7},
            {"august",     8}, {"aug", 8},  {"aug.",  8},
            {"september",  9}, {"sep", 9},  {"sep.",  9}, {"sept", 9}, {"sept.", 9},
            {"october",   10}, {"oct", 10}, {"oct.", 10},
            {"november",  11}, {"nov", 11}, {"nov.", 11},
            {"december",  12}, {"dec", 12}, {"dec.", 12},
    };

    static std::map<std::string, int> WEEKDAY_OFFSET{
            {"sunday",   0}, {"sun",  0},
            {"monday",   1}, {"mon",  1},
            {"tuesday",  2}, {"tue",  2}, {"tues",  2},
            {"wednesday",3}, {"wed",  3},
            {"thursday", 4}, {"thur", 4}, {"thurs", 4}, {"thu", 4},
            {"friday",   5}, {"fri",  5},
            {"saturday", 6}, {"sat",  6}
    };

    static std::map<std::string, int> DEFAULT_TIMEZONE_ABBR_MAP {
            {"ACDT",  630}, {"ACST",  570}, {"ADT",  -180}, {"AEDT", 660}, {"AEST",  600},
            {"AFT",   270}, {"AKDT", -480}, {"AKST", -540}, {"ALMT", 360}, {"AMST", -180},
            {"AMT",  -240}, {"ANAST", 720}, {"ANAT",  720}, {"AQTT", 300}, {"ART", - 180},
            {"AST",  -240}, {"AWDT",  540}, {"AWST",  480}, {"AZOST",  0}, {"AZOT",  -60},
            {"AZST",  300}, {"AZT",   240}, {"BNT",   480}, {"BOT", -240}, {"BRST", -120},
            {"BRT",  -180}, {"BST",    60}, {"BTT",   360}, {"CAST", 480}, {"CAT",   120},
            {"CCT",   390}, {"CDT",  -300}, {"CEST",  120}, {"CET",   60}, {"CHADT", 825},
            {"CHAST", 765}, {"CKT",  -600}, {"CLST", -180}, {"CLT", -240}, {"COT",  -300},
            {"CST",  -360}, {"CVT",   -60}, {"CXT",   420}, {"ChST", 600}, {"DAVT",  420},
            {"EASST",-300}, {"EAST", -360}, {"EAT",   180}, {"ECT", -300}, {"EDT",  -240},
            {"EEST",  180}, {"EET",   120}, {"EGST",    0}, {"EGT",  -60}, {"EST",  -300},
            {"ET",   -300}, {"FJST",  780}, {"FJT",   720}, {"FKST",-180}, {"FKT",  -240},
            {"FNT",  -120}, {"GALT", -360}, {"GAMT", -540}, {"GET",  240}, {"GFT",  -180},
            {"GILT",  720}, {"GMT",     0}, {"GST",   240}, {"GYT", -240}, {"HAA",  -180},
            {"HAC",  -300}, {"HADT", -540}, {"HAE",  -240}, {"HAP", -420}, {"HAR",  -360},
            {"HAST", -600}, {"HAT",   -90}, {"HAY",  -480}, {"HKT",  480}, {"HLV",  -210},
            {"HNA",  -240}, {"HNC",  -360}, {"HNE",  -300}, {"HNP", -480}, {"HNR",  -420},
            {"HNT",  -150}, {"HNY",  -540}, {"HOVT",  420}, {"ICT",  420}, {"IDT",   180},
            {"IOT",   360}, {"IRDT",  270}, {"IRKST", 540}, {"IRKT", 540}, {"IRST",  210},
            {"IST",   330}, {"JST",   540}, {"KGT",   360}, {"KRAST",480}, {"KRAT",  480},
            {"KST",   540}, {"KUYT",  240}, {"LHDT",  660}, {"LHST", 630}, {"LINT",  840},
            {"MAGST", 720}, {"MAGT",  720}, {"MART", -510}, {"MAWT", 300}, {"MDT",  -360},
            {"MESZ",  120}, {"MEZ",    60}, {"MHT",   720}, {"MMT",  390}, {"MSD",   240},
            {"MSK",   240}, {"MST",  -420}, {"MUT",   240}, {"MVT",  300}, {"MYT",   480},
            {"NCT",   660}, {"NDT",   -90}, {"NFT",   690}, {"NOVST",420}, {"NOVT",  360},
            {"NPT",   345}, {"NST",  -150}, {"NUT",  -660}, {"NZDT", 780}, {"NZST",  720},
            {"OMSST", 420}, {"OMST",  420}, {"PDT",  -420}, {"PET", -300}, {"PETST", 720},
            {"PETT",  720}, {"PGT",   600}, {"PHOT",  780}, {"PHT",  480}, {"PKT",   300},
            {"PMDT", -120}, {"PMST", -180}, {"PONT",  660}, {"PST", -480}, {"PT",   -480},
            {"PWT",   540}, {"PYST", -180}, {"PYT",  -240}, {"RET",  240}, {"SAMT",  240},
            {"SAST",  120}, {"SBT",   660}, {"SCT",   240}, {"SGT",  480}, {"SRT",  -180},
            {"SST",  -660}, {"TAHT", -600}, {"TFT",   300}, {"TJT",  300}, {"TKT",   780},
            {"TLT",   540}, {"TMT",   300}, {"TVT",   720}, {"ULAT", 480}, {"UTC",     0},
            {"UYST", -120}, {"UYT",  -180}, {"UZT",   300}, {"VET", -210}, {"VLAST", 660},
            {"VLAT",  660}, {"VUT",   660}, {"WAST",  120}, {"WAT",   60}, {"WEST",   60},
            {"WESZ",   60}, {"WET",     0}, {"WEZ",     0}, {"WFT",  720}, {"WGST", -120},
            {"WGT",  -180}, {"WIB",   420}, {"WIT",   540}, {"WITA", 480}, {"WST",   780},
            {"WT",      0}, {"YAKST", 600}, {"YAKT",  600}, {"YAPT", 600}, {"YEKST", 360},
            {"YEKT",  360}
    };

    static std::string ordinalWords = [&](std::map<std::string, int> &) -> std::string {
        std::string retVal, t;
        // std::regex re{"\\s"};
        for (auto a : ORDINAL_WORDS) {
            t = std::regex_replace(a.first, std::regex("\\s"), "[ -]");
            retVal.append(t + "|");
        }
        if (retVal.back() == '|')
            retVal.pop_back();
        return retVal;
    }(ORDINAL_WORDS);

    std::string toLowerCase(std::string);
    std::string toUpperCase(std::string);

    int argToOrdinalValue(const std::string &);

    // todo: make template
    std::string keysToString(std::map<std::string, int>);

    static std::string INTEGER_WORDS_PATTERN{"(?:" + utils::keysToString(utils::INTEGER_WORDS) + ")"};

    static std::string TIME_UNIT{"(" + INTEGER_WORDS_PATTERN + "|[0-9]+|an?(?:\\s*few)?|half(?:\\s*an?)?)\\s*" +
                                 "(sec(?:onds?)?|min(?:ute)?s?|hours?|weeks?|days?|months?|years?)\\s*"};

    static std::regex PATTERN_TIME_UNIT(TIME_UNIT, std::regex::icase);

    std::map<std::string, float> extractDateTimeUnitFragments(std::string);

    std::map<std::string, float> collectDateTimeFragment(std::smatch , std::map<std::string, float>& );
}
typedef gregorian::nth_day_of_the_week_in_month ndow;
typedef gregorian::last_day_of_the_week_in_month ldow;

#endif
