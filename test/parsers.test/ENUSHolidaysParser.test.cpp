
#include "gtest/gtest.h"
#include "src/parsers/en/ENUSHolidaysParser.hpp"

using namespace std;


class ENUSHolidaysTest : public ::testing::Test {
public:
    string text;
    Result results;
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
    results = holidayParser.execute(text, t);
    r = results[0];

    EXPECT_EQ(r.getIndex(), 2);
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 11);
    EXPECT_EQ(r.startDate.get_mDay(), 28);

    text = "fireworks on independence day ";
    results = holidayParser.execute(text, t);
    r = results[0];

    EXPECT_EQ(r.getIndex(), 12);
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 7);
    EXPECT_EQ(r.startDate.get_mDay(), 4);

    text = "new year's eve";
    results = holidayParser.execute(text, t);
    r = results[0];

    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 12);
    EXPECT_EQ(r.startDate.get_mDay(), 31);

    text = "new year's day";
    results = holidayParser.execute(text, t);
    r = results[0];

    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 1);
    EXPECT_EQ(r.startDate.get_mDay(), 1);

    text = "when is labor day??";
    results = holidayParser.execute(text, t);
    r = results[0];

    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 9);
    EXPECT_EQ(r.startDate.get_mDay(), 2);

    text = "on memorial day, we...";
    results = holidayParser.execute(text, t);
    r = results[0];

    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 5);
    EXPECT_EQ(r.startDate.get_mDay(), 27);

    text = "who celebrates President's day";
    results = holidayParser.execute(text, t);
    r = results[0];

    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 2);
    EXPECT_EQ(r.startDate.get_mDay(), 18);

    text = "what is columbus day?";
    results = holidayParser.execute(text, t);
    r = results[0];

    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 10);
    EXPECT_EQ(r.startDate.get_mDay(), 14);

    text = "mlk day";
    results = holidayParser.execute(text, t);
    r = results[0];

    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 1);
    EXPECT_EQ(r.startDate.get_mDay(), 21);

    text = "Martin Luther King Jr day";
    results = holidayParser.execute(text, t);
    r = results[0];

    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 1);
    EXPECT_EQ(r.startDate.get_mDay(), 21);

}

