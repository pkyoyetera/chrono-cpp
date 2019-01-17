
#include <iostream>

#include "src/result.h"
#include "gtest/gtest.h"

using namespace std;


class Result__Test: public ::testing::Test {
public:
    parse::ParsedComponents pc1;
    parse::ParsedComponents today;
    parse::ParsedResult pr;

    Result__Test() { }

    virtual void SetUp() {
        today.setYear(2018);
        today.setMonth(12);
        today.set_mDay(5);
        today.set_wDay(3);
        today.setHour(12);
        today.setMinute(10);
        today.setSeconds(45);
    }

    virtual void TearDown() {

    }

    ~Result__Test() = default;
};


TEST_F(Result__Test, PC__Test) {

    EXPECT_EQ(pc1.getHour(), 0);
    EXPECT_EQ(pc1.getYear(), 0);
    EXPECT_EQ(pc1.getMinute(), 0);
    EXPECT_EQ(pc1.getMonth(), 0);
    EXPECT_EQ(pc1.get_mDay(), 0);
    EXPECT_EQ(pc1.get_wDay(), 0);

    parse::ParsedComponents pc2(today);

    EXPECT_EQ(pc2.getHour(), today.getHour());
    EXPECT_EQ(pc2.getYear(), today.getYear());
    EXPECT_EQ(pc2.getMinute(), today.getMinute());
    EXPECT_EQ(pc2.getMonth(), today.getMonth());
    EXPECT_EQ(pc2.get_mDay(), today.get_mDay());
    EXPECT_EQ(pc2.get_wDay(), today.get_wDay());
    EXPECT_EQ(pc2.getSeconds(), today.getSeconds());
}


TEST_F(Result__Test, PR__Test) {
    // parse::ParsedResult pr;

    EXPECT_FALSE(pr.hasPossibleDates());
    ASSERT_EQ(pr.startDate.get_mDay(), 0);
    ASSERT_EQ(pr.startDate.get_wDay(), 0);
    ASSERT_EQ(pr.startDate.getMonth(), 0);
    ASSERT_EQ(pr.startDate.getYear(), 0);
    ASSERT_EQ(pr.startDate.getHour(), 0);

//    string sample = "test string";
//    parse::ParsedResult pr1(today, 15, sample);
//
//    EXPECT_EQ(pr1.startDate.getHour(), 0);
//    EXPECT_EQ(pr1.startDate.getYear(), 0);
//    // ASSERT_EQ(pr.anchor.getYear, today["year"].second);
    // todo: tests for operator=...IMPORTANT
}