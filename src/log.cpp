
#include <iosfwd>

/* 
 * File:   log.cpp
 * Author: doe300
 * 
 * Created on September 16, 2016, 1:33 PM
 */
#include "log.h"
#include "log_impl.h"

#include <sstream>
#include <chrono>
#include <vector>

using namespace CPPLOG_NAMESPACE;

std::wostream& CPPLOG_NAMESPACE::log(const Level level)
{
    CPPLOG_NAMESPACE::internal::local.start = std::chrono::system_clock::now();
    CPPLOG_NAMESPACE::internal::local.level = level;
    return CPPLOG_NAMESPACE::internal::local.stream;
}

std::wostream& CPPLOG_NAMESPACE::endl(std::wostream& stream)
{
    stream << std::endl;
    CPPLOG_NAMESPACE::internal::appendLog(CPPLOG_NAMESPACE::internal::local.level,
                                          CPPLOG_NAMESPACE::internal::local.stream.str(),
                                          CPPLOG_NAMESPACE::internal::local.start);

    //reset stream-data
    //realStream.str(std::string());
    //realStream.clear();
    //or 
    std::wstringstream().swap(CPPLOG_NAMESPACE::internal::local.stream);

    return CPPLOG_NAMESPACE::internal::local.stream;
}

std::wostream& operator<<(std::wostream& stream, const std::string& string)
{
    std::vector<wchar_t> result(string.size());
    int res = std::mbstowcs(result.data(), string.data(), string.size());
    if(res >= 0)
    {
        return stream << std::wstring(result.data(), res);
    }
    else
    {
        //TODO error
        return stream;
    }
}