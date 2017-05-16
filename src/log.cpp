
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
#include <stdarg.h>

using namespace CPPLOG_NAMESPACE;

std::wostream& CPPLOG_NAMESPACE::log(const Level level)
{
	//TODO if level under log-level, write to null-stream? Performance?
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

void CPPLOG_NAMESPACE::logf(const Level level, const wchar_t* format, ...)
{
    static const std::size_t bufferSize = 4096;
    std::vector<wchar_t> buffer(bufferSize);
    va_list args;
    va_start(args, format);
    int num = vswprintf(buffer.data(), buffer.size(), format, args);
    va_end(args);
    log(level) << std::wstring(buffer.data(), num) << endl;
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
