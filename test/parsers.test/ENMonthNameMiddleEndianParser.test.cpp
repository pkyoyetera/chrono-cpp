
#include "gtest/gtest.h"
#include "src/parsers/en/ENMonthNameMiddleEndianParser.hpp"

using namespace std;


class ENMiddleEndianTest : public ::testing::Test {
public:
    ENMonthNameMiddleEndianParser middleEndianParser;
    Result results;
    posix_time::ptime t;

    ENMiddleEndianTest() {
        string date{"2019-01-21 23:59:00.00"};
        t = posix_time::time_from_string(date);
    }

};


TEST_F(ENMiddleEndianTest, t1) {
    string text{"Jan 12"};
    results = middleEndianParser.execute(text, t);

    EXPECT_EQ(results[0].startDate.getYear(), 2019);
    EXPECT_EQ(results[0].startDate.getMonth(), 01);
    EXPECT_EQ(results[0].startDate.get_mDay(), 12);

    text = "July 24 - 27";
    results = middleEndianParser.execute(text, t);

    EXPECT_EQ(results[0].startDate.getYear(), 2018);
    EXPECT_EQ(results[0].startDate.getMonth(), 7);
    EXPECT_EQ(results[0].startDate.get_mDay(), 24);
    EXPECT_TRUE(results[0].end());
    EXPECT_EQ(results[0].endDate.getYear(), 2018);
    EXPECT_EQ(results[0].endDate.getMonth(), 7);
    EXPECT_EQ(results[0].endDate.get_mDay(), 27);
}
