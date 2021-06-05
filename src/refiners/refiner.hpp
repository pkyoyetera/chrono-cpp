#pragma once

#include <iostream>

#include "src/parsed_result.hpp"

#include "src/utils/utils.hpp"

namespace refiners
{
class Refiner
{
protected:
    std::vector<std::string> options;
    std::shared_ptr<refiners::Refiner> _next;

public:
    Refiner() {}

    void setOpt(std::vector<std::string> opt) {
        options = opt;
    }

    std::vector<std::string> getOpt() {
        return options;
    }

    virtual parse::Result refine(parse::Result, std::string) {
        parse::Result r;
        return r;
    }


    const std::shared_ptr<Refiner> chain(const std::shared_ptr<Refiner>& next)
    {
        _next = next;
        return next;
    }

};
}

