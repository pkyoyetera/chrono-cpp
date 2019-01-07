#include <iostream>
#include "gtest/gtest.h"
#include "src/parsers/en/ENDayOfTheWeekParser.cpp"

using namespace std;

class ENDayOfTheWeekTest : public ::testing::Test {
public:
    ENDayOfWeekParser dwp;
    Result results;
    posix_time::ptime t;

    ENDayOfTheWeekTest() {
        std::string s = "2019-01-20 23:59:00.00";
        t = posix_time::time_from_string(s);
    }
    ~ENDayOfTheWeekTest() { }

};

TEST_F(ENDayOfTheWeekTest, test1) {
    string text{"Monday"};
    results = dwp.execute(text, t);

    EXPECT_EQ(results[0].startDate.getYear(), 2019);
    EXPECT_EQ(results[0].startDate.getMonth(), 1);
    EXPECT_EQ(results[0].startDate.get_mDay(), 21);

    text = "Friday";
    results = dwp.execute(text, t);

    EXPECT_EQ(results[0].startDate.getYear(), 2019);
    EXPECT_EQ(results[0].startDate.getMonth(), 1);
    EXPECT_EQ(results[0].startDate.get_mDay(), 25);

    text = "Sunday";
    results = dwp.execute(text, t);

    EXPECT_EQ(results[0].startDate.getYear(), 2019);
    EXPECT_EQ(results[0].startDate.getMonth(), 1);
    EXPECT_EQ(results[0].startDate.get_mDay(), 20);
}