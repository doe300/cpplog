/*
 * File:   logger.cpp
 * Author: doe300
 *
 * Created on September 16, 2016, 1:56 PM
 */

#include "logger.h"

#include <ctime>
#include <ios>
#include <iostream>

using namespace CPPLOG_NAMESPACE;

Logger::Logger(const Level minimumLevel) noexcept : writeLock(), minLevel(minimumLevel) {}

bool Logger::willBeLogged(const Level level) const
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

const std::string Logger::getCurrentTime()
{
    time_t now = time(nullptr);
    std::string text = ctime(&now);
    // required, since ctime (asctime) append a new-line
    text.erase(text.find_last_of('\n'), 1);
    return text;
}

const std::wstring Logger::toString(Level level)
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

ConsoleLogger::ConsoleLogger(const Level minimumLevel) noexcept : Logger(minimumLevel) {}

void ConsoleLogger::logMessage(
    const Level level, const std::wstring& local, const std::chrono::system_clock::time_point timestamp)
{
    if(!willBeLogged(level))
        return;
    std::lock_guard<std::mutex> guard(writeLock);
    if(level == Level::ERROR || level == Level::SEVERE)
        std::wcerr << toString(level) << " " << getCurrentTime() << ": " << local;
    else
        std::wcout << toString(level) << " " << getCurrentTime() << ": " << local;
}

FileLogger::FileLogger(const std::string& fileName, const Level minimumLevel) :
    Logger(minimumLevel), fileStream(fileName, std::ios::out)
{
}

FileLogger::~FileLogger()
{
    fileStream.flush();
    fileStream.close();
}

void FileLogger::logMessage(
    const Level level, const std::wstring& local, const std::chrono::system_clock::time_point timestamp)
{
    if(!willBeLogged(level))
        return;
    std::lock_guard<std::mutex> guard(writeLock);
    fileStream << toString(level) << " " << getCurrentTime() << ": " << local;
}

StreamLogger::StreamLogger(std::wostream& out, const Level minimumLevel) : Logger(minimumLevel), stream(out) {}

void StreamLogger::logMessage(
    const Level level, const std::wstring& local, const std::chrono::system_clock::time_point timestamp)
{
    if(!willBeLogged(level))
        return;
    std::lock_guard<std::mutex> guard(writeLock);
    stream << toString(level) << " " << getCurrentTime() << ": " << local;
}

ColoredLogger::ColoredLogger(std::wostream& out, const Level minimumLevel) : StreamLogger(out, minimumLevel) {}

void ColoredLogger::logMessage(
    const Level level, const std::wstring& local, const std::chrono::system_clock::time_point timestamp)
{
    if(!willBeLogged(level))
        return;
    std::lock_guard<std::mutex> guard(writeLock);
    if(level == Level::ERROR || level == Level::SEVERE)
        stream << "\033[31m" << toString(level) << " " << getCurrentTime() << ": " << local << "\033[39;49m";
    else if(level == Level::WARNING)
        stream << "\033[33m" << toString(level) << " " << getCurrentTime() << ": " << local << "\033[39;49m";
    else
        stream << toString(level) << " " << getCurrentTime() << ": " << local;
    stream.flush();
}
