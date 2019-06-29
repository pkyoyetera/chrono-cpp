
#include "gtest/gtest.h"
#include "src/parsers/en/ENRelativeDateFormatParser.hpp"

using namespace std;


class ENRelativeFormatTest : public ::testing::Test {
public:
	parse::ENRelativeDateFormatParser relativeDateFormatParser;
	Result results;
	parse::ParsedResult r;
	posix_time::ptime t;

	ENRelativeFormatTest()  {
		std::string s = "2019-02-01 20:59:00.00";
		t = posix_time::time_from_string(s);
	}

};

TEST_F(ENRelativeFormatTest, test1_this) {
	string text{"this week"};
	results = relativeDateFormatParser.execute(text, t);
	r = results[0];
	EXPECT_EQ(r.getIndex(), 0);
	EXPECT_EQ(r.startDate.getYear(), 2019);
	EXPECT_EQ(r.startDate.getMonth(), 1);
	EXPECT_EQ(r.startDate.get_mDay(), 27);
	EXPECT_EQ(r.endDate.getYear(), 2019);
	EXPECT_EQ(r.endDate.getMonth(), 2);
	EXPECT_EQ(r.endDate.get_mDay(), 2);

	text = "it's happening this month";
	results = relativeDateFormatParser.execute(text, t);
	r = results[14];
	EXPECT_EQ(r.getIndex(), 0);
	EXPECT_EQ(r.startDate.getYear(), 2019);
	EXPECT_EQ(r.startDate.getMonth(), 2);
	EXPECT_EQ(r.startDate.get_mDay(), 1);
	EXPECT_EQ(r.endDate.getYear(), 2019);
	EXPECT_EQ(r.endDate.getMonth(), 2);
	EXPECT_EQ(r.endDate.get_mDay(), 28);

	text = "i'll be out this year";
	results = relativeDateFormatParser.execute(text, t);
	r = results[0];
	EXPECT_EQ(r.getIndex(), 11);
	EXPECT_EQ(r.startDate.getYear(), 2019);
	EXPECT_EQ(r.startDate.getMonth(), 1);
	EXPECT_EQ(r.startDate.get_mDay(), 1);
	EXPECT_EQ(r.endDate.getYear(), 2019);
	EXPECT_EQ(r.endDate.getMonth(), 12);
	EXPECT_EQ(r.endDate.get_mDay(), 31);
}

TEST_F(ENRelativeFormatTest, test1_next) {
	string text{"out next week"};
	results = relativeDateFormatParser.execute(text, t);
	r = results[0];
	EXPECT_EQ(r.getIndex(), 3);
	EXPECT_EQ(r.startDate.getYear(), 2019);
	EXPECT_EQ(r.startDate.getMonth(), 2);
	EXPECT_EQ(r.startDate.get_mDay(), 8);

	text = "the next 5 weeks are cold";
	results = relativeDateFormatParser.execute(text, t);
	r = results[0];
	EXPECT_EQ(r.getIndex(), 3);
	EXPECT_EQ(r.startDate.getYear(), 2019);
	EXPECT_EQ(r.startDate.getMonth(), 3);
	EXPECT_EQ(r.startDate.get_mDay(), 8);

	text = "start next month";
	results = relativeDateFormatParser.execute(text, t);
	r = results[0];
	EXPECT_EQ(r.getIndex(), 5);
	EXPECT_EQ(r.startDate.getYear(), 2019);
	EXPECT_EQ(r.startDate.getMonth(), 3);
	EXPECT_EQ(r.startDate.get_mDay(), 1);

	text = "next 5 months";
	results = relativeDateFormatParser.execute(text, t);
	r = results[0];
	EXPECT_EQ(r.getIndex(), 0);
	EXPECT_EQ(r.startDate.getYear(), 2019);
	EXPECT_EQ(r.startDate.getMonth(), 7);
	EXPECT_EQ(r.startDate.get_mDay(), 1);

	text = "how about next year bruh?";
	results = relativeDateFormatParser.execute(text, t);
	r = results[0];
	EXPECT_EQ(r.getIndex(), 9);
	EXPECT_EQ(r.startDate.getYear(), 2020);
	EXPECT_EQ(r.startDate.getMonth(), 2);
	EXPECT_EQ(r.startDate.get_mDay(), 1);

	text = "next 10 years";
	results = relativeDateFormatParser.execute(text, t);
	r = results[0];
	EXPECT_EQ(r.getIndex(), 0);
	EXPECT_EQ(r.startDate.getYear(), 2029);
	EXPECT_EQ(r.startDate.getMonth(), 2);
	EXPECT_EQ(r.startDate.get_mDay(), 1);
}


TEST_F(ENRelativeFormatTest, test1_last) {
	string text{"out last week"};
	results = relativeDateFormatParser.execute(text, t);
	r = results[0];
	EXPECT_EQ(r.getIndex(), 3);
	EXPECT_EQ(r.startDate.getYear(), 2019);
	EXPECT_EQ(r.startDate.getMonth(), 1);
	EXPECT_EQ(r.startDate.get_mDay(), 25);

	text = "the last 5 weeks were cold";
	results = relativeDateFormatParser.execute(text, t);
	r = results[0];
	EXPECT_EQ(r.getIndex(), 3);
	EXPECT_EQ(r.startDate.getYear(), 2018);
	EXPECT_EQ(r.startDate.getMonth(), 12);
	EXPECT_EQ(r.startDate.get_mDay(), 28);

	text = "started last month";
	results = relativeDateFormatParser.execute(text, t);
	r = results[0];
	EXPECT_EQ(r.getIndex(), 7);
	EXPECT_EQ(r.startDate.getYear(), 2019);
	EXPECT_EQ(r.startDate.getMonth(), 1);
	EXPECT_EQ(r.startDate.get_mDay(), 1);

	text = "the last 5 months";
	results = relativeDateFormatParser.execute(text, t);
	r = results[0];
	EXPECT_EQ(r.getIndex(), 3);
	EXPECT_EQ(r.startDate.getYear(), 2018);
	EXPECT_EQ(r.startDate.getMonth(), 9);
	EXPECT_EQ(r.startDate.get_mDay(), 1);

	text = "how about last year bruh?";
	results = relativeDateFormatParser.execute(text, t);
	r = results[0];
	EXPECT_EQ(r.getIndex(), 9);
	EXPECT_EQ(r.startDate.getYear(), 2018);
	EXPECT_EQ(r.startDate.getMonth(), 2);
	EXPECT_EQ(r.startDate.get_mDay(), 1);

	text = "last 10 years";
	results = relativeDateFormatParser.execute(text, t);
	r = results[0];
	EXPECT_EQ(r.getIndex(), 0);
	EXPECT_EQ(r.startDate.getYear(), 2009);
	EXPECT_EQ(r.startDate.getMonth(), 2);
	EXPECT_EQ(r.startDate.get_mDay(), 1);
}
