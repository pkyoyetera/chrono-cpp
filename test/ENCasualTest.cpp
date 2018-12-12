
#include <iostream>
#include <chrono>
// #include <iomanip>
// #include <sstream>

#include "gtest/gtest.h"
#include "src/parsers/en/ENCasualTimeParser.h"

using namespace std;
using std::atoi;

class ENCasualTimeTest : public ::testing::Test {
public:
    ENCasualTimeParser ps;
    parse::ParsedComponents _cp;
    tm anchor, *today;
    time_t tmp;
    string text1, text2, text3, text4, text5;
    std::vector<parse::ParsedResult> results;

    void setCP(tm st_tmp) {
        _cp.setYear(st_tmp.tm_year);
        _cp.setMonth(st_tmp.tm_mon);
        _cp.set_mDay(st_tmp.tm_mday);
        _cp.set_wDay(st_tmp.tm_wday);
        _cp.setHour(st_tmp.tm_hour);
        _cp.setMinute(st_tmp.tm_min);
        _cp.setSeconds(st_tmp.tm_sec);
    }

    ENCasualTimeTest() {

        text1 = "What about this morning!";
        text2 = "This afternoon was unusually cold";
        text3 = "the evening sunset is noice";
        text4 = "How does noon today sound?";
        text5 = "Eventually the night is calm";
    }

    virtual void SetUp() {
        // set up work before the test is called
    }

    virtual void TearDown() {
        // clean up work after the test executes
    }

    ~ENCasualTimeTest() {}

};


TEST_F(ENCasualTimeTest, test1) {
    time(&tmp);
    today = localtime(&tmp);

    setCP((*today));

    results = ps.execute(text1, _cp);

    EXPECT_EQ(results[0].startDate.getYear(), today->tm_year);
    EXPECT_EQ(results[0].startDate.getMonth(), today->tm_mon);
    EXPECT_EQ(results[0].startDate.get_mDay(), today->tm_mday);
    EXPECT_EQ(results[0].startDate.get_wDay(), today->tm_wday);
    ASSERT_EQ(results[0].startDate.getHour(), 06);

    results = ps.execute(text2, _cp);
    ASSERT_EQ(results[0].startDate.getHour(), 15);

    results = ps.execute(text3, _cp);
    ASSERT_EQ(results[0].startDate.getHour(), 20);

    results = ps.execute(text4, _cp);
    ASSERT_EQ(results[0].startDate.getHour(), 12);

    results = ps.execute(text5, _cp);
    ASSERT_EQ(results[0].startDate.getHour(), 20);
}


TEST_F(ENCasualTimeTest, test2) {
    string testRefDate("2018-02-13 18:16:27"), format("%Y-%m-%d %T");

    // int example = atoi(testRefDate.substr(0, 4).c_str())-1900;  // this works fine
    anchor.tm_year = atoi(testRefDate.substr(0, 4).c_str()); // this does not
    anchor.tm_mon  = atoi(testRefDate.substr(5, 2).c_str());
    anchor.tm_mday = atoi(testRefDate.substr(8, 2).c_str());
    anchor.tm_hour = atoi(testRefDate.substr(11, 2).c_str());
    anchor.tm_min  = atoi(testRefDate.substr(14, 2).c_str());
    anchor.tm_sec  = atoi(testRefDate.substr(17, 2).c_str());

    // anchor = (*today);

    // get_time(today, format.c_str());
    // strptime(testRefDate.c_str(), format.c_str(), today);        // careful here strptime is a bitch

    setCP(anchor);

    results = ps.execute(text1, _cp);

    EXPECT_EQ(results[0].startDate.getYear(), anchor.tm_year);
    EXPECT_EQ(results[0].startDate.getMonth(), anchor.tm_mon);
    EXPECT_EQ(results[0].startDate.get_mDay(), anchor.tm_mday);
    EXPECT_EQ(results[0].startDate.get_wDay(), anchor.tm_wday);
    ASSERT_EQ(results[0].startDate.getHour(), 06);

    results = ps.execute(text2, _cp);
    ASSERT_EQ(results[0].startDate.getHour(), 15);

    results = ps.execute(text3, _cp);
    ASSERT_EQ(results[0].startDate.getHour(), 20);

    results = ps.execute(text4, _cp);
    ASSERT_EQ(results[0].startDate.getHour(), 12);

    results = ps.execute(text5, _cp);
    ASSERT_EQ(results[0].startDate.getHour(), 20);

}