#pragma once

#include <iostream>
#include <memory>
#include <sstream>
#include <vector>
#include <array>
#include <source_location>
#include <fstream>
#include <filesystem>

namespace hpr::logging
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
        std::string p_message;

    public:

        Sink() :
                p_severity {Emergency},
                p_message {}
        {}

        Sink(Severity severity) :
                p_severity {severity},
                p_message {}
        {}

        void addMessage(const std::string& message)
        {
            p_message = message;
        }

        virtual
        void flush() = 0;
        /*{
            p_message.clear();
        }*/

        virtual
        Sink* clone() const = 0;


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
            if (p_severity <= Error)
                std::cerr << p_message;// << "\n";
            if (p_severity > Error && p_severity <= Debug)
                std::cout << p_message;// << "\n";
            std::cout.flush();
            //p_message.clear();
        }

        StandardOutput* clone() const override
        {
            return new StandardOutput(*this);
        }

        ~StandardOutput() override = default;

    };

    class FileOutput : public Sink
    {

    protected:

        std::string p_filename;
        static std::mutex mutex;
    public:

        FileOutput() = delete;

        FileOutput(Severity severity, const std::string& filename) :
                Sink {severity},
                p_filename {filename}
        {}

        void flush() override
        {
            auto path = std::filesystem::canonical(std::filesystem::path(p_filename));
            std::ofstream sf {path, std::ios::app};

            if (!sf.is_open())
                throw std::runtime_error("Failed to open file");
            else
            {
                std::lock_guard<std::mutex> lock(mutex);
                sf << p_message;
            }
        }

        FileOutput* clone() const override
        {
            return new FileOutput(*this);
        }

        ~FileOutput() override = default;

    };

    std::mutex FileOutput::mutex;

    class DumbassOutput : public Sink
    {

    public:

        DumbassOutput() :
                Sink()
        {}

        explicit
        DumbassOutput(Severity severity) :
                Sink(severity)
        {}

        void flush() override
        {
            std::cout << "BLUAARUARA";// << "\n";
            std::cout.flush();
            p_message.clear();
        }

        DumbassOutput* clone() const override
        {
            return new DumbassOutput(*this);
        }

        ~DumbassOutput() override = default;

    };


    class Logger
    {

    public:

        enum class State
        {
            Endline,
            Flush,
            Exception,
            Exit
        };

    private:

        static std::mutex mutex;

    protected:

        Severity p_severity;
        std::ostringstream p_stream;
        int p_exitcode;
        std::array<std::string, 8> p_levelNames;
        std::vector<Sink*> p_sinks;
        std::source_location p_location;
        bool p_isGlobal;
        std::string p_format;

    public:

        Logger() :
                p_severity {Emergency},
                p_stream {"", std::ios::out | std::ios::ate },
                p_exitcode {-1},
                p_levelNames { "Emergency", "Alert", "Critical", "Error", "Warning", "Notice", "Info", "Debug"},
                p_sinks {},
                p_location {},
                p_isGlobal {false},
                p_format {"{levelname}({location}): {message}"}
        {}

        explicit
        Logger(Severity severity) :
                p_severity {severity},
                p_stream {},
                p_exitcode {-1},
                p_levelNames { "Emergency", "Alert", "Critical", "Error", "Warning", "Notice", "Info", "Debug"},
                p_sinks {},
                p_location {},
                p_isGlobal {false},
                p_format {"{levelname}: {filename}:{location}: {message}"}
        {}

        Logger(const Logger& logger)
        {
            copy(*this, logger);
        }

        Logger& operator=(const Logger& logger)
        {
            copy(*this, logger);
            return *this;
        }

        virtual
        ~Logger()
        {
            for (auto& sink : p_sinks)
                delete sink;
        }

        inline
        void severity(Severity severity)
        {
            p_severity = severity;
        }

        inline
        Severity severity()
        {
            return p_severity;
        }

        inline
        std::string stream()
        {
            return p_stream.str();
        }

        inline
        void levelname(Severity severity, const std::string& name)
        {
            p_levelNames[severity] = name;
        }

        inline
        std::string levelname(Severity severity)
        {
            return p_levelNames[severity];
        }

        inline
        void location(const std::source_location& location)
        {
            p_location = location;
        }

        inline
        std::source_location location()
        {
            return p_location;
        }

        inline
        void format(const std::string& format)
        {
            p_format = format;
        }

        inline
        std::string format()
        {
            return p_format;
        }

        inline
        const std::vector<Sink*>& sinks()
        {
            return p_sinks;
        }

        inline
        void addSink(Sink* sink)
        {
            if (sink != nullptr)
            {
                p_sinks.push_back(sink->clone());
                //p_sinks.push_back(new T());
                //*p_sinks.back() = *sink;
                //p_sinks.emplace_back();
                //p_sinks.back() = reinterpret_cast<Sink *>(&sink);
            }
        }

        inline
        void removeSink(std::size_t n)
        {
            p_sinks.erase(p_sinks.begin() + n);
        }

        inline
        Sink* defaultSink()
        {
            return new StandardOutput(p_severity);
        }

        std::string formatMessage(const std::string& message)
        {
            auto replace = [](std::string& lhs, const std::string& rep, const std::string& replacement)
            {
                std::size_t index = 0;
                while ((index = lhs.find(rep, index)) != std::string::npos)
                {
                    lhs.replace(index, rep.length(), replacement);
                    index += replacement.length();
                }
                return lhs;
            };

            std::string target = p_format;
            target = replace(target, "{levelname}", p_levelNames[p_severity]);
            target = replace(target, "{location}", std::to_string(p_location.line()));
            target = replace(target, "{function}", p_location.function_name());
            target = replace(target, "{filename}", p_location.file_name());
            target = replace(target, "{message}", message);

            return target;
        }

        template <class T>
        Logger& operator<<(const T& message)
        {
            if constexpr (std::is_same_v<T, State>)
            {
                //std::lock_guard<std::mutex> lock(mutex);

                if (message >= State::Endline)
                {
                    p_stream << "\n";
                }

                if (message >= State::Flush)
                {
                    std::string formattedMessage = formatMessage(p_stream.str());

                    if (p_sinks.empty())
                    {
                        std::cout << "default sink" << std::endl;
                        Sink* sink = defaultSink();
                        sink->addMessage(formattedMessage);
                        sink->flush();
                        delete sink;
                    }
                    else
                    {
                        std::cout << "local sinks" << std::endl;
                        for (auto& sink : p_sinks)
                        {
                            if (sink != nullptr)
                            {
                                sink->addMessage(formattedMessage);
                                sink->flush();
                            }
                        }
                    }

                    p_stream.flush();
                }

                if (message == State::Exception)
                {
                    throw std::runtime_error(p_stream.str());
                }

                if (message == State::Exit)
                {
                    std::exit(p_exitcode);
                }

            }
            else
            {
                std::lock_guard<std::mutex> lock(mutex);
                p_stream << message;
            }
            return *this;
        }


        friend inline
        Logger log(Severity severity, std::source_location location);

    public:

        static void copy(Logger& lhs, const Logger& rhs)
        {
            lhs.p_severity = rhs.p_severity;

            lhs.p_stream.flush();
            lhs.p_stream << rhs.p_stream.str();

            lhs.p_exitcode = rhs.p_exitcode;
            lhs.p_levelNames = rhs.p_levelNames;
            lhs.p_isGlobal = false;
            lhs.p_location = rhs.p_location;
            lhs.p_format = rhs.p_format;

            if (!rhs.p_sinks.empty())
            {
                lhs.p_sinks.reserve(rhs.p_sinks.size());
                for (const auto& sink : rhs.p_sinks)
                {
                    lhs.p_sinks.push_back(sink->clone());
                    //*lhs.p_sinks.back() = *sink;
                    //lhs.p_sinks.push_back(std::unique_ptr<Sink>(sink));
                }
            }
        }

    private:

        static std::shared_ptr<Logger> globalLogger;

    public:

        friend inline
        void create();

        friend inline
        void destroy();

        friend inline
        std::shared_ptr<Logger>& get();
    };

    std::shared_ptr<Logger> Logger::globalLogger;
    std::mutex Logger::mutex;

    inline
    Logger::State flush()
    {
        return Logger::State::Flush;
    }

    inline
    Logger log(Severity severity, std::source_location location = std::source_location::current())
    {
        if (Logger::globalLogger != nullptr)
        {
            Logger::globalLogger->severity(severity);
            Logger logger = *Logger::globalLogger;
            logger.location(location);
            return logger;
        }
        else
        {
            Logger logger = Logger(severity);
            logger.location(location);
            return logger;
        }
    }

    inline
    void create()
    {
        Logger::globalLogger = std::make_shared<Logger>();
        Logger::globalLogger->p_isGlobal = true;
    }

    inline
    void destroy()
    {
        if (Logger::globalLogger != nullptr)
            Logger::globalLogger.reset();
    }

    inline
    std::shared_ptr<Logger>& get()
    {
        return Logger::globalLogger;
    }


    inline
    Logger emergency(std::source_location location = std::source_location::current())
    {
        return log(Emergency, location);
    }

    inline
    Logger alert(std::source_location location = std::source_location::current())
    {
        return log(Alert, location);
    }

    inline
    Logger critical(std::source_location location = std::source_location::current())
    {
        return log(Critical, location);
    }

    inline
    Logger error(std::source_location location = std::source_location::current())
    {
        return log(Error, location);
    }

    inline
    Logger warning(std::source_location location = std::source_location::current())
    {
        return log(Warning, location);
    }

    inline
    Logger notice(std::source_location location = std::source_location::current())
    {
        return log(Notice, location);
    }

    inline
    Logger info(std::source_location location = std::source_location::current())
    {
        return log(Info, location);
    }

    inline
    Logger debug(std::source_location location = std::source_location::current())
    {
        return log(Debug, location);
    }

}