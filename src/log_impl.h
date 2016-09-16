/* 
 * File:   log_impl.h
 * Author: doe300
 *
 * Created on September 16, 2016, 1:46 PM
 */

#ifndef LOG_IMPL_H
#define LOG_IMPL_H

#include "log.h"

#include <sstream>
#include <chrono>

namespace CPPLOG_NAMESPACE
{
    namespace internal
    {
        struct Local
        {
            std::stringstream stream;
            CPPLOG_NAMESPACE::Level level;
            std::chrono::system_clock::time_point start;
        };
        
        extern thread_local Local local;
        
        void appendLog(const CPPLOG_NAMESPACE::Level level, const std::string local, const std::chrono::system_clock::time_point timestamp);
    }
}

#endif /* LOG_IMPL_H */

