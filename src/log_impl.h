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
			std::wstringstream stream;
			std::chrono::system_clock::time_point start;
			CPPLOG_NAMESPACE::Level level;

			explicit Local() noexcept;
		};

		extern thread_local Local local;

		void appendLog(CPPLOG_NAMESPACE::Level level, const std::wstring& local, std::chrono::system_clock::time_point timestamp);
	} // namespace internal
}

#endif /* LOG_IMPL_H */

