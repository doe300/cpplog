/* 
 * File:   log.h
 * Author: doe300
 *
 * Created on September 16, 2016, 1:33 PM
 */
#ifndef LOG_H
#define LOG_H

#include <ostream>
#include <memory>
#include <string>

namespace CPPLOG_NAMESPACE
{

    /*!
     * The log-level
     */
    enum class Level : unsigned char
    {
        DEBUG = 'D',
        INFO = 'I',
        //something is not as expected
        WARNING = 'W',
        //program-error
        ERROR = 'E',
        //critical software--error
        SEVERE = 'S'
    };

    std::wostream& log(const Level level);
    std::wostream& endl(std::wostream& stream);

    inline std::wostream& debug()
    {
        return log(Level::DEBUG);
    }

    inline std::wostream& info()
    {
        return log(Level::INFO);
    }

    inline std::wostream& warn()
    {
        return log(Level::WARNING);
    }

    inline std::wostream& error()
    {
        return log(Level::ERROR);
    }

    inline std::wostream& severe()
    {
        return log(Level::SEVERE);
    }
    
    void logf(const Level level, const wchar_t* format, ...);

    //Forward-declaration for the logger-instance
    class Logger;
    /*!
     * This is the Logger-instance being used to write the logs.
     * 
     * Use this variable to set a custom Logger-instance.
     * To disable logging (or stopping running logging), set this LOGGER to nullptr
     */ 
    extern std::unique_ptr<Logger> LOGGER;
}

/*!
 * Convenience-wrapper to allow writing std::string into std::wostream
 */
std::wostream& operator <<(std::wostream& stream, const std::string& string);

#endif /* LOG_H */

