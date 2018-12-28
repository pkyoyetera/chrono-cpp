#include <iostream>
#include "gtest/gtest.h"
#include "src/parsers/en/ENDeadlineFormatParser.hpp"

using namespace std;


class ENDeadlineFormatTest : public ::testing::Test {
public:
    ENDeadlineFormatParser dp;
    std::vector<parse::ParsedResult> results;
    posix_time::ptime t;
    ENDeadlineFormatTest() : t{posix_time::second_clock::local_time()} { }

    ~ENDeadlineFormatTest() { }
};

TEST_F (ENDeadlineFormatTest, test1) {
    string text{"I'll be home within six days!"};
    posix_time::ptime testTime{t + gregorian::days(6)};
    results = dp.execute(text, t);

    EXPECT_EQ(results[0].startDate.getYear(), testTime.date().year());
    EXPECT_EQ(results[0].startDate.getMonth(), testTime.date().month());
    EXPECT_EQ(results[0].startDate.get_mDay(), testTime.date().day());
}

TEST_F(ENDeadlineFormatTest, test2) {
    string text{"in a few years, the greedy will crumble"};
    posix_time::ptime testTime{t + gregorian::years(3)};
    results = dp.execute(text, t);

    EXPECT_EQ(results[0].startDate.getYear(), testTime.date().year());
    EXPECT_EQ(results[0].startDate.getMonth(), testTime.date().month());
    EXPECT_EQ(results[0].startDate.get_mDay(), testTime.date().day());
}

TEST_F(ENDeadlineFormatTest, test3) {
    string text{"Depot closes in 5 minutes"};
    posix_time::ptime testTime{t + posix_time::minutes(5)};
    results = dp.execute(text, t);

    EXPECT_EQ(results[0].startDate.getYear(), testTime.date().year());
    EXPECT_EQ(results[0].startDate.getMonth(), testTime.date().month());
    EXPECT_EQ(results[0].startDate.get_mDay(), testTime.date().day());

    EXPECT_EQ(results[0].startDate.getHour(), testTime.time_of_day().hours());
    EXPECT_EQ(results[0].startDate.getMinute(), testTime.time_of_day().minutes());
    EXPECT_EQ(results[0].startDate.getSeconds(), testTime.time_of_day().seconds());
}

TEST_F(ENDeadlineFormatTest, test4) {
    string text{"It shall be built in a month, be patient"};
    posix_time::ptime testTime{t + gregorian::months(1)};
    results = dp.execute(text, t);

    EXPECT_EQ(results[0].startDate.getYear(), testTime.date().year());
    EXPECT_EQ(results[0].startDate.getMonth(), testTime.date().month());
    EXPECT_EQ(results[0].startDate.get_mDay(), testTime.date().day());
}

/*
TEST_F(ENDeadlineFormatTest, test5) {
    string text{"I can finish it in half an hour"};
    posix_time::ptime testTime{t + posix_time::minutes(30)};
    results = dp.execute(text, t);

    EXPECT_EQ(results[0].startDate.getYear(), testTime.date().year());
    EXPECT_EQ(results[0].startDate.getMonth(), testTime.date().month());
    EXPECT_EQ(results[0].startDate.get_mDay(), testTime.date().day());

    EXPECT_EQ(results[0].startDate.getHour(), testTime.time_of_day().hours());
    EXPECT_EQ(results[0].startDate.getMinute(), testTime.time_of_day().minutes());
    EXPECT_EQ(results[0].startDate.getSeconds(), testTime.time_of_day().seconds());
}

// something tricky with this test come back and finish
*/