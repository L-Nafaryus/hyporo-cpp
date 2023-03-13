#pragma once

#include <exception>
#include <string>
#include <source_location>
#include <sstream>
#include <utility>
#include <vector>

namespace hpr
{
    class Exception : public std::exception
    {
    protected:
        std::string p_message;
    public:
        inline explicit
        Exception(std::source_location location = std::source_location::current()) :
            p_message {}
        {
            std::stringstream _message;
            _message << "\t" << p_message
                << "\n  where:\t\t" << location.file_name() << ":" << location.line() << ":" << location.column()
                << "\n  function:\t" <<  location.function_name();
            p_message = _message.str();
        }
        inline explicit
        Exception(const std::string& message, std::source_location location = std::source_location::current()) :
                p_message {message}
        {
            std::stringstream _message;
            _message << "\t" << p_message
                     << "\n  where:\t" << location.file_name() << ":" << location.line() << ":" << location.column()
                     << "\n  function:\t" <<  location.function_name();
            p_message = _message.str();
        }
        [[nodiscard]] const char* what() const noexcept override {
            std::vector<float> vv;
            return p_message.data();
        }
    };

    struct OutOfRange : public Exception
    {
        inline explicit OutOfRange(std::source_location location = std::source_location::current()) : Exception {"Out of range", location} {}
        inline explicit OutOfRange(const std::string& message, std::source_location location = std::source_location::current()) : Exception {message, location} {}
    };

    struct LengthError : public Exception
    {
        inline explicit LengthError(std::source_location location = std::source_location::current()) : Exception {"Length error", location} {}
        inline explicit LengthError(const std::string& message, std::source_location location = std::source_location::current()) : Exception {message, location} {}
    };
}