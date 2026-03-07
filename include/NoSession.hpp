#pragma once
#include "InsightException.hpp"

class NoSession : public InsightException
{
public:
    NoSession()
        : InsightException("No study sessions logged.") {}
};