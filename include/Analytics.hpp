#ifndef ANALYTICS_HPP
#define ANALYTICS_HPP

#include <vector>
#include "StudySession.hpp"

class Analytics {
public:
    virtual void analyze(const std::vector<StudySession>& sessions) = 0;
    virtual ~Analytics() {}
};
#endif