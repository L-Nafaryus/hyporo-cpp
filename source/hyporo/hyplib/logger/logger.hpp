#pragma once

#include <ostream>


namespace hpr
{
    class Logger
    {
    public:
        enum Severity
        {
            None,
            Error,
            Warning,
            Info,
            Debug
        };
        static Severity severity;

        template <typename T>
        friend
        Logger& operator<<(Logger& l, const T& data)
        {
            if (severity == Info)
                std::cout << data << std::endl;
            else if (severity == Error)
                std::cerr << data << std::endl;

            return l;
        }
    };
    Logger::Severity Logger::severity = Logger::Warning;
    Logger logger;
}
