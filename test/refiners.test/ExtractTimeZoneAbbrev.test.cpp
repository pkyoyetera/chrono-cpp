
#include "gtest/gtest.h"
#include "src/parsers/en/ENTimeExpressionParser.hpp"
#include "src/refiners/ExtractTimeZoneAbbreviation.hpp"

using namespace std;


class ExtractTimeZone : public ::testing::Test {
public:
    parser::ENTimeExpressionParser parser;
    ExtractTimeZoneAbbreviation extractor;
    Result res;
    posix_time::ptime t;
    parse::ParsedResult r;
    string text;

    ExtractTimeZone() {
        string date{"2019-01-21 23:59:00.00"};
        t = posix_time::time_from_string(date);
    }
    ~ExtractTimeZone() = default;
};


TEST_F(ExtractTimeZone, test1) {
    text = "16:59 EST";
    parser.execute(text, t, res);
    res = extractor.refine(res, text);
    parse::ParsedResult r = res[0];

    EXPECT_TRUE(r.getTag(utils::ExtractTimeZoneAbbreviation));
    EXPECT_EQ(r.startDate.getHour(), 16);
    EXPECT_EQ(r.startDate.getMinute(), 59);
    EXPECT_EQ(r.startDate.getTimeZoneOffset(),
            utils::DEFAULT_TIMEZONE_ABBR_MAP["EST"]);
    // EXPECT_STREQ(text.substr(6, 9), )

    /*text = "16:59 EST";
    res = extractor.refine(parser.execute(text, t, results), text);
    parse::ParsedResult r = res[0];

    EXPECT_TRUE(r.getTag(utils::ExtractTimeZoneAbbreviation));
    EXPECT_EQ(r.startDate.getHour(), 16);
    EXPECT_EQ(r.startDate.getMinute(), 59);*/
}