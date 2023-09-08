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


    class StdIOLogger : ILogger {
    private:
        std::ostream &&outputStream;
    
    public:

        StdIOLogger(const StdIOLogger &logger) = delete;

        StdIOLogger& operator= (const StdIOLogger &logger) = delete;

        StdIOLogger(std::ostream &outputStream);

        virtual void log(LogLevel level, const std::string &message) override;

    };

}

#endif