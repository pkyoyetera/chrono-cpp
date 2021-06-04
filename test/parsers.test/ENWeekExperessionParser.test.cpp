
#include "gtest/gtest.h"
#include "src/parsers/en/ENWeekExperessionParser.hpp"

using namespace std;

class ENWeekExpressionTest : public ::testing::Test {
public:
    string text;
    Result results;
    posix_time::ptime t;
    parse::ParsedResult r;
    parser::ENWeekExpressionParser weekExpressionParser;

    ENWeekExpressionTest() {
        string date{"2019-01-28 23:59:00.00"};
        t = posix_time::time_from_string(date);
    }
    ~ENWeekExpressionTest() = default;
};


TEST_F(ENWeekExpressionTest , t1) {
    text = "next two weeks are crucial";
    results = weekExpressionParser.execute(text, t);
    r = results[0];
    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 2);
    EXPECT_EQ(r.startDate.get_mDay(), 11);

    text = "the last two weeks have been a slugger";
    results = weekExpressionParser.execute(text, t);
    r = results[0];
    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 1);
    EXPECT_EQ(r.startDate.get_mDay(), 14);

    text = "next 2 weeks are crucial";
    results = weekExpressionParser.execute(text, t);
    r = results[0];
    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 2);
    EXPECT_EQ(r.startDate.get_mDay(), 11);

    text = "the last 2 weeks have been a slugger";
    results = weekExpressionParser.execute(text, t);
    r = results[0];
    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 1);
    EXPECT_EQ(r.startDate.get_mDay(), 14);

    text = "Meet next week then?";
    results = weekExpressionParser.execute(text, t);
    r = results[0];
    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 2);
    EXPECT_EQ(r.startDate.get_mDay(), 4);
}