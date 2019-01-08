#include <iostream>
#include "gtest/gtest.h"
#include "src/parsers/en/ENDayOfTheWeekParser.hpp"

using namespace std;

class ENDayOfTheWeekTest : public ::testing::Test {
public:
    ENDayOfWeekParser dwp;
    Result results;
    posix_time::ptime t;

    ENDayOfTheWeekTest() {
        std::string s = "2019-01-21 23:59:00.00";
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

TEST_F(ENDayOfTheWeekTest, test2) {
    string text{"this Tuesday"};
    results = dwp.execute(text, t);

    EXPECT_EQ(results[0].startDate.getYear(), 2019);
    EXPECT_EQ(results[0].startDate.getMonth(), 1);
    EXPECT_EQ(results[0].startDate.get_mDay(), 22);

    text = "this Sat";
    results = dwp.execute(text, t);

    EXPECT_EQ(results[0].startDate.getYear(), 2019);
    EXPECT_EQ(results[0].startDate.getMonth(), 1);
    EXPECT_EQ(results[0].startDate.get_mDay(), 26);
}

TEST_F(ENDayOfTheWeekTest, test3) {
    string text{"next thursday"};
    results = dwp.execute(text, t);

    EXPECT_EQ(results[0].startDate.getYear(), 2019);
    EXPECT_EQ(results[0].startDate.getMonth(), 1);
    EXPECT_EQ(results[0].startDate.get_mDay(), 31);

    text = "next Monday";
    results = dwp.execute(text, t);

    EXPECT_EQ(results[0].startDate.getYear(), 2019);
    EXPECT_EQ(results[0].startDate.getMonth(), 1);
    EXPECT_EQ(results[0].startDate.get_mDay(), 28);

}

// works with first day of the week being a saturday
TEST_F(ENDayOfTheWeekTest, test4) {
    string text{"last thursday"};
    results = dwp.execute(text, t);

    EXPECT_EQ(results[0].startDate.getYear(), 2019);
    EXPECT_EQ(results[0].startDate.getMonth(), 1);
    EXPECT_EQ(results[0].startDate.get_mDay(), 17);

    text = "last Sunday";
    results = dwp.execute(text, t);

    EXPECT_EQ(results[0].startDate.getYear(), 2019);
    EXPECT_EQ(results[0].startDate.getMonth(), 1);
    EXPECT_EQ(results[0].startDate.get_mDay(), 13);

    text = "last mon";
    results = dwp.execute(text, t);

    EXPECT_EQ(results[0].startDate.getYear(), 2019);
    EXPECT_EQ(results[0].startDate.getMonth(), 1);
    EXPECT_EQ(results[0].startDate.get_mDay(), 14);
}

