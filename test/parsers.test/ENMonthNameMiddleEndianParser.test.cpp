
#include "gtest/gtest.h"
#include "src/parsers/en/ENMonthNameMiddleEndianParser.hpp"

using namespace std;

class ENMiddleEndianTest : public ::testing::Test {
public:
    string text;
    Result results;
    posix_time::ptime t;
    parse::ParsedResult r;
    parser::ENMonthNameMiddleEndianParser middleEndianParser;

    ENMiddleEndianTest() {
        string date{"2019-01-21 23:59:00.00"};
        t = posix_time::time_from_string(date);
    }

};

TEST_F(ENMiddleEndianTest, t1_month_name_and_date) {
    text = "Jan 12";
    results = middleEndianParser.execute(text, t);
    EXPECT_EQ(results[0].startDate.getYear(), 2019);
    EXPECT_EQ(results[0].startDate.getMonth(), 01);
    EXPECT_EQ(results[0].startDate.get_mDay(), 12);

    text = "November, 3rd";
    results = middleEndianParser.execute(text, t);
    EXPECT_EQ(results[0].startDate.getYear(), 2018);
    EXPECT_EQ(results[0].startDate.getMonth(), 11);
    EXPECT_EQ(results[0].startDate.get_mDay(), 3);

}


// start here
TEST_F(ENMiddleEndianTest, t2_full_and_separators) {
    text = "Tue, Aug. 22, 1934!";
    results = middleEndianParser.execute(text, t);
    r = results[0];

    EXPECT_EQ(r.startDate.getYear(), 1934);
    EXPECT_EQ(r.startDate.getMonth(), 8);
    EXPECT_EQ(r.startDate.get_mDay(), 22);
    EXPECT_EQ(r.startDate.get_wDay(), 2);
}

TEST_F(ENMiddleEndianTest, t3_full_date) {
    text = "Sunday, March 6th, 2020";
    results = middleEndianParser.execute(text, t);
    r = results[0];

    EXPECT_EQ(r.startDate.getYear(), 2020);
    EXPECT_EQ(r.startDate.getMonth(), 03);
    EXPECT_EQ(r.startDate.get_mDay(), 6);
    EXPECT_EQ(r.startDate.get_wDay(), 0);

}

TEST_F(ENMiddleEndianTest, t4_separators) {
    text = "March/6/2020";
    results = middleEndianParser.execute(text, t);
    r = results[0];
    EXPECT_EQ(r.startDate.getYear(), 2020);
    EXPECT_EQ(r.startDate.getMonth(), 03);
    EXPECT_EQ(r.startDate.get_mDay(), 6);
    // EXPECT_EQ(r.startDate.get_wDay(), 0);

    text = "Apr-12-2018";
    results = middleEndianParser.execute(text, t);
    r = results[0];
    EXPECT_EQ(r.startDate.getYear(), 2018);
    EXPECT_EQ(r.startDate.getMonth(), 4);
    EXPECT_EQ(r.startDate.get_mDay(), 12);
    //EXPECT_EQ(r.startDate.get_wDay(), 4);

    text = "Apr-2 2018";
    results = middleEndianParser.execute(text, t);
    r = results[0];
    EXPECT_EQ(r.startDate.getYear(), 2018);
    EXPECT_EQ(r.startDate.getMonth(), 4);
    EXPECT_EQ(r.startDate.get_mDay(), 2);
}


TEST_F(ENMiddleEndianTest, t5_range_date) {
    text = "July 24 - 27";
    results = middleEndianParser.execute(text, t);
    r = results[0];
    EXPECT_EQ(r.startDate.getYear(), 2018);
    EXPECT_EQ(r.startDate.getMonth(), 7);
    EXPECT_EQ(r.startDate.get_mDay(), 24);
    EXPECT_TRUE(r.end());
    EXPECT_EQ(r.endDate.getYear(), 2018);
    EXPECT_EQ(r.endDate.getMonth(), 7);
    EXPECT_EQ(r.endDate.get_mDay(), 27);

    text = "Feb. 18th - 22nd, 2005";
    results = middleEndianParser.execute(text, t);
    r = results[0];
    EXPECT_EQ(r.startDate.getYear(), 2005);
    EXPECT_EQ(r.startDate.getMonth(), 2);
    EXPECT_EQ(r.startDate.get_mDay(), 18);
    EXPECT_TRUE(r.end());
    EXPECT_EQ(r.endDate.getYear(), 2005);
    EXPECT_EQ(r.endDate.getMonth(), 2);
    EXPECT_EQ(r.endDate.get_mDay(), 22);
}