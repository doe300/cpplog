/*
 * File:   log_impl.cpp
 * Author: doe300
 *
 * Created on September 16, 2016, 1:46 PM
 */
#include "log_impl.h"
#include "../include/logger.h"

#include <chrono>
#include <queue>
#include <utility>

using namespace CPPLOG_NAMESPACE::internal;

// Global logger
#ifndef CPPLOG_CUSTOM_LOGGER
std::unique_ptr<CPPLOG_NAMESPACE::Logger> CPPLOG_NAMESPACE::DEFAULT_LOGGER =
    std::unique_ptr<CPPLOG_NAMESPACE::Logger>(new CPPLOG_NAMESPACE::ConsoleLogger());
#endif

thread_local Local CPPLOG_NAMESPACE::internal::local;

Local::Local() noexcept : level{CPPLOG_NAMESPACE::Level::DEBUG}, logger(nullptr) {}

bool Local::willBeLogged(Level messageLevel) const
{
    if(logger)
        return logger->willBeLogged(messageLevel);
    return DEFAULT_LOGGER && DEFAULT_LOGGER->willBeLogged(messageLevel);
}

void Local::writeMessage()
{
    if(logger)
        logger->logMessage(level, stream.str(), start);
    else if(DEFAULT_LOGGER)
        DEFAULT_LOGGER->logMessage(level, stream.str(), start);
}

void Local::setLogger(Logger* log)
{
    logger = log;
}
