#pragma once
#include <exception>
#include <string>

class InsightException : public std::exception
{
protected:
    std::string message;

public:
    InsightException(const std::string& msg)
        : message(msg) {}

    const char* what() const noexcept override
    {
        return message.c_str();
    }
};