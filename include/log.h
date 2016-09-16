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

    std::ostream& log(const Level level);
    std::ostream& endl(std::ostream& stream);

    inline std::ostream& debug()
    {
        return log(Level::DEBUG);
    }

    inline std::ostream& info()
    {
        return log(Level::INFO);
    }

    inline std::ostream& warn()
    {
        return log(Level::WARNING);
    }

    inline std::ostream& error()
    {
        return log(Level::ERROR);
    }

    inline std::ostream& severe()
    {
        return log(Level::SEVERE);
    }

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

#endif /* LOG_H */

