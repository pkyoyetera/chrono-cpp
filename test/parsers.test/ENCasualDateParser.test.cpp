
#include "gtest/gtest.h"
#include "src/parsers/en/ENCasualDateParser.hpp"

using namespace std;


class ENCasualDateTest : public ::testing::Test {
protected:

    ENCasualDateParser dateParser;
    string t1, t2, t3, t4, t5;
    Result results;
    parse::ParsedResult r;

    ENCasualDateTest() {
        t1 = "What happened last NIGHT bro?";
        t2 = "Was tomorrow morning even acceptable?";
        t3 = "";
        t4 = "Later tonight then? ";
        t5 = "RIGHT NOW!!!";
    }

    ~ENCasualDateTest() = default;

};


TEST_F(ENCasualDateTest, construct) {
    string testRefDate("2018-02-13 18:16:27");
    posix_time::ptime anchor (posix_time::time_from_string(testRefDate));

    // test 1: date corresponds to yesterday's
    results = dateParser.execute(t1, anchor);
    r = results[0];

    EXPECT_EQ(r.getIndex(), 14);
    EXPECT_EQ(r.startDate.getYear(),  anchor.date().year());
    EXPECT_EQ(r.startDate.getMonth(), anchor.date().month()) ;
    EXPECT_EQ(r.startDate.get_mDay(), anchor.date().day()-1);
    EXPECT_EQ(r.startDate.getHour(), 0);

    /// test 2: date corresponds to tomorrow
    results = dateParser.execute(t2, anchor);
    r = results[0];
    EXPECT_EQ(r.startDate.getYear(),  anchor.date().year());
    EXPECT_EQ(r.startDate.getMonth(), anchor.date().month());
    EXPECT_EQ(r.startDate.get_mDay(), anchor.date().day() + 1);

    // test 4: date should correspond to later today
    results = dateParser.execute(t4, anchor);
    r = results[0];
    EXPECT_EQ(r.startDate.getYear(),  anchor.date().year());
    EXPECT_EQ(r.startDate.getMonth(), anchor.date().month());
    EXPECT_EQ(r.startDate.get_mDay(), anchor.date().day());
    EXPECT_EQ(r.startDate.getHour(), 22);

    // test 5: corresponds to right this moment
    results = dateParser.execute(t5, anchor);
    r = results[0];
    EXPECT_EQ(r.startDate.getYear(),  anchor.date().year());
    EXPECT_EQ(r.startDate.getMonth(), anchor.date().month());
    EXPECT_EQ(r.startDate.get_mDay(), anchor.date().day());
    EXPECT_EQ(r.startDate.getHour(),  anchor.time_of_day().hours());
    EXPECT_EQ(r.startDate.getMinute(), anchor.time_of_day().minutes());
    EXPECT_EQ(r.startDate.getSeconds(), anchor.time_of_day().seconds());
 }

