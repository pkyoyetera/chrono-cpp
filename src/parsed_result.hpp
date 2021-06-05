#pragma once

#include "parsed_component.hpp"


namespace parse
{

class ParsedResult
{
private:
    bool __end;
protected:
    posix_time::ptime anchor;
    unsigned index;
    std::string text; // todo: add accessor for text to aid testing
    utils::Tags tags;

public:
    ParsedResult();
    ParsedResult(posix_time::ptime, long, std::string);
    ParsedResult(const ParsedResult& pr);
    ~ParsedResult();
    // ParsedResult& operator=(ParsedResult&);

    ParsedComponents startDate, endDate;    // todo: make protected & make accessors and mutators

    bool isEmpty() const;
    void setTag(utils::Modifiers);
    bool hasPossibleDates();
    std::string toDate();
    unsigned getIndex() const;
    bool getTag(utils::Modifiers);
    void setIndex(int idx);
    void setText(std::string);
    size_t textLength() const;
    bool end() const;
    void makeEndDateValid();
    parse::ParsedResult& operator=(parse::ParsedResult);
};

typedef std::vector<ParsedResult> Result;

}