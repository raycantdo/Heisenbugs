#pragma once
#include "InsightException.hpp"

class NoQuiz : public InsightException
{
public:
    NoQuiz()
        : InsightException("No quizzes found for this course.") {}
};