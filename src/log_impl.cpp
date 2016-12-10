
#include <chrono>

/* 
 * File:   log_impl.cpp
 * Author: doe300
 * 
 * Created on September 16, 2016, 1:46 PM
 */
#include "log_impl.h"
#include "../include/logger.h"

#include <thread>
#include <mutex>
#include <queue>
#include <utility>

using namespace CPPLOG_NAMESPACE::internal;

//Global logger
#ifndef CPPLOG_CUSTOM_LOGGER
std::unique_ptr<CPPLOG_NAMESPACE::Logger> CPPLOG_NAMESPACE::LOGGER = std::unique_ptr<CPPLOG_NAMESPACE::Logger>(new CPPLOG_NAMESPACE::ConsoleLogger());
#endif

thread_local Local CPPLOG_NAMESPACE::internal::local;

static std::mutex queueMutex;
static std::queue<std::tuple<CPPLOG_NAMESPACE::Level, std::wstring, std::chrono::system_clock::time_point>> logQueue;
static std::chrono::milliseconds LOG_UPDATE_INTERVAL{CPPLOG_LOG_INTERVAL};

//
// Logger thread
//
#ifndef CPPLOG_NO_THREADS
static void loggerLoop()
{
    std::chrono::system_clock::time_point start;
    while(CPPLOG_NAMESPACE::LOGGER)
    {
        start = std::chrono::system_clock::now();
        while(!logQueue.empty())
        {
            std::lock_guard<std::mutex> guard(queueMutex);
            const auto& front = logQueue.front();
            CPPLOG_NAMESPACE::LOGGER->logMessage(std::get<0>(front), std::get<1>(front), std::get<2>(front));
            logQueue.pop();
        }
        const auto remainder = LOG_UPDATE_INTERVAL - std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);
        if(remainder.count() > 0)
        {
            std::this_thread::sleep_for(remainder);
        }
    }
}

static std::thread logThread = std::thread(&loggerLoop);

//
// Frontend threads
//

void CPPLOG_NAMESPACE::internal::appendLog(const CPPLOG_NAMESPACE::Level level, const std::wstring& local, const std::chrono::system_clock::time_point timestamp)
{
    std::lock_guard<std::mutex> guard(queueMutex);
    logQueue.emplace(level, local, timestamp);
}

#else
void CPPLOG_NAMESPACE::internal::appendLog(const CPPLOG_NAMESPACE::Level level, const std::wstring& local, const std::chrono::system_clock::time_point timestamp)
{
    CPPLOG_NAMESPACE::LOGGER->logMessage(level, local, timestamp);
}
#endif