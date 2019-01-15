
#include "gtest/gtest.h"
#include "src/parsers/en/ENTimeLaterParser.hpp"

using namespace std;


class ENTimeLaterTest : public ::testing::Test {
public:
    string text;
    Result results;
    posix_time::ptime t;
    parse::ParsedResult r;
    ENTimeLaterParser laterParser;

    ENTimeLaterTest() {
        string date{"2019-01-21 23:59:00.00"};
        t = posix_time::time_from_string(date);
    }
    ~ENTimeLaterTest() { }
};


TEST_F(ENTimeLaterTest, t1_later) {
    text = "2 days later";
    results = laterParser.execute(text, t);
    r = results[0];
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 01);
    EXPECT_EQ(r.startDate.get_mDay(), 23);

    text = "eight years later";
    results = laterParser.execute(text, t);
    r = results[0];
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(r.startDate.getYear(), 2027);
    EXPECT_EQ(r.startDate.getMonth(), 01);
    EXPECT_EQ(r.startDate.get_mDay(), 21);

    text = "39 minutes later";
    results = laterParser.execute(text, t);
    r = results[0];
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 01);
    EXPECT_EQ(r.startDate.get_mDay(), 22);
    EXPECT_EQ(r.startDate.getHour(), 00);
    EXPECT_EQ(r.startDate.getMinute(), 38);
    EXPECT_EQ(r.startDate.getSeconds(), 00);
}

/*
TEST_F(ENTimeLaterTest, t2) {
    text = "4 weeks out";
    results = laterParser.execute(text, t);
    r = results[0];
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 02);
    EXPECT_EQ(r.startDate.get_mDay(), 22);
    EXPECT_EQ(r.startDate.getHour(), 00);
    EXPECT_EQ(r.startDate.getMinute(), 0);
    EXPECT_EQ(r.startDate.getSeconds(), 00);
}*/