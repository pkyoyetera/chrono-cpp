
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

    std::shared_ptr<Parser> ctp = make_shared<ENCasualTimeParser>();
    std::shared_ptr<Parser> dfp = make_shared<ENDeadlineFormatParser>();
    std::shared_ptr<Parser> cdp = make_shared<ENCasualDateParser>();
    std::shared_ptr<Parser> dow = make_shared<ENDayOfWeekParser>();
    std::shared_ptr<Parser> mme = make_shared<ENMonthNameMiddleEndianParser>();
    std::shared_ptr<Parser> tlp = make_shared<ENTimeLaterParser>();
    std::shared_ptr<Parser> mnp = make_shared<ENMonthNameParser>();
    std::shared_ptr<Parser> tap = make_shared<ENTimeAgoFormatParser>();
    std::shared_ptr<Parser> iso = make_shared<ENISOFormatParser>();
    std::shared_ptr<Parser> hol = make_shared<ENHolidayParser>();
    std::shared_ptr<Parser> wxp = make_shared<ENWeekExpressionParser>();
    std::shared_ptr<Parser> txp = make_shared<ENTimeExpressionParser>();

    std::shared_ptr<Refiner> olr = make_shared<OverlapRemover>();
    std::shared_ptr<Refiner> tza = make_shared<ExtractTimeZoneAbbreviation>();
    std::shared_ptr<Refiner> mdt = make_shared<ENMergeDateAndTime>();
    std::shared_ptr<Refiner> mdr = make_shared<ENMergeDateRange>();


    list<std::shared_ptr<Parser> >  parsers  {ctp, dfp, dow, cdp, mme, tlp, mnp, tap, txp, iso, hol, wxp};
    list<std::shared_ptr<Refiner> > refiners {olr, tza, mdt, mdr}; // NOTE: place mdt refiner before mdr refiner

    str = argv[1];

    if (argc == 2){
        t = posix_time::second_clock::local_time();
    }

    if(argc == 3) {
        string refDate = argv[2];
        t = posix_time::time_from_string(refDate);
    }

    for(auto& parser: parsers) {
        Result p_result = parser->execute(str, t);
        results.insert(results.end(), p_result.begin(), p_result.end());
    }

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