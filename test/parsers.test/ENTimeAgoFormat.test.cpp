

#include "gtest/gtest.h"
#include "src/parsers/en/ENTimeAgoFormatParser.hpp"

using namespace std;

class ENTimeAgoTest : public ::testing::Test {
public:
    string text;
    Result results;
    posix_time::ptime t;
    parse::ParsedResult r;
    parser::ENTimeAgoFormatParser timeAgoParser;

    ENTimeAgoTest() {
        string date{"2019-01-21 23:59:00.00"};
        t = posix_time::time_from_string(date);
    }
};


TEST_F(ENTimeAgoTest, t1_ago) {
    text = "they left 2 days ago";
    results = timeAgoParser.execute(text, t);
    ASSERT_FALSE(results.empty());

    r = results[0];

    EXPECT_EQ(r.getIndex(), 10);
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 1);
    EXPECT_EQ(r.startDate.get_mDay(), 19);

    text = "eight years ago";
    results = timeAgoParser.execute(text, t);
    r = results[0];
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(r.startDate.getYear(), 2011);
    EXPECT_EQ(r.startDate.getMonth(), 01);
    EXPECT_EQ(r.startDate.get_mDay(), 21);

    text = "39 minutes earlier";
    results = timeAgoParser.execute(text, t);
    r = results[0];
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(r.startDate.getYear(), 2019);
    EXPECT_EQ(r.startDate.getMonth(), 01);
    EXPECT_EQ(r.startDate.get_mDay(), 21);
    EXPECT_EQ(r.startDate.getHour(), 23);
    EXPECT_EQ(r.startDate.getMinute(), 20);
    EXPECT_EQ(r.startDate.getSeconds(), 00);
}

TEST_F(ENTimeAgoTest, t2) {
    text = "";

}