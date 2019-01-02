#include <iostream>
#include <string>
#include "src/parsers/en/ENCasualTimeParser.h"
#include "src/parsers/en/ENCasualDateParser.h"
#include "src/parsers/en/ENDeadlineFormatParser.hpp"

#include "src/refiners/OverlapRemovalRefiner.hpp"


using namespace std;
using std::atoi;


void printUsage() {
    cout << "Usage: ./main [test string] [reference date] (last argument is optional) " << endl;
    cout << "If reference date is provided, format should be YYY-MM-DD HH:MM:SS" << endl;
}

int main(int argc, char* argv[]) {

    if(argc < 2 or argc > 3) {
        printUsage();
        return 0;
    }

    Parser* tp = new ENCasualTimeParser();
    Parser* dp = new ENCasualDateParser();
    Parser* dfp = new ENDeadlineFormatParser();


    std::list<Parser*> parsers {tp, dp, dfp};

    Result results;
    posix_time::ptime t;
    string str;

    if (argc == 2){
        t = posix_time::second_clock::local_time();
    }

    if(argc == 3) {
        string refDate = argv[2];
        t = posix_time::time_from_string(refDate);
    }

    str = argv[1];

    for(std::list<Parser*>::iterator it = parsers.begin(); it != parsers.end(); ++it) {
        Result p_result = (*it)->execute(str, t);
        results.insert(results.end(), p_result.begin(), p_result.end());
    }

    for(int i=0; i<results.size(); i++)
        cout << results[i].toDate() << endl;

    Refiner* ov = new OverlapRemover();
    Result final = ov->refine(results, str);

    cout << endl;
    cout << "Refiner result" << endl;
    for(int i=0; i<results.size(); i++)
        cout << results[i].toDate() << endl;

    return 0;
}