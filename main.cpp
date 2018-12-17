#include <iostream>
#include <string>
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
    std::vector<parse::ParsedResult> results;
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

    results = pt.execute(str, t);
    cout << results[0].toDate() << endl;

    return 0;
}