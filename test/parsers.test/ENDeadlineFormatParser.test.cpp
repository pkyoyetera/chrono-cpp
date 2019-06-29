#include <iostream>
#include "gtest/gtest.h"
#include "src/parsers/en/ENDeadlineFormatParser.hpp"

using namespace std;


class ENDeadlineFormatTest : public ::testing::Test {
public:
    parser::ENDeadlineFormatParser dp;
    Result results;
    parse::ParsedResult r;
    posix_time::ptime t;
    ENDeadlineFormatTest() : t{posix_time::second_clock::local_time()} { }

    ~ENDeadlineFormatTest() = default;
};

TEST_F (ENDeadlineFormatTest, test1) {
    string text{"I'll be home within six weeks!"};
    posix_time::ptime testTime{t + gregorian::weeks(6)};
    results = dp.execute(text, t);
    r = results[0];

    EXPECT_EQ(r.getIndex(), 12);
    EXPECT_EQ(r.startDate.getYear(), testTime.date().year());
    EXPECT_EQ(r.startDate.getMonth(), testTime.date().month());
    EXPECT_EQ(r.startDate.get_mDay(), testTime.date().day());
}

TEST_F(ENDeadlineFormatTest, test2) {
    string text{"in a few years, the greedy will crumble"};
    posix_time::ptime testTime{t + gregorian::years(3)};
    results = dp.execute(text, t);
    r = results[0];

    EXPECT_EQ(r.getIndex(), 0);
    EXPECT_EQ(r.startDate.getYear(), testTime.date().year());
    EXPECT_EQ(r.startDate.getMonth(), testTime.date().month());
    EXPECT_EQ(r.startDate.get_mDay(), testTime.date().day());
}

TEST_F(ENDeadlineFormatTest, test3) {
    string text{"closes in 5 minutes"};
    posix_time::ptime testTime{t + posix_time::minutes(5)};
    results = dp.execute(text, t);
    r = results[0];

    EXPECT_EQ(r.getIndex(), 6);
    EXPECT_EQ(r.startDate.getYear(), testTime.date().year());
    EXPECT_EQ(r.startDate.getMonth(), testTime.date().month());
    EXPECT_EQ(r.startDate.get_mDay(), testTime.date().day());

    EXPECT_EQ(r.startDate.getHour(), testTime.time_of_day().hours());
    EXPECT_EQ(r.startDate.getMinute(), testTime.time_of_day().minutes());
    EXPECT_EQ(r.startDate.getSeconds(), testTime.time_of_day().seconds());
}

TEST_F(ENDeadlineFormatTest, test4) {
    string text{"It shall be built in 3 months, be patient"};
    posix_time::ptime testTime{t + gregorian::months(3)};
    results = dp.execute(text, t);
    r = results[0];

    EXPECT_EQ(r.getIndex(), 17);
    EXPECT_EQ(r.startDate.getYear(), testTime.date().year());
    EXPECT_EQ(r.startDate.getMonth(), testTime.date().month());
    EXPECT_EQ(r.startDate.get_mDay(), testTime.date().day());
}


TEST_F(ENDeadlineFormatTest, test5) {
    string text{"finish up in half an hour"};
    posix_time::ptime testTime{t + posix_time::minutes(30)};
    results = dp.execute(text, t);
    r = results[0];

    EXPECT_EQ(r.getIndex(), 9);
    EXPECT_EQ(r.startDate.getYear(), testTime.date().year());
    EXPECT_EQ(r.startDate.getMonth(), testTime.date().month());
    EXPECT_EQ(r.startDate.get_mDay(), testTime.date().day());

    EXPECT_EQ(r.startDate.getHour(), testTime.time_of_day().hours());
    EXPECT_EQ(r.startDate.getMinute(), testTime.time_of_day().minutes());
    EXPECT_EQ(r.startDate.getSeconds(), testTime.time_of_day().seconds());
}

TEST_F(ENDeadlineFormatTest, test6) {
    string text{"be back in half a day"};
    posix_time::ptime testTime{t + posix_time::hours(12)};
    results = dp.execute(text, t);
    r = results[0];

    EXPECT_EQ(r.getIndex(), 7);
    EXPECT_EQ(r.startDate.getYear(), testTime.date().year());
    EXPECT_EQ(r.startDate.getMonth(), testTime.date().month());
    EXPECT_EQ(r.startDate.get_mDay(), testTime.date().day());

    EXPECT_EQ(r.startDate.getHour(), testTime.time_of_day().hours());
    EXPECT_EQ(r.startDate.getMinute(), testTime.time_of_day().minutes());
}