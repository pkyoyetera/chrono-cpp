#ifndef EXTRACTTIMEZONEABBREVIATION_HPP
#define EXTRACTTIMEZONEABBREVIATION_HPP

#include "refiner.hpp"

#define TIMEZONE_NAME_PATTERN "^\\s*\\(?([A-Z]{2,4})\\)?(?=\\W|$)"

class ExtractTimeZoneAbbreviation : public refiners::Refiner {
public:
    ExtractTimeZoneAbbreviation()  = default;
    ~ExtractTimeZoneAbbreviation() = default;

    Result refine(Result r, std::string text) override {
        // Result refinedResults;
        std::smatch match;

        for(auto& res : r) {
            if(res.getTag(utils::ENTimeExpressionParser)) {
                // match text starting at index immediately succeeding parser's matched text
                std::string tmp;
                try {
                    tmp= text.substr(res.getIndex() + res.textLength(), text.length());
                } catch (std::out_of_range& e) {
                    std::cerr << e.what() << std::endl;
                    continue;
                }
                if(std::regex_match(tmp, match, std::regex(TIMEZONE_NAME_PATTERN, std::regex::icase))) {
                    std::string tzAbbrev = utils::toUpperCase(match[1].str());

                    if(utils::DEFAULT_TIMEZONE_ABBR_MAP.find(tzAbbrev) != utils::DEFAULT_TIMEZONE_ABBR_MAP.end()) {
                        int offset = utils::DEFAULT_TIMEZONE_ABBR_MAP[tzAbbrev];

                        if(!res.startDate.isCertain("tzoffset")) {
                            res.startDate.setTimeZoneOffset(offset);
                        }

                        if(res.end() and !res.endDate.isCertain("tzoffset")) {
                            res.startDate.setTimeZoneOffset(offset);
                        }

                        res.setText(match[1].str());
                        res.setTag(utils::ExtractTimeZoneAbbreviation);
                    }
                }
            }
            else {
                continue;
            }
        }
        return r;
    }
};

#undef TIMEZONE_NAME_PATTERN

#endif 