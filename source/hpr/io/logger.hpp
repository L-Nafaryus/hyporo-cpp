#pragma once

#include "../containers/array.hpp"
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

namespace hpr
{


namespace logging
{
    enum Severity
    {
        Emergency,
        Alert,
        Critical,
        Error,
        Warning,
        Notice,
        Info,
        Debug
    };

    class Sink
    {
        //friend class Logger;

    protected:

        Severity p_severity;
        std::string_view p_message;

    public:

        Sink() :
            p_severity {Emergency},
            p_message {}
        {}

        Sink(Severity severity) :
            p_severity {severity},
            p_message {}
        {}

        void addMessage(const std::string_view& message)
        {
            p_message = message;
        }

        virtual
        void flush() = 0;


        virtual
        ~Sink() = default;

    };

    class StandardOutput : public Sink
    {

    public:

        StandardOutput() :
            Sink()
        {}

        explicit
        StandardOutput(Severity severity) :
            Sink(severity)
        {}

        void flush() override
        {
            if (p_severity < Error)
                std::cerr << p_message << "\n";
            if (p_severity < Debug)
                std::cout << p_message << "\n";
            std::cout.flush();
        }


        ~StandardOutput() override = default;

    };

    enum class LoggerState
    {
        Endline,
        Flush,
        Exception,
        Exit
    };

    class Logger
    {
        static Logger g_instance;
        static darray<Sink*> g_sinks;

    protected:

        Severity p_severity;
        std::ostringstream p_stream;
        int p_exitcode;
        sarray<std::string, 8> p_levelNames;
        darray<std::string> p_buffer;

    protected:

        Logger() :
            p_severity {Emergency},
            p_stream {},
            p_exitcode {-1},
            p_levelNames { "Emergency", "Alert", "Critical", "Error", "Warning", "Notice", "Info", "Debug"}
        {}

        static Logger& instance()
        {
            return g_instance;
        }

    public:

        static void destroy()
        {
            for (Sink* sink : g_sinks)
                delete sink;
            g_sinks.clear();
        }

        static darray<Sink*>& sinks()
        {
            return g_sinks;
        }

        static void addSink(Sink* sink)
        {
            if (sink != nullptr)
                g_sinks.push(sink);
        }

        static Severity severity()
        {
            return g_instance.p_severity;
        }

        static void severity(Severity severity)
        {
            g_instance.p_severity = severity;
        }

        // begin functions
        friend
        std::ostringstream&& log(Severity severity);



        // end functions
        friend
        LoggerState endl();

        friend
        LoggerState flush();

        friend
        LoggerState exception();

        friend
        LoggerState exit(int code);

        //
        friend
        std::ostream& operator<<(std::ostream& stream, const LoggerState& state);
    };

    //
    Logger Logger::g_instance;
    darray<Sink*> Logger::g_sinks;

    //
    std::ostringstream&& log(Severity severity)
    {
        Logger& instance = Logger::instance();
        instance.p_severity = severity;
        //std::ostringstream oss;
        //return instance.p_stream;
        return std::move(std::ostringstream());// oss;//std::forward(oss);
    }

    std::ostringstream&& error()
    {
        return log(Error);
    }

    //
    LoggerState endl()
    {
        return LoggerState::Endline;
    }

    LoggerState flush()
    {
        return LoggerState::Flush;
    }

    LoggerState exception()
    {
        return LoggerState::Exception;
    }

    LoggerState exit(int code)
    {
        Logger& instance = Logger::instance();
        instance.p_exitcode = code;
        return LoggerState::Exit;
    }


    std::ostream& operator<<(std::ostream& stream, const LoggerState& state)
    {
        stream << std::endl;
        std::ostringstream oss;
        oss << stream.rdbuf();
        std::string test = oss.str();
        std::cout << test << std::endl;
        //static std::mutex mtx;
        //std::lock_guard<std::mutex> lock(mtx);
        Logger& instance = Logger::instance();

        if (state >= LoggerState::Endline)
        {
            stream << "\n";
        }

        //instance.p_stream << stream.rdbuf();


        if (state >= LoggerState::Flush)
        {
            // default sink
            if (Logger::sinks().is_empty())
            {
                std::unique_ptr<Sink> sink = std::make_unique<StandardOutput>(Logger::severity());
                sink->addMessage(oss.str());
                sink->flush();
                //delete sink;
            }

            // global sinks
            for (Sink* sink : Logger::sinks())
            {
                sink->addMessage(oss.str());
                sink->flush();
            }
        }

        if (state == LoggerState::Exception)
        {
            throw std::runtime_error(oss.str());
        }

        if (state == LoggerState::Exit)
        {
            std::exit(instance.p_exitcode);
        }

        //instance.p_stream.flush();
        return stream;
    }
}
}
