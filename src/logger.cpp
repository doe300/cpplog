/*
 * Author: doe300
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "logger.h"

#include <array>
#include <ctime>
#include <ios>
#include <iostream>

using namespace cpplog;

Logger::Logger(Level minimumLevel) noexcept : writeLock(), minLevel(minimumLevel) {}

bool Logger::willBeLogged(Level level) const
{
    switch(minLevel)
    {
    case Level::DEBUG:
        return true;
    case Level::INFO:
        return level != Level::DEBUG;
    case Level::WARNING:
        return level != Level::DEBUG && level != Level::INFO;
    case Level::ERROR:
        return level == Level::ERROR || level == Level::SEVERE;
    case Level::SEVERE:
        return level == Level::SEVERE;
    default:
        throw std::invalid_argument("Log level not handled!");
    }
}

std::string Logger::toString(std::chrono::system_clock::time_point timestamp) const
{
    auto time = std::chrono::system_clock::to_time_t(timestamp);
#ifdef _POSIX_C_SOURCE
    // ctime is not thread-safe so use thread-safe alternative
    std::array<char, 1024> buff = {0};
    std::string text = ctime_r(&time, buff.data());
#else
    std::string text = ctime(&time);
#endif
    // required, since ctime (asctime) append a new-line
    text.erase(text.find_last_of('\n'), 1);
    return text;
}

std::wstring Logger::toString(Level level) const
{
    switch(level)
    {
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

ConsoleLogger::ConsoleLogger(Level minimumLevel) noexcept : Logger(minimumLevel) {}

void ConsoleLogger::logMessage(Level level, const std::wstring& local, std::chrono::system_clock::time_point timestamp)
{
    if(!willBeLogged(level))
        return;
    std::lock_guard<std::mutex> guard(writeLock);
    if(level == Level::ERROR || level == Level::SEVERE)
        std::wcerr << toString(level) << " " << toString(timestamp) << ": " << local;
    else
        std::wcout << toString(level) << " " << toString(timestamp) << ": " << local;
}

FileLogger::FileLogger(const std::string& fileName, Level minimumLevel) :
    Logger(minimumLevel), fileStream(fileName, std::ios::out)
{
}

FileLogger::~FileLogger()
{
    fileStream.flush();
    fileStream.close();
}

void FileLogger::logMessage(Level level, const std::wstring& local, std::chrono::system_clock::time_point timestamp)
{
    if(!willBeLogged(level))
        return;
    std::lock_guard<std::mutex> guard(writeLock);
    fileStream << toString(level) << " " << toString(timestamp) << ": " << local;
}

StreamLogger::StreamLogger(std::wostream& out, Level minimumLevel) : Logger(minimumLevel), stream(out) {}

void StreamLogger::logMessage(Level level, const std::wstring& local, std::chrono::system_clock::time_point timestamp)
{
    if(!willBeLogged(level))
        return;
    std::lock_guard<std::mutex> guard(writeLock);
    stream << toString(level) << " " << toString(timestamp) << ": " << local;
}

ColoredLogger::ColoredLogger(std::wostream& out, Level minimumLevel) : StreamLogger(out, minimumLevel) {}

void ColoredLogger::logMessage(Level level, const std::wstring& local, std::chrono::system_clock::time_point timestamp)
{
    if(!willBeLogged(level))
        return;
    std::lock_guard<std::mutex> guard(writeLock);
    if(level == Level::ERROR || level == Level::SEVERE)
        stream << "\033[31m" << toString(level) << " " << toString(timestamp) << ": " << local << "\033[39;49m";
    else if(level == Level::WARNING)
        stream << "\033[33m" << toString(level) << " " << toString(timestamp) << ": " << local << "\033[39;49m";
    else
        stream << toString(level) << " " << toString(timestamp) << ": " << local;
    stream.flush();
}
