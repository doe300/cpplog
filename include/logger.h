/* 
 * File:   logger.h
 * Author: doe300
 *
 * Created on September 16, 2016, 1:56 PM
 */
#ifndef LOGGER_H
#define LOGGER_H

#include "log.h"
#include <chrono>
#include <fstream>

namespace CPPLOG_NAMESPACE
{
    class Logger
    {
    public:
        virtual ~Logger();
        
        virtual void logMessage(const Level level, const std::string local, const std::chrono::system_clock::time_point timestamp) = 0;
    protected:
        virtual std::string getCurrentTime();
        
        virtual std::string toString(Level level);

        bool logLevel(const Level level);

        Logger(const Level minLevel = Level::DEBUG);

        Level minLevel;
    };
    
    class ConsoleLogger : public Logger
    {
    public:
    	ConsoleLogger(const Level minLevel = Level::DEBUG);

        virtual ~ConsoleLogger();
        
        virtual void logMessage(const Level level, const std::string local, const std::chrono::system_clock::time_point timestamp) override;
    };
    
    class FileLogger : public Logger
    {
    public:
    	FileLogger(const std::string fileName, const Level minLevel = Level::DEBUG);

        virtual ~FileLogger();
        
        virtual void logMessage(const Level level, const std::string local, const std::chrono::system_clock::time_point timestamp) override;
        
    private:
        std::ofstream fileStream;
    };
    
    class ColoredLogger : public Logger
    {
    public:
    	ColoredLogger(std::ostream& stream, const Level minLevel = Level::DEBUG);
        virtual ~ColoredLogger();
        
        virtual void logMessage(const Level level, const std::string local, const std::chrono::system_clock::time_point timestamp) override;
        
    private:
        std::ostream& stream;
    };
}

#endif /* LOGGER_H */

