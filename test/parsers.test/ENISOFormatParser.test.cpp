

#include "gtest/gtest.h"
#include "src/parsers/en/ENISOFormatParser.hpp"

using namespace std;


class ENISOFormatTest : public ::testing::Test {
public:
    string text;
    parse::Result results;
    posix_time::ptime t;
    parse::ParsedResult r;
    parser::ENISOFormatParser isoParser;

    ENISOFormatTest() {
        string date{"2019-01-21 23:59:00.00"};
        t = posix_time::time_from_string(date);
    }
    ~ENISOFormatTest() = default;
};


TEST_F(ENISOFormatTest, t1) {
    text = "2016-11-17";
    results.clear();
    isoParser.execute(text, t, results);
    r = results[0];

    EXPECT_EQ(r.getIndex(), 0);
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(r.startDate.getYear(), 2016);
    EXPECT_EQ(r.startDate.getMonth(), 11);
    EXPECT_EQ(r.startDate.get_mDay(), 17);

    text = "2022-12-01T08:15:30";
    results.clear();
    isoParser.execute(text, t, results);
    r = results[0];
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(r.startDate.getYear(), 2022);
    EXPECT_EQ(r.startDate.getMonth(), 12);
    EXPECT_EQ(r.startDate.get_mDay(), 1);
    EXPECT_EQ(r.startDate.getHour(), 8);
    EXPECT_EQ(r.startDate.getMinute(), 15);
    EXPECT_EQ(r.startDate.getSeconds(), 30);

    text = "1994-11-05T13:15:30Z";
    results.clear();
    isoParser.execute(text, t, results);
    r = results[0];
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(r.startDate.getYear(), 1994);
    EXPECT_EQ(r.startDate.getMonth(), 11);
    EXPECT_EQ(r.startDate.get_mDay(), 5);
    EXPECT_EQ(r.startDate.getHour(), 13);
    EXPECT_EQ(r.startDate.getMinute(), 15);
    EXPECT_EQ(r.startDate.getSeconds(), 30);

    // todo: tests including parsing of timezones offset element

}

