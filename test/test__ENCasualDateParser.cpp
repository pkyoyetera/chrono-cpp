
#include "gtest/gtest.h"
#include "src/parsers/en/ENCasualDateParser.h"

using namespace std;


class ENCasualDateTest : public ::testing::Test {
protected:

    ENCasualDateParser dateParser;
    tm anchor;
    string t1, t2, t3, t4, t5;
    Result results;

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
    // posix_time::ptime anchor( posix_time::second_clock::local_time() );

    string testRefDate("2018-02-13 18:16:27");
    posix_time::ptime anchor (posix_time::time_from_string(testRefDate));

    // test 1: date corresponds to yesterday's

    results = dateParser.execute(t1, anchor);

    EXPECT_EQ(results[0].startDate.getYear(),  anchor.date().year());
    EXPECT_EQ(results[0].startDate.getMonth(), anchor.date().month()) ;
    EXPECT_EQ(results[0].startDate.get_mDay(), anchor.date().day()-1);
    // EXPECT_EQ(results[0].startDate.get_wDay(), );
    // EXPECT_STREQ(results[0].text, t1.substr(0, 10)); // text is protected

    /// todo: place the above tests into a general test case for parsers in general

    /// test 2: date corresponds to tomorrow
    results = dateParser.execute(t2, anchor);
    EXPECT_EQ(results[0].startDate.getYear(),  anchor.date().year());
    EXPECT_EQ(results[0].startDate.getMonth(), anchor.date().month());
    EXPECT_EQ(results[0].startDate.get_mDay(), anchor.date().day() + 1);
    EXPECT_EQ(results[0].startDate.get_wDay(), anchor.date().day_of_week() + 1);

    /// test 4: date should correspond to later today
    results = dateParser.execute(t4, anchor);
    EXPECT_EQ(results[0].startDate.getYear(),  anchor.date().year());
    EXPECT_EQ(results[0].startDate.getMonth(), anchor.date().month());
    EXPECT_EQ(results[0].startDate.get_mDay(), anchor.date().day());
    EXPECT_EQ(results[0].startDate.get_wDay(), anchor.date().day_of_week());

    /// test 5: corresponds to right this moment
    results = dateParser.execute(t5, anchor);
    EXPECT_EQ(results[0].startDate.getYear(),  anchor.date().year());
    EXPECT_EQ(results[0].startDate.getMonth(), anchor.date().month());
    EXPECT_EQ(results[0].startDate.get_mDay(), anchor.date().day());
    EXPECT_EQ(results[0].startDate.get_wDay(), anchor.date().day_of_week());
}

