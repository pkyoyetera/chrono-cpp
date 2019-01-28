
#include <iostream>
#include <string>

#include "src/parsers/en/ENCasualTimeParser.hpp"
#include "src/parsers/en/ENCasualDateParser.hpp"
#include "src/parsers/en/ENTimeLaterParser.hpp"
#include "src/parsers/en/ENMonthNameParser.hpp"
#include "src/parsers/en/ENDayOfTheWeekParser.hpp"
#include "src/parsers/en/ENTimeAgoFormatParser.hpp"
#include "src/parsers/en/ENDeadlineFormatParser.hpp"
#include "src/parsers/en/ENMonthNameMiddleEndianParser.hpp"
#include "src/parsers/en/ENTimeExpressionParser.hpp"
#include "src/parsers/en/ENISOFormatParser.hpp"
#include "src/parsers/en/ENUSHolidaysParser.hpp"

#include "src/refiners/OverlapRemovalRefiner.hpp"
#include "src/refiners/en/ENMergeDateRangeRefiner.hpp"
#include "src/refiners/ExtractTimeZoneAbbreviation.hpp"
#include "src/refiners/en/ENMergeDateAndTimeRefiner.hpp"


using namespace std;
using std::atoi;

void printUsage() {
    cout << "Usage: ./main [test string] [reference date] (last argument is optional) " << endl;
    cout << "If reference date is provided, format should be YYYY-MM-DD HH:MM:SS" << endl;
}

int main(int argc, char* argv[]) {
    if(argc < 2 or argc > 3) {
        printUsage();
        return 0;
    }

    Result results;
    posix_time::ptime t;
    string str;

    Parser* ctp = new ENCasualTimeParser();
    Parser* cdp = new ENCasualDateParser();
    Parser* dfp = new ENDeadlineFormatParser();
    Parser* dow = new ENDayOfWeekParser();
    Parser* mme = new ENMonthNameMiddleEndianParser();
    Parser* tlp = new ENTimeLaterParser();
    Parser* mnp = new ENMonthNameParser();
    Parser* tap = new ENTimeAgoFormatParser();
    Parser* txp = new ENTimeExpressionParser();
    Parser* iso = new ENISOFormatParser();
    Parser* hol = new ENHolidayParser();

    Refiner* olr = new OverlapRemover();
    Refiner* tza = new ExtractTimeZoneAbbreviation();
    Refiner* mdr = new ENMergeDateRange();
    Refiner* mdt = new ENMergeDateAndTime();

    list<Parser*>  parsers  {ctp, dfp, dow, cdp, mme, tlp, mnp, tap, txp, iso, hol};
    list<Refiner*> refiners {olr, tza, mdt, mdr}; // NOTE: place mdt refiner before mdr refiner

    str = argv[1];

    if (argc == 2){
        t = posix_time::second_clock::local_time();
    }

    if(argc == 3) {
        string refDate = argv[2];
        t = posix_time::time_from_string(refDate);
    }

    for(list<Parser*>::iterator it = parsers.begin(); it != parsers.end(); ++it) { // NOLINT(modernize-use-auto)
        Result p_result = (*it)->execute(str, t);
        results.insert(results.end(), p_result.begin(), p_result.end());
    }

    std::sort(results.begin(), results.end(),
            [&](parse::ParsedResult p1, parse::ParsedResult p2) {
                return p1.getIndex() < p2.getIndex();
    });

    for(list<Refiner*>::iterator it = refiners.begin(); it != refiners.end(); ++it) {
        results = (*it)->refine(results, str);
    }

    if(results.empty())
        cout << "[???] -- Invalid date" << endl;
    else
        cout << "Date:\t"  << results.at(0).toDate() << endl;

    // free memory
    for(auto p: parsers)
        delete p;
    for(auto r: refiners)
        delete r;

    return 0;
}