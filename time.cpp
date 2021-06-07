
#include <iostream>
#include <string>
#include <memory>

#include "src/parsers/en/ENISOFormatParser.hpp"
#include "src/parsers/en/ENTimeLaterParser.hpp"
#include "src/parsers/en/ENMonthNameParser.hpp"
#include "src/parsers/en/ENCasualTimeParser.hpp"
#include "src/parsers/en/ENCasualDateParser.hpp"
#include "src/parsers/en/ENUSHolidaysParser.hpp"
#include "src/parsers/en/ENDayOfTheWeekParser.hpp"
#include "src/parsers/en/ENTimeAgoFormatParser.hpp"
#include "src/parsers/en/ENDeadlineFormatParser.hpp"
#include "src/parsers/en/ENTimeExpressionParser.hpp"
#include "src/parsers/en/ENWeekExperessionParser.hpp"
#include "src/parsers/en/ENMonthNameMiddleEndianParser.hpp"

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

    posix_time::ptime t;
    string str;

    auto ctp = make_shared<parser::ENCasualTimeParser>();
    auto dfp = make_shared<parser::ENDeadlineFormatParser>();
    auto cdp = make_shared<parser::ENCasualDateParser>();
    auto dow = make_shared<parser::ENDayOfWeekParser>();
    auto mme = make_shared<parser::ENMonthNameMiddleEndianParser>();
    auto tlp = make_shared<parser::ENTimeLaterParser>();
    auto mnp = make_shared<parser::ENMonthNameParser>();
    auto tap = make_shared<parser::ENTimeAgoFormatParser>();
    auto iso = make_shared<parser::ENISOFormatParser>();
    auto hol = make_shared<parser::ENHolidayParser>();
    auto wxp = make_shared<parser::ENWeekExpressionParser>();
    auto txp = make_shared<parser::ENTimeExpressionParser>();

    auto olr = make_shared<OverlapRemover>();
    auto tza = make_shared<ExtractTimeZoneAbbreviation>();
    auto mdt = make_shared<ENMergeDateAndTime>();
    auto mdr = make_shared<ENMergeDateRange>();

    ctp->chain(dfp)
       ->chain(dow)
       ->chain(cdp)
       ->chain(mme)
       ->chain(tlp)
       ->chain(mnp)
       ->chain(tap)
       ->chain(txp)
       ->chain(iso)
       ->chain(hol)
       ->chain(wxp);

    list<std::shared_ptr<refiners::Refiner> > refiners {olr, tza, mdt, mdr}; // NOTE: place mdt refiner before mdr refiner

    str = argv[1];

    if(argc == 2){
        t = posix_time::second_clock::local_time();
    }

    if(argc == 3) {
        string refDate = argv[2];
        t = posix_time::time_from_string(refDate);
    }

    parse::Result results;
    ctp->execute(str, t, results);

    std::sort(results.begin(), results.end(),
            [&](parse::ParsedResult p1, parse::ParsedResult p2) {
                return p1.getIndex() < p2.getIndex();
    });

    for(auto& refiner: refiners) {
        results = refiner->refine(results, str);
    }

    if(results.empty())
        cout << "[???] -- Invalid date" << endl;
    else
        cout << "Date:\t"  << results.at(0).toDate() << endl;

    return 0;
}