

#include "gtest/gtest.h"
#include "src/parsers/en/ENMonthNameParser.hpp"

using namespace std;


class ENMonthNameTest : public ::testing::Test {
public:
    string text;
    Result results;
    posix_time::ptime t;
    parse::ParsedResult r;
    ENMonthNameParser monthParser;

    ENMonthNameTest() {
        string date{"2019-01-21 23:59:00.00"};
        t = posix_time::time_from_string(date);
    }
    ~ENMonthNameTest() { }
};


TEST_F(ENMonthNameTest, t1) {
    text = "September 2025";
    results = monthParser.execute(text, t);
    r = results[0];
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(r.startDate.getYear(), 2025);
    EXPECT_EQ(r.startDate.getMonth(), 9);
    EXPECT_EQ(r.startDate.get_mDay(), 1);

    text = "Sep 2025";
    results = monthParser.execute(text, t);
    r = results[0];
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(r.startDate.getYear(), 2025);
    EXPECT_EQ(r.startDate.getMonth(), 9);
    EXPECT_EQ(r.startDate.get_mDay(), 1);

    text = "September, 2025";
    results = monthParser.execute(text, t);
    r = results[0];
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(r.startDate.getYear(), 2025);
    EXPECT_EQ(r.startDate.getMonth(), 9);
    EXPECT_EQ(r.startDate.get_mDay(), 1);

    text = "Sept-2025";
    results = monthParser.execute(text, t);
    r = results[0];
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(r.startDate.getYear(), 2025);
    EXPECT_EQ(r.startDate.getMonth(), 9);
    EXPECT_EQ(r.startDate.get_mDay(), 1);

    text = "Sept. 2025";
    results = monthParser.execute(text, t);
    r = results[0];
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(r.startDate.getYear(), 2025);
    EXPECT_EQ(r.startDate.getMonth(), 9);
    EXPECT_EQ(r.startDate.get_mDay(), 1);
}
