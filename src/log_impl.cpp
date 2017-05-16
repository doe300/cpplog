
#include <chrono>

/* 
 * File:   log_impl.cpp
 * Author: doe300
 * 
 * Created on September 16, 2016, 1:46 PM
 */
#include "log_impl.h"
#include "../include/logger.h"

#include <mutex>
#include <queue>
#include <utility>

using namespace CPPLOG_NAMESPACE::internal;

//Global logger
#ifndef CPPLOG_CUSTOM_LOGGER
std::unique_ptr<CPPLOG_NAMESPACE::Logger> CPPLOG_NAMESPACE::LOGGER = std::unique_ptr<CPPLOG_NAMESPACE::Logger>(new CPPLOG_NAMESPACE::ConsoleLogger());
#endif

thread_local Local CPPLOG_NAMESPACE::internal::local;

void CPPLOG_NAMESPACE::internal::appendLog(const CPPLOG_NAMESPACE::Level level, const std::wstring& local, const std::chrono::system_clock::time_point timestamp)
{
    CPPLOG_NAMESPACE::LOGGER->logMessage(level, local, timestamp);
}
