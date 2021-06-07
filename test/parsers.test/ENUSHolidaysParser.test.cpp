
#include "gtest/gtest.h"
#include "src/parsers/en/ENUSHolidaysParser.hpp"

using namespace std;


class ENUSHolidaysTest : public ::testing::Test {
public:
    string text;
    parse::Result results;
    posix_time::ptime t;
    parse::ParsedResult r;
    parser::ENHolidayParser holidayParser;

    ENUSHolidaysTest() {
        string date{"2019-01-21 23:59:00.00"};
        t = posix_time::time_from_string(date);
    }
    ~ENUSHolidaysTest() { }
};


TEST_F(ENUSHolidaysTest,  days) {
    text = "on thanksgiving day";
    results.clear();
    holidayParser.execute(text, t, results);
    r = results[0];

    EXPECT_EQ(r.getIndex(), 3);
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 11);
    EXPECT_EQ(r.startDate.get_mDay(), 28);

    text = "fireworks on independence day ";
    results.clear();
    holidayParser.execute(text, t, results);
    r = results[0];

    EXPECT_EQ(r.getIndex(), 13);
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 7);
    EXPECT_EQ(r.startDate.get_mDay(), 4);

//    fixme
//    text = "new year's eve";
//    holidayParser.execute(text, t, results);
//    r = results[0];
//
//    EXPECT_EQ(r.startDate.getYear(), 2019);
//    EXPECT_EQ(r.startDate.getMonth(), 12);
//    EXPECT_EQ(r.startDate.get_mDay(), 31);

    text = "new year's day";
    results.clear();
    holidayParser.execute(text, t, results);
    r = results[0];

    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 1);
    EXPECT_EQ(r.startDate.get_mDay(), 1);

    text = "when is labor day??";
    results.clear();
    holidayParser.execute(text, t, results);
    r = results[0];

    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 9);
    EXPECT_EQ(r.startDate.get_mDay(), 2);

    text = "on memorial day, we...";
    results.clear();
    holidayParser.execute(text, t, results);
    r = results[0];

    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 5);
    EXPECT_EQ(r.startDate.get_mDay(), 27);

    text = "who celebrates President's day";
    results.clear();
    holidayParser.execute(text, t, results);
    r = results[0];

    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 2);
    EXPECT_EQ(r.startDate.get_mDay(), 18);

    text = "what is columbus day?";
    results.clear();
    holidayParser.execute(text, t, results);
    r = results[0];

    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 10);
    EXPECT_EQ(r.startDate.get_mDay(), 14);

    text = "mlk day";
    results.clear();
    holidayParser.execute(text, t, results);
    r = results[0];

    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 1);
    EXPECT_EQ(r.startDate.get_mDay(), 21);

    text = "Martin Luther King Jr day";
    results.clear();
    holidayParser.execute(text, t, results);
    r = results[0];

    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 1);
    EXPECT_EQ(r.startDate.get_mDay(), 21);

}

