
#include "gtest/gtest.h"
#include "src/parsers/en/ENTimeExpressionParser.hpp"

using namespace std;


class ENTimeExpTest : public ::testing::Test {
protected:

    string text;
    Result results;
    posix_time::ptime t;
    parse::ParsedResult r;
    ENTimeExpressionParser timeExpressionParser;

    ENTimeExpTest() {
        string date{"2019-01-21 23:59:00.00"};
        t = posix_time::time_from_string(date);
    }

    ~ENTimeExpTest() = default;
};

TEST_F(ENTimeExpTest, time_words) {
    text = "at noon";
    results = timeExpressionParser.execute(text, t);
    r = results[0];
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 1);
    EXPECT_EQ(r.startDate.get_mDay(), 21);
    EXPECT_EQ(r.startDate.getHour(), 12);
    EXPECT_EQ(r.startDate.getMinute(), 0);

    text = "let's get together around midnight.";
    results = timeExpressionParser.execute(text, t);
    r = results[0];
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 1);
    EXPECT_EQ(r.startDate.get_mDay(), 21);
    EXPECT_EQ(r.startDate.getHour(), 0);
    EXPECT_EQ(r.startDate.getMinute(), 0);
}

TEST_F(ENTimeExpTest, failures) {
    text = "25:12";
    results = timeExpressionParser.execute(text, t);
    EXPECT_EQ(results.size(), 0);

    text = "02:69";
    results = timeExpressionParser.execute(text, t);
    EXPECT_EQ(results.size(), 0);

    text = "14:12 p.m";
    results = timeExpressionParser.execute(text, t);
    EXPECT_EQ(results.size(), 0);
}

TEST_F(ENTimeExpTest, using_from) {
    text = "it'll be running from 4:12 p.m";
    results = timeExpressionParser.execute(text, t);
    EXPECT_EQ(results.size(), 1);

    r = results[0];
    EXPECT_EQ(r.startDate.getHour(), 16);
    EXPECT_EQ(r.startDate.getMinute(), 12);
}


TEST_F(ENTimeExpTest, ampm) {
    text = "starting at 4:57 p.m";
    results = timeExpressionParser.execute(text, t);
    EXPECT_EQ(results.size(), 1);

    r = results[0];
    EXPECT_EQ(r.startDate.getHour(), 16);
    EXPECT_EQ(r.startDate.getMinute(), 57);

    text = "11am";
    results = timeExpressionParser.execute(text, t);
    r = results[0];
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(r.startDate.getHour(), 11);
    EXPECT_EQ(r.startDate.getMinute(), 0);

    text = "11 am";
    results = timeExpressionParser.execute(text, t);
    r = results[0];
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(r.startDate.getHour(), 11);
    EXPECT_EQ(r.startDate.getMinute(), 0);

    text = "11 a.m";
    results = timeExpressionParser.execute(text, t);
    r = results[0];
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(r.startDate.getHour(), 11);
    EXPECT_EQ(r.startDate.getMinute(), 0);

    text = "11a.m.";
    results = timeExpressionParser.execute(text, t);
    r = results[0];
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(r.startDate.getHour(), 11);
    EXPECT_EQ(r.startDate.getMinute(), 0);

    text = "8 o'clock";
    results = timeExpressionParser.execute(text, t);
    r = results[0];
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(r.startDate.getHour(), 8);
    EXPECT_EQ(r.startDate.getMinute(), 0);

    text = "6:00 p.m";
    results = timeExpressionParser.execute(text, t);
    r = results[0];
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(r.startDate.getHour(), 18);
    EXPECT_EQ(r.startDate.getMinute(), 0);

    text = "9:11 p.m.";
    results = timeExpressionParser.execute(text, t);
    r = results[0];
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(r.startDate.getHour(), 21);
    EXPECT_EQ(r.startDate.getMinute(), 11);
}


TEST_F(ENTimeExpTest, range_expression) {
    text = "from 4:57 p.m to 6:50 p.m.";
    results = timeExpressionParser.execute(text, t);
    r = results[0];
    EXPECT_EQ(results.size(), 1);
    EXPECT_TRUE(r.end());
    EXPECT_EQ(r.endDate.getHour(), 18);
    EXPECT_EQ(r.endDate.getMinute(), 50);

    text = "from 4:57am - 6:50am";
    results = timeExpressionParser.execute(text, t);
    r = results[0];
    EXPECT_EQ(results.size(), 1);
    EXPECT_TRUE(r.end());
    EXPECT_EQ(r.startDate.getHour(), 4);
    EXPECT_EQ(r.startDate.getMinute(), 57);
    EXPECT_EQ(r.endDate.getHour(), 6);
    EXPECT_EQ(r.endDate.getMinute(), 50);

}

TEST_F(ENTimeExpTest, asserted_meridian) {
    text = "from 4:57 p.m to 6:50";
    results = timeExpressionParser.execute(text, t);
    r = results[0];
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(r.startDate.getHour(), 16);
    EXPECT_EQ(r.startDate.getMinute(), 57);
    EXPECT_TRUE(r.end());
    EXPECT_EQ(r.endDate.getHour(), 18);
    EXPECT_EQ(r.endDate.getMinute(), 50);
}