#pragma once
#include "InsightException.hpp"

class NoData : public InsightException
{
public:
    NoData()
        : InsightException("No data found.") {}
};