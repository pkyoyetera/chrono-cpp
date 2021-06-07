

#include "gtest/gtest.h"
#include "src/parsers/en/ENMonthNameParser.hpp"

using namespace std;


class ENMonthNameTest : public ::testing::Test {
public:
    string text;
    parse::Result results;
    posix_time::ptime t;
    parse::ParsedResult r;
    parser::ENMonthNameParser monthParser;

    ENMonthNameTest() {
        string date{"2019-01-21 23:59:00.00"};
        t = posix_time::time_from_string(date);
    }
    ~ENMonthNameTest() { }
};


TEST_F(ENMonthNameTest, t1) {
    text = "September 2025";
    results.clear();
    monthParser.execute(text, t, results);
    r = results[0];
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(r.startDate.getYear(), 2025);
    EXPECT_EQ(r.startDate.getMonth(), 9);
    EXPECT_EQ(r.startDate.get_mDay(), 1);

    text = "Sep 2025";
    results.clear();
    monthParser.execute(text, t, results);
    r = results[0];
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(r.startDate.getYear(), 2025);
    EXPECT_EQ(r.startDate.getMonth(), 9);
    EXPECT_EQ(r.startDate.get_mDay(), 1);

    text = "September, 2025";
    results.clear();
    monthParser.execute(text, t, results);
    r = results[0];
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(r.startDate.getYear(), 2025);
    EXPECT_EQ(r.startDate.getMonth(), 9);
    EXPECT_EQ(r.startDate.get_mDay(), 1);

    text = "Sept-2025";
    results.clear();
    monthParser.execute(text, t, results);
    r = results[0];
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(r.startDate.getYear(), 2025);
    EXPECT_EQ(r.startDate.getMonth(), 9);
    EXPECT_EQ(r.startDate.get_mDay(), 1);

    text = "Sept. 2025";
    results.clear();
    monthParser.execute(text, t, results);
    r = results[0];
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(r.startDate.getYear(), 2025);
    EXPECT_EQ(r.startDate.getMonth(), 9);
    EXPECT_EQ(r.startDate.get_mDay(), 1);
}

TEST_F(ENMonthNameTest, t2) {
    text = "1200 BC";
    results.clear();
    monthParser.execute(text, t, results);

    // todo: complete BC/AD tests


}