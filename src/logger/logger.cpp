#include "logger.h"
#include <fstream>
#include <vector>
#include <ctime>

std::string Logger::levelToStr(Logger::LogLevel level) {
    static std::vector<std::string> strReprs {"INFO", "ERROR"};
    return strReprs[level];
}

Logger::StdIOLogger::StdIOLogger(std::ostream &outputStream) : outputStream(std::move(outputStream)) {
    this->outputStream << "Started logging...\n";
}

static std::string currentLocalTime() {
    time_t now = time(0);
    tm* localTime = localtime(&now);
    char outputTime[13];
    sprintf(outputTime, "[%.4d/%.2d/%.2d]", localTime->tm_year+1900, localTime->tm_mon, localTime->tm_mday);
    return std::string(outputTime);
}

void Logger::StdIOLogger::log(Logger::LogLevel level, const std::string &message) {
    this->outputStream << currentLocalTime() << " [" << levelToStr(level) << "] " << message << std::endl;
}