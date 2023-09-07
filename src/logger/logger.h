#ifndef LOGGER_GUARD
#define LOGGER_GUARD

#include <string>
#include <fstream>

namespace Logger {

    enum LogLevel {
        INFO_LEVEL, ERROR_LEVEL
    };

    std::string levelToStr(LogLevel level);

    class ILogger {

    public:

        virtual void log(LogLevel level, const std::string &message) = 0;

    };


    class FileLogger : public ILogger {

        std::ofstream fileStream;

    public:

        FileLogger(const std::string &filePath);

        virtual void log(LogLevel level, const std::string &message) override;

    };

}

#endif