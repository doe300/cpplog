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
#include <mutex>

namespace CPPLOG_NAMESPACE
{
    class Logger
    {
    public:
        virtual ~Logger();
        
        /*
         * Writes the log to the output beneath
         *
         * NOTE: Implementations of this method need to be thread-safe, e.g. this method can be written to from multiple threads concurrently
         */
        virtual void logMessage(const Level level, const std::wstring& local, const std::chrono::system_clock::time_point timestamp) = 0;
    protected:
        virtual const std::string getCurrentTime();
        
        virtual const std::wstring toString(Level level);

        bool logLevel(const Level level);

        Logger(const Level minLevel = Level::INFO);

        Level minLevel;
        std::mutex writeLock;
    };
    
    class ConsoleLogger : public Logger
    {
    public:
    	ConsoleLogger(const Level minLevel = Level::INFO);

        virtual ~ConsoleLogger();
        
        virtual void logMessage(const Level level, const std::wstring& local, const std::chrono::system_clock::time_point timestamp) override;
    };
    
    class FileLogger : public Logger
    {
    public:
    	FileLogger(const std::string fileName, const Level minLevel = Level::INFO);

        virtual ~FileLogger();
        
        virtual void logMessage(const Level level, const std::wstring& local, const std::chrono::system_clock::time_point timestamp) override;
        
    private:
        std::wofstream fileStream;
    };
    
    class StreamLogger : public Logger
    {
    public:
    	StreamLogger(std::wostream& stream, const Level minLevel = Level::INFO);
    	virtual ~StreamLogger();

    	virtual void logMessage(const Level level, const std::wstring& local, const std::chrono::system_clock::time_point timestamp) override;
    protected:
    	std::wostream& stream;
    };

    class ColoredLogger : public StreamLogger
	{
	public:
		ColoredLogger(std::wostream& stream, const Level minLevel = Level::INFO);
		virtual ~ColoredLogger();

		virtual void logMessage(const Level level, const std::wstring& local, const std::chrono::system_clock::time_point timestamp) override;
	};
}

#endif /* LOGGER_H */

