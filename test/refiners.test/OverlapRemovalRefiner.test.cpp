
#include "gtest/gtest.h"
#include "src/refiners/OverlapRemovalRefiner.hpp"


TEST(OverlapRemove, test1) {
    Result test_result;
    std::string _long{"this text is longer than any other"},
                _short{"this text is long"};
    posix_time::ptime t{posix_time::second_clock::local_time()};

    parse::ParsedResult r1(t, 11, _short), r2(t, 15, _long), r3(t, 22, R"(example)");

    test_result.push_back(r1);
    test_result.push_back(r2);

    Refiner* ovr = new OverlapRemover();
    Result new_result = ovr->refine(test_result, _long);

    EXPECT_EQ(new_result.size(), 1);
    for(auto res : new_result){
        EXPECT_FALSE(res.end());
    }

    test_result.pop_back();
    new_result = ovr->refine(test_result, _long);
    EXPECT_EQ(new_result.size(), 1);
    EXPECT_FALSE(new_result[0].end());
    // todo: more tests
}

