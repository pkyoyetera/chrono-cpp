#include <iostream>

#include "gtest/gtest.h"

#include "src/parsers/en/ENDayOfTheWeekParser.hpp"

using namespace std;

class ENDayOfTheWeekTest : public ::testing::Test {
public:
    parser::ENDayOfWeekParser dwp;
    Result results;
    posix_time::ptime t;
    parse::ParsedResult r;

    ENDayOfTheWeekTest() {
        std::string s = "2019-01-21 23:59:00.00";
        t = posix_time::time_from_string(s);
    }
    ~ENDayOfTheWeekTest() = default;

};

TEST_F(ENDayOfTheWeekTest, test1) {
    string text{"Monday"};
    results.clear();
    dwp.execute(text, t, results);
    r = results.at(0);
    EXPECT_EQ(r.getIndex(), 0);
    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 1);
    EXPECT_EQ(r.startDate.get_mDay(), 21);

    text = "Friday";
    results.clear();
    dwp.execute(text, t, results);
    r = results[0];
    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 1);
    EXPECT_EQ(r.startDate.get_mDay(), 25);

    text = "Sunday";
    results.clear();
    dwp.execute(text, t, results);
    r = results[0];
    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 1);
    EXPECT_EQ(r.startDate.get_mDay(), 20);
}

TEST_F(ENDayOfTheWeekTest, test2) {
    string text{"this Tuesday"};
    results.clear();
    dwp.execute(text, t, results);
    r = results[0];
    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 1);
    EXPECT_EQ(r.startDate.get_mDay(), 22);

    text = "this Sat";
    results.clear();
    dwp.execute(text, t, results);
    r = results[0];
    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 1);
    EXPECT_EQ(r.startDate.get_mDay(), 26);
}

TEST_F(ENDayOfTheWeekTest, test3) {
    string text{"next thursday"};
    results.clear();
    dwp.execute(text, t, results);
    r = results[0];
    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 1);
    EXPECT_EQ(r.startDate.get_mDay(), 31);

    text = "next Monday";
    results.clear();
    dwp.execute(text, t, results);
    r = results[0];
    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 1);
    EXPECT_EQ(r.startDate.get_mDay(), 28);

}

// works with first day of the week being a saturday
TEST_F(ENDayOfTheWeekTest, test4) {
    string text{"last thursday"};
    results.clear();
    dwp.execute(text, t, results);
    r = results[0];
    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 1);
    EXPECT_EQ(r.startDate.get_mDay(), 17);

    text = "last Sunday";
    results.clear();
    dwp.execute(text, t, results);
    r = results[0];
    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 1);
    EXPECT_EQ(r.startDate.get_mDay(), 13);

    text = "last mon";
    results.clear();
    dwp.execute(text, t, results);
    r = results[0];
    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 1);
    EXPECT_EQ(r.startDate.get_mDay(), 14);
}

TEST_F(ENDayOfTheWeekTest, test5) {
    string text{"Sunday next week"};
    results.clear();
    dwp.execute(text, t, results);
    r = results[0];
    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 1);
    EXPECT_EQ(r.startDate.get_mDay(), 27);

    text = "friday last week";
    results.clear();
    dwp.execute(text, t, results);
    r = results[0];
    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 1);
    EXPECT_EQ(r.startDate.get_mDay(), 18);

    text = "earlier last friday";
    results.clear();
    dwp.execute(text, t, results);
    r = results[0];
    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 1);
    EXPECT_EQ(r.startDate.get_mDay(), 18);
    EXPECT_EQ(r.startDate.getHour(), 6);

    text = "late last friday";
    results.clear();
    dwp.execute(text, t, results);
    r = results[0];
    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 1);
    EXPECT_EQ(r.startDate.get_mDay(), 18);
    EXPECT_EQ(r.startDate.getHour(), 20);
}