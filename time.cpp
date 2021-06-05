
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

    Result results;
    posix_time::ptime t;
    string str;

    shared_ptr<parser::Parser> ctp = make_shared<parser::ENCasualTimeParser>();
    shared_ptr<parser::Parser> dfp = make_shared<parser::ENDeadlineFormatParser>();
    shared_ptr<parser::Parser> cdp = make_shared<parser::ENCasualDateParser>();
    shared_ptr<parser::Parser> dow = make_shared<parser::ENDayOfWeekParser>();
    shared_ptr<parser::Parser> mme = make_shared<parser::ENMonthNameMiddleEndianParser>();
    shared_ptr<parser::Parser> tlp = make_shared<parser::ENTimeLaterParser>();
    shared_ptr<parser::Parser> mnp = make_shared<parser::ENMonthNameParser>();
    shared_ptr<parser::Parser> tap = make_shared<parser::ENTimeAgoFormatParser>();
    shared_ptr<parser::Parser> iso = make_shared<parser::ENISOFormatParser>();
    shared_ptr<parser::Parser> hol = make_shared<parser::ENHolidayParser>();
    shared_ptr<parser::Parser> wxp = make_shared<parser::ENWeekExpressionParser>();
    shared_ptr<parser::Parser> txp = make_shared<parser::ENTimeExpressionParser>();

    shared_ptr<refiners::Refiner> olr = make_shared<OverlapRemover>();
    shared_ptr<refiners::Refiner> tza = make_shared<ExtractTimeZoneAbbreviation>();
    shared_ptr<refiners::Refiner> mdt = make_shared<ENMergeDateAndTime>();
    shared_ptr<refiners::Refiner> mdr = make_shared<ENMergeDateRange>();

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

    // list<std::shared_ptr<parser::Parser> >  parsers  {ctp, dfp, dow, cdp, mme, tlp, mnp, tap, txp, iso, hol, wxp};
    list<std::shared_ptr<refiners::Refiner> > refiners {olr, tza, mdt, mdr}; // NOTE: place mdt refiner before mdr refiner

    str = argv[1];

    if(argc == 2){
        t = posix_time::second_clock::local_time();
    }

    if(argc == 3) {
        string refDate = argv[2];
        t = posix_time::time_from_string(refDate);
    }

    Result p_result;
    ctp->execute(str, t, p_result);

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