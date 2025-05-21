#pragma once
#include <exception>
#include <string>
#include <sstream>
#include "Types.h"

class Error : public std::exception 
{
public:
    Error(const std::string& file, long line, const std::string& functionName, const std::string& message = "");
    const char* what() const noexcept override;
private:
    SharedPtr<std::string> _message;
};
