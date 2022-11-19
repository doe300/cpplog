/*
 * Author: doe300
 *
 * See the file "LICENSE" for the full license governing this code.
 */
#include "log_impl.h"
#include "../include/logger.h"

#include <chrono>
#include <queue>
#include <utility>

using namespace cpplog::internal;

// Global logger
#ifndef CPPLOG_CUSTOM_LOGGER
std::unique_ptr<cpplog::Logger> cpplog::DEFAULT_LOGGER = std::unique_ptr<cpplog::Logger>(new cpplog::ConsoleLogger());
#endif

thread_local Local cpplog::internal::local;

Local::Local() noexcept : level{cpplog::Level::DEBUG}, logger(nullptr) {}

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
