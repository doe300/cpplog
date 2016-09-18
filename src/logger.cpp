/* 
 * File:   logger.cpp
 * Author: doe300
 * 
 * Created on September 16, 2016, 1:56 PM
 */

#include <ios>


#include "logger.h"

#include <ctime>
#include <iostream>

using namespace CPPLOG_NAMESPACE;

Logger::Logger(const Level minLevel) : minLevel(minLevel)
{
}

Logger::~Logger()
{

}

bool Logger::logLevel(const Level level)
{
    return level >= minLevel;
}

const std::string Logger::getCurrentTime()
{
    time_t now = time(0);
    std::string text = ctime(&now);
    //required, since ctime (asctime) append a new-line
    text.erase(text.find_last_of('\n'), 1);
    return text;
}

const std::wstring Logger::toString(Level level)
{
    switch (level) {
    case Level::DEBUG:
        return L"[D]";
    case Level::INFO:
        return L"[I]";
    case Level::WARNING:
        return L"[W]";
    case Level::ERROR:
        return L"[E]";
    case Level::SEVERE:
    default:
        return L"[S]";
    }
}

ConsoleLogger::ConsoleLogger(const Level minLevel) : Logger(minLevel)
{
}

ConsoleLogger::~ConsoleLogger()
{
}

void ConsoleLogger::logMessage(const Level level,
                               const std::wstring& local,
                               const std::chrono::system_clock::time_point timestamp)
{
    if (!logLevel(level)) {
        return;
    }
    if (level == Level::ERROR || level == Level::SEVERE) {
        std::wcerr << toString(level) << " " << getCurrentTime() << ": " << local;
    }
    else {
        std::wcout << toString(level) << " " << getCurrentTime() << ": " << local;
    }
}

FileLogger::FileLogger(const std::string fileName, const Level minLevel) : Logger(minLevel), fileStream(fileName, std::ios::out)
{
}

FileLogger::~FileLogger()
{
    fileStream.flush();
    fileStream.close();
}

void FileLogger::logMessage(const Level level,
                            const std::wstring& local,
                            const std::chrono::system_clock::time_point timestamp)
{
}

ColoredLogger::ColoredLogger(std::wostream& stream, const Level minLevel) : Logger(minLevel), stream(stream)
{
}

ColoredLogger::~ColoredLogger()
{
}

void ColoredLogger::logMessage(const Level level,
                               const std::wstring& local,
                               const std::chrono::system_clock::time_point timestamp)
{
    if (!logLevel(level)) {
        return;
    }
    if (level == Level::ERROR || level == Level::SEVERE) {
        stream << "\033[31m" << toString(level) << " " << getCurrentTime() << ": " << local << "\033[39;49m";

    }
    else if (level == Level::WARNING) {
        stream << "\033[33m" << toString(level) << " " << getCurrentTime() << ": " << local << "\033[39;49m";
    }
    else {
        stream << toString(level) << " " << getCurrentTime() << ": " << local;
    }
    stream.flush();
}
