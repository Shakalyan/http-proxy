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

        virtual void log(LogLevel level, std::string const& message) = 0;

    };


    class StdIOLogger : public ILogger {
    private:
        std::ostream& outputStream;
    
    public:

        StdIOLogger(StdIOLogger const& logger) = delete;

        StdIOLogger& operator= (StdIOLogger const& logger) = delete;

        explicit StdIOLogger(std::ostream& outputStream);

        virtual ~StdIOLogger();

        virtual void log(LogLevel level, std::string const& message) override;

    };

}

#endif