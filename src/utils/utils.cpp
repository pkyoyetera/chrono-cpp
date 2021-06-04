
#include "utils.hpp"

std::string utils::toLowerCase(std::string text) {
    std::transform(text.begin(), text.end(), text.begin(), ::tolower);
    return text;
}

std::string utils::toUpperCase(std::string text) {
    std::transform(text.begin(), text.end(), text.begin(), ::toupper);
    return text;
}

unsigned utils::argToOrdinalValue(const std::string &arg) {
    std::string value{std::regex_replace(arg, std::regex("-"), " ")};
    value = std::regex_replace(value, std::regex("^ +| +$|( ) +"), "$1");
    std::transform(value.begin(), value.end(), value.begin(), ::tolower);
    return utils::ORDINAL_WORDS[value];
}

std::string utils::keysToString(std::map<std::string, unsigned> arg) {
    std::string result;
    for(auto it=arg.begin(); it != arg.end(); ++it) {
        result += it->first + "|";
    }
    /*for(auto a : arg) {
        result + a.first + "|";
    }*/
    // if(result.end() == "|")
    result.pop_back();
    return result;
}

std::map<std::string, float> utils::extractDateTimeUnitFragments(std::string timeunitText) {
    std::map<std::string, float> fragments;// = {};
    std::string remainingText{timeunitText};
    std::smatch match;

    bool res = std::regex_search(remainingText, match, utils::PATTERN_TIME_UNIT);

    while (res) {
        fragments = utils::collectDateTimeFragment(match, fragments);
        remainingText = remainingText.substr(match[0].str().length());
        res = std::regex_search(remainingText, match, utils::PATTERN_TIME_UNIT);
    }
    return fragments;
};

std::map<std::string, float> utils::collectDateTimeFragment(std::smatch match, std::map<std::string, float>& fragments) {
    std::string _num = utils::toLowerCase(match[1].str());
    float num;

    if (utils::INTEGER_WORDS.count(_num) > 0) {
        num = utils::INTEGER_WORDS[_num];
    } else if(_num == "a" or _num == "an"){
        num = 1;
    } else if (std::regex_search(_num, std::regex("few"))) {
        num = 3;
    } else if (std::regex_search(_num, std::regex("half"))) {
        num = 0.5;
    } else {
        num = std::stoi(_num);
    }

    std::string element{match[2].str()};
    if(std::regex_search(element, std::regex("hour", std::regex::icase)))
        fragments["hour"] = num;
    else if(std::regex_search(element, std::regex("min", std::regex::icase)))
        fragments["minute"] = num;
    else if(std::regex_search(element, std::regex("sec", std::regex::icase)))
        fragments["second"] = num;
    else if(std::regex_search(element, std::regex("week", std::regex::icase)))
        fragments["week"] = num;
    else if(std::regex_search(element, std::regex("day", std::regex::icase)))
        fragments["day"] = num;
    else if(std::regex_search(element, std::regex("month", std::regex::icase)))
        fragments["month"] = num;
    else if(std::regex_search(element, std::regex("year", std::regex::icase)))
        fragments["year"] = num;

    return fragments;
}

