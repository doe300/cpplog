/*
 * File:   log_impl.h
 * Author: doe300
 *
 * Created on September 16, 2016, 1:46 PM
 */

#ifndef LOG_IMPL_H
#define LOG_IMPL_H

#include "log.h"

#include <chrono>
#include <sstream>

namespace CPPLOG_NAMESPACE
{
    namespace internal
    {
        struct Local
        {
        public:
            std::wstringstream stream;
            std::chrono::system_clock::time_point start;
            CPPLOG_NAMESPACE::Level level;

            explicit Local() noexcept;

            bool willBeLogged(Level messageLevel) const;
            void writeMessage();
            void setLogger(Logger* log);

        private:
            Logger* logger;
        };

        extern thread_local Local local;
    } // namespace internal
} // namespace CPPLOG_NAMESPACE

#endif /* LOG_IMPL_H */
