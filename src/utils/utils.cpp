
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

std::map<utils::Units, float> utils::extractDateTimeUnitFragments(const std::string& timeunitText)
{
    std::map<Units, float> fragments;
    std::string remainingText{timeunitText};
    std::smatch match;

    bool res = std::regex_search(remainingText, match, utils::PATTERN_TIME_UNIT);

    while (res) {
        fragments = utils::collectDateTimeFragment(match, fragments);
        remainingText = remainingText.substr(match[0].str().length());
        res = std::regex_search(remainingText, match, utils::PATTERN_TIME_UNIT);
    }
    return fragments;
}

std::map<utils::Units, float> utils::collectDateTimeFragment(std::smatch match,
                                                             std::map<Units, float>& fragments)
{
    const std::string _num = utils::toLowerCase(match[1].str());
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

    const std::string_view _unit = match.str(2);
    if (boost::algorithm::contains(_unit, "hour"))
        fragments[HOUR] = num;
    else if (boost::algorithm::contains(_unit, "min"))
        fragments[MINUTE] = num;
    else if (boost::algorithm::contains(_unit, "sec"))
        fragments[SECOND] = num;
    else if (boost::algorithm::contains(_unit, "week"))
        fragments[WEEK] = num;
    else if (boost::algorithm::contains(_unit, "day"))
        fragments[DAY] = num;
    else if (boost::algorithm::contains(_unit, "month"))
        fragments[MONTH] = num;
    else if (boost::algorithm::contains(_unit, "year"))
        fragments[YEAR] = num;

    return fragments;
}

