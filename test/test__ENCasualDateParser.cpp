
#include "gtest/gtest.h"
#include "src/parsers/en/ENCasualDateParser.h"

using namespace std;


class ENCasualDateTest : public ::testing::Test {
protected:

    ENCasualDateParser dateParser;
    tm anchor;
    string t1, t2, t3, t4, t5;
    std::vector<parse::ParsedResult> results;

    ENCasualDateTest() {
        t1 = "last NIGHT was not bonkers";
        t2 = "Was tomorrow morning even acceptable?";
        t3 = "";
        t4 = "Later tonight then? ";
        t5 = "RIGHT NOW!!!";
    }

    ~ENCasualDateTest() = default;

};


TEST_F(ENCasualDateTest, construct) {
    posix_time::ptime t( posix_time::second_clock::local_time() );
    anchor = posix_time::to_tm(t);

    // test 1: date corresponds to yesterday's

    results = dateParser.execute(t1, t);
    EXPECT_EQ(results[0].startDate.getYear(),  anchor.tm_year) ;
    EXPECT_EQ(results[0].startDate.getMonth(), anchor.tm_mon) ;
    EXPECT_EQ(results[0].startDate.get_mDay(), anchor.tm_mday - 1);
    EXPECT_EQ(results[0].startDate.get_wDay(), anchor.tm_wday - 1);
     // EXPECT_STREQ(results[0].text, t1.substr(0, 10)); // text is protected

    /// todo: place the above tests into a general test case for parsers in general

    /// test 2: date corresponds to tomorrow
    results = dateParser.execute(t2, t);
    EXPECT_EQ(results[0].startDate.getYear(),  anchor.tm_year) ;
    EXPECT_EQ(results[0].startDate.getMonth(), anchor.tm_mon) ;
    EXPECT_EQ(results[0].startDate.get_mDay(), anchor.tm_mday + 1);
    EXPECT_EQ(results[0].startDate.get_wDay(), anchor.tm_wday + 1);

    /// test 4: date should correspond to later today
    results = dateParser.execute(t4, t);
    EXPECT_EQ(results[0].startDate.getYear(),  anchor.tm_year);
    EXPECT_EQ(results[0].startDate.getMonth(), anchor.tm_mon);
    EXPECT_EQ(results[0].startDate.get_mDay(), anchor.tm_mday);
    EXPECT_EQ(results[0].startDate.get_wDay(), anchor.tm_wday);

    /// test 5: corresponds to right this moment
    results = dateParser.execute(t5, t);
    EXPECT_EQ(results[0].startDate.getYear(),  anchor.tm_year);
    EXPECT_EQ(results[0].startDate.getMonth(), anchor.tm_mon);
    EXPECT_EQ(results[0].startDate.get_mDay(), anchor.tm_mday);
    EXPECT_EQ(results[0].startDate.get_wDay(), anchor.tm_wday);
}

