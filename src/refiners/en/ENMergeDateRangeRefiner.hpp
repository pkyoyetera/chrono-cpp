
#include "src/refiners/refiner.hpp"

class ENMergeDateRange : public Refiner {
private:
    // std::regex to{R"(^\s*(and|to|\-)\s*$)"};
    std::regex to{R"(\s*(and|to|\-)\s*)"};

public:
    ENMergeDateRange() = default;
    ~ENMergeDateRange() = default;

    bool isAbleToMerge(std::string,
            parse::ParsedResult, parse::ParsedResult);
    parse::ParsedResult mergeResult(std::string,
            parse::ParsedResult, parse::ParsedResult);

    Result refine(Result r, std::string t) override {
        if(r.size() < 2) return r;

        parse::ParsedResult prev, curr, tmp;
        Result merged_result;

        for(unsigned i=1; i<r.size(); i++) {
            curr = r[i];
            prev = r[i-1];

            bool able = isAbleToMerge(t, prev, curr);
            if (!prev.end() and !curr.end() and isAbleToMerge(t, prev, curr)) {
                prev = mergeResult(t, prev, curr);
                curr = tmp;
                i += 1;
            }
            merged_result.push_back(prev);
        }
        if(!prev.isEmpty())
            merged_result.push_back(prev);  // if prev != null add to list of results

        return merged_result;
    }
};

parse::ParsedResult ENMergeDateRange::mergeResult(std::string text,
        parse::ParsedResult fromResult, parse::ParsedResult toResult) {

    if(fromResult.startDate.date() > toResult.startDate.date()) {
        parse::ParsedResult tmp = fromResult;
        toResult = fromResult;
        fromResult = tmp;
    }
    fromResult.endDate = toResult.startDate;
    fromResult.makeEndDateValid();

    size_t startIndex = std::min(fromResult.getIndex(), toResult.getIndex());
    size_t endIndex   = std::max(fromResult.getIndex() + fromResult.textLength(),
                                   toResult.getIndex() + toResult.textLength());

    fromResult.setIndex(static_cast<int>(startIndex));
    fromResult.setText(text.substr(startIndex, endIndex - startIndex + 1));

    for(auto& mod: utils::ModList) {
        if(toResult.getTag(mod))
            fromResult.setTag(mod);
    }
    fromResult.setTag(utils::ENMergeDateRangeRefiner);

    return fromResult;
}

bool ENMergeDateRange::isAbleToMerge(std::string text,
                                     parse::ParsedResult r1, parse::ParsedResult r2) {

    unsigned begin = r1.getIndex() + r1.textLength();
    unsigned end   = r2.getIndex();
    std::string textBetween = text.substr(begin, end - begin);

    return regex_search(textBetween, to);
}
