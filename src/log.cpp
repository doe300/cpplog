/*
 * File:   log.cpp
 * Author: doe300
 *
 * Created on September 16, 2016, 1:33 PM
 */
#include "log.h"

#include "log_impl.h"

#include <chrono>
#include <sstream>
#include <vector>

using namespace CPPLOG_NAMESPACE;

std::wostream& CPPLOG_NAMESPACE::log(Level level)
{
    auto& local = CPPLOG_NAMESPACE::internal::local;
    if(!(local.willBeLogged(level)))
    {
        // apparently setting the bad-bit will prevent the << operators from running conversion, which saves some
        // processing power
        // https://stackoverflow.com/questions/8243743/is-there-a-null-stdostream-implementation-in-c-or-libraries
        local.stream.setstate(std::ios::badbit);
    }
    else
    {
        local.start = std::chrono::system_clock::now();
        local.level = level;
    }
    return local.stream;
}

std::wostream& CPPLOG_NAMESPACE::endl(std::wostream& stream)
{
    stream << std::endl;
    auto& local = CPPLOG_NAMESPACE::internal::local;
    if(!local.stream.bad())
    {
        // only write to underyling logger, if we didn't set the bad-bit
        local.writeMessage();
    }

    // reset stream-data (and state)
    local.stream.str(std::wstring());
    local.stream.clear();

    return local.stream;
}

bool CPPLOG_NAMESPACE::willBeLogged(Level level)
{
    return CPPLOG_NAMESPACE::internal::local.willBeLogged(level);
}

void CPPLOG_NAMESPACE::setThreadLogger(Logger* logger)
{
    auto& local = CPPLOG_NAMESPACE::internal::local;
    local.setLogger(logger);
}

std::wostream& operator<<(std::wostream& stream, const std::string& string)
{
    std::vector<wchar_t> result(string.size());
    size_t res = std::mbstowcs(result.data(), string.data(), string.size());
    if(res == static_cast<std::size_t>(-1))
    {
        // TODO handle error
        return stream;
    }
    return stream << std::wstring(result.data(), res);
}
