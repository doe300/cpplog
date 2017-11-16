/* 
 * File:   log.cpp
 * Author: doe300
 * 
 * Created on September 16, 2016, 1:33 PM
 */
#include "log.h"

#include "log_impl.h"
#include "logger.h"

#include <chrono>
#include <cstdarg>
#include <iosfwd>
#include <sstream>
#include <vector>

using namespace CPPLOG_NAMESPACE;

std::wostream& CPPLOG_NAMESPACE::log(const Level level)
{
	if(!(LOGGER && LOGGER->willBeLogged(level)))
	{
		//apparently setting the bad-bit will prevent the << operators from running conversion, which saves some processing power
		//https://stackoverflow.com/questions/8243743/is-there-a-null-stdostream-implementation-in-c-or-libraries
		CPPLOG_NAMESPACE::internal::local.stream.setstate(std::ios::badbit);
	}
	else
	{
		CPPLOG_NAMESPACE::internal::local.start = std::chrono::system_clock::now();
		CPPLOG_NAMESPACE::internal::local.level = level;
	}
	return CPPLOG_NAMESPACE::internal::local.stream;
}

std::wostream& CPPLOG_NAMESPACE::endl(std::wostream& stream)
{
	stream << std::endl;
	if(!CPPLOG_NAMESPACE::internal::local.stream.bad())
	{
		//only write to underyling logger, if we didn't set the bad-bit
		CPPLOG_NAMESPACE::internal::appendLog(CPPLOG_NAMESPACE::internal::local.level,
											  CPPLOG_NAMESPACE::internal::local.stream.str(),
											  CPPLOG_NAMESPACE::internal::local.start);
	}

	//reset stream-data (and state)
	CPPLOG_NAMESPACE::internal::local.stream.str(std::wstring());
	CPPLOG_NAMESPACE::internal::local.stream.clear();

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
	log(level) << std::wstring(buffer.data(), static_cast<std::size_t>(num)) << endl;
}

std::wostream& operator<<(std::wostream& stream, const std::string& string)
{
	std::vector<wchar_t> result(string.size());
	size_t res = std::mbstowcs(result.data(), string.data(), string.size());
	if(res == static_cast<std::size_t>(-1))
	{
		//TODO handle error
		return stream;
	}
	return stream << std::wstring(result.data(), res);
}
