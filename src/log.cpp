
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

using namespace CPPLOG_NAMESPACE;

std::ostream& CPPLOG_NAMESPACE::log(const Level level)
{
    CPPLOG_NAMESPACE::internal::local.start = std::chrono::system_clock::now();
    CPPLOG_NAMESPACE::internal::local.level = level;
    return CPPLOG_NAMESPACE::internal::local.stream;
}

std::ostream& CPPLOG_NAMESPACE::endl(std::ostream& stream)
{
    stream << std::endl;
    CPPLOG_NAMESPACE::internal::appendLog(CPPLOG_NAMESPACE::internal::local.level,
                                          CPPLOG_NAMESPACE::internal::local.stream.str(), 
                                          CPPLOG_NAMESPACE::internal::local.start);
    
    //reset stream-data
    //realStream.str(std::string());
    //realStream.clear();
    //or 
    std::stringstream().swap(CPPLOG_NAMESPACE::internal::local.stream);
    
    return CPPLOG_NAMESPACE::internal::local.stream;
}