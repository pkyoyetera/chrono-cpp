
#include "utils.hpp"


int argToOrdinalValue(const std::string &arg) {
    std::string value{std::regex_replace(arg, std::regex("-"), " ")};
    value = std::regex_replace(value, std::regex("^ +| +$|( ) +"), "$1");
    std::transform(value.begin(), value.end(), value.begin(), ::tolower);
    return utils::ORDINAL_WORDS[value];
};
