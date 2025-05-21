#include "pch.h"
#include "Error.h"

std::string format_error(const std::string&, long, const std::string&, const std::string& message) 
{
    std::ostringstream msg;
    msg << message;
    return msg.str();
}

Error::Error(const std::string& file, long line, const std::string& function, const std::string& message) 
{
    _message = MakeShared<std::string>(format_error(file, line, function, message));
}

const char* Error::what() const noexcept { return _message->c_str(); }