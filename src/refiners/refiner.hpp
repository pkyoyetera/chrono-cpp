#ifndef REFINER_HPP
#define REFINER_HPP

#include <iostream>

#include "src/utils/utils.hpp"
#include "src/parsers/parsers.h"

class Refiner {
protected:
    std::vector<std::string> options;

public:
    Refiner() { }
    void setOpt(std::vector<std::string> opt) {
        options = opt;
    }
    std::vector<std::string> getOpt() {
        return options;
    }

    virtual Result refine(Result, std::string) {
        Result r;
        return r;
    }

};


#endif