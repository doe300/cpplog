/*
 * Author: doe300
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef LOG_IMPL_H
#define LOG_IMPL_H

#include "log.h"

#include <chrono>
#include <sstream>

namespace cpplog
{
    namespace internal
    {
        struct Local
        {
        public:
            std::wstringstream stream;
            std::chrono::system_clock::time_point start;
            cpplog::Level level;

            explicit Local() noexcept;

            bool willBeLogged(Level messageLevel) const;
            void writeMessage();
            void setLogger(Logger* log);

        private:
            Logger* logger;
        };

        extern thread_local Local local;
    } // namespace internal
} // namespace cpplog

#endif /* LOG_IMPL_H */
