#include <iostream>
#include <cstdlib>
#include <string>
#include <stdlib.h>
#include "src/parsers/en/ENCasualTimeParser.h"


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

    ENCasualTimeParser pt;
    parse::ParsedComponents ref;
    std::vector<parse::ParsedResult> results;
    tm brokenTime, *tmp2;
    string str;

    if (argc == 2){
        time_t tmp;
        time(&tmp);
        tmp2 = localtime(&tmp);
        brokenTime = (*tmp2);
    }

    if(argc == 3) {
        string refDate = argv[2];

        // strptime(refDate.c_str(), "%Y-%m-%d %T", brokenTime);
        // time_t sinceEpoch = timegm(brokenTime);
        brokenTime.tm_year = atoi(refDate.substr(0, 4).c_str())-1900; // this does not
        brokenTime.tm_mon  = atoi(refDate.substr(5, 2).c_str())-1;
        brokenTime.tm_mday = atoi(refDate.substr(8, 2).c_str());
        //brokenTime.tm_wday = 0;
        brokenTime.tm_hour = atoi(refDate.substr(11, 2).c_str());
        brokenTime.tm_min  = atoi(refDate.substr(14, 2).c_str());
        brokenTime.tm_sec  = atoi(refDate.substr(17, 2).c_str());

            // cout << brokenTime.tm_min << " " << brokenTime.tm_wday << " " << brokenTime.tm_hour << endl;
    }

    ref.setMonth(brokenTime.tm_mon );
    ref.set_mDay(brokenTime.tm_mday);
    // ref.set_wDay(brokenTime.tm_wday);
    ref.setHour( brokenTime.tm_hour);
    ref.setMinute(brokenTime.tm_min );
    ref.setSeconds(brokenTime.tm_sec );
    ref.setYear( brokenTime.tm_year);

    /*
     cout << ref.getYear() << " " << ref.getMonth() << " " <<
         ref.get_mDay() << " " << ref.getHour() << " " <<
         ref.getMinute() << " " << endl;
    */

    str = argv[1];

    results = pt.execute(str, ref);
    cout << results[0].toDate() << endl;



    return 0;
}