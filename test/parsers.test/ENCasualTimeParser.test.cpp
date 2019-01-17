
#include <iostream>
#include "gtest/gtest.h"
#include "src/parsers/en/ENCasualTimeParser.hpp"

using namespace std;
using std::atoi;


class ENCasualTimeTest : public ::testing::Test {
public:
    ENCasualTimeParser ps;
    tm anchor;
    string text1, text2, text3, text4, text5;
    Result results;

    ENCasualTimeTest() {
        text1 = "What about this morning!";
        text2 = "This afternoon was unusually cold";
        text3 = "the evening sunset is noice";
        text4 = "How does noon today sound?";
        text5 = "Eventually the night is calm";
    }

    ~ENCasualTimeTest() { }
};


TEST_F(ENCasualTimeTest, test1) {
    posix_time::ptime t( posix_time::second_clock::local_time() );
    anchor = posix_time::to_tm(t);

    results = ps.execute(text1, t);

    EXPECT_EQ(results[0].startDate.getYear(), t.date().year());
    EXPECT_EQ(results[0].startDate.getMonth(), t.date().month());
    EXPECT_EQ(results[0].startDate.get_mDay(), t.date().day());
    EXPECT_EQ(results[0].startDate.get_wDay(), t.date().day_of_week());
    ASSERT_EQ(results[0].startDate.getHour(), 06);

    results = ps.execute(text2, t);
    ASSERT_EQ(results[0].startDate.getHour(), 15);

    results = ps.execute(text3, t);
    ASSERT_EQ(results[0].startDate.getHour(), 20);

    results = ps.execute(text4, t);
    ASSERT_EQ(results[0].startDate.getHour(), 12);

    results = ps.execute(text5, t);
    ASSERT_EQ(results[0].startDate.getHour(), 20);
}


TEST_F(ENCasualTimeTest, test2) {
    string testRefDate("2018-02-13 18:16:27");
    posix_time::ptime t (posix_time::time_from_string(testRefDate));
    anchor = posix_time::to_tm(t);

    results = ps.execute(text1, t);

    EXPECT_EQ(results[0].startDate.getYear(), t.date().year());
    EXPECT_EQ(results[0].startDate.getMonth(), t.date().month());
    EXPECT_EQ(results[0].startDate.get_mDay(), t.date().day());
    EXPECT_EQ(results[0].startDate.get_wDay(), anchor.tm_wday);
    ASSERT_EQ(results[0].startDate.getHour(), 06);

    results = ps.execute(text2, t);
    ASSERT_EQ(results[0].startDate.getHour(), 15);

    results = ps.execute(text3, t);
    ASSERT_EQ(results[0].startDate.getHour(), 20);

    results = ps.execute(text4, t);
    ASSERT_EQ(results[0].startDate.getHour(), 12);

    results = ps.execute(text5, t);
    ASSERT_EQ(results[0].startDate.getHour(), 20);

}
