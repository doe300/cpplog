/*
 * Author: doe300
 *
 * See the file "LICENSE" for the full license governing this code.
 */
#ifndef CPPLOG_LOG_H
#define CPPLOG_LOG_H

#include <functional>
#include <memory>
#include <ostream>
#include <string>

namespace cpplog
{
    /*!
     * The log-level
     */
    enum class Level : unsigned char
    {
        DEBUG = 'D',
        INFO = 'I',
        // something is not as expected
        WARNING = 'W',
        // program-error
        ERROR = 'E',
        // critical software-error
        SEVERE = 'S'
    };

    std::wostream& log(Level level);
    std::wostream& endl(std::wostream& stream);

    inline std::wostream& debug()
    {
        return cpplog::log(Level::DEBUG);
    }

    inline std::wostream& info()
    {
        return cpplog::log(Level::INFO);
    }

    inline std::wostream& warn()
    {
        return cpplog::log(Level::WARNING);
    }

    inline std::wostream& error()
    {
        return cpplog::log(Level::ERROR);
    }

    inline std::wostream& severe()
    {
        return cpplog::log(Level::SEVERE);
    }

    template <typename Func>
    using CallableWithLogStream = std::is_convertible<Func, std::function<void(std::wostream&)>>;

    /*!
     * Returns whether a log statement with the given level would be logged for the current configuration.
     */
    bool willBeLogged(Level level);

    /*!
     * Wrapper around a logging statement to only execute it if the level of logging
     * will be written to the output.
     * This can be used to skip complex logging statements when not required.
     */
    template <typename Func>
    typename std::enable_if<CallableWithLogStream<Func>::value>::type logLazy(Level level, Func&& statement)
    {
        if(willBeLogged(level))
        {
            statement(log(level));
        }
    }

    /*!
     * Wrapper around several logging statements to only execute them if the level of
     * logging will be written to the output.
     * This can be used to skip complex logging statements when not required.
     */
    template <typename Func>
    typename std::enable_if<!CallableWithLogStream<Func>::value>::type logLazy(Level level, Func&& statement)
    {
        if(willBeLogged(level))
        {
            statement();
        }
    }

    // Forward-declaration for the logger-instance
    class Logger;
    /*!
     * This is the default Logger-instance being used to write the logs.
     *
     * Use this variable to set a custom default Logger-instance.
     * To disable logging (or stopping running logging), set this DEFAULT_LOGGER to nullptr.
     *
     * NOTE: Changing the DEFAULT_LOGGER instance while messages are being logged might lead to a race condition and is
     * therefore discouraged!
     */
    extern std::unique_ptr<Logger> DEFAULT_LOGGER;

    /*!
     * Set the per-thread Logger-instance being used to write the logs.
     *
     * If not explicitly set, this will default to the DEFAULT_LOGGER Logger instance for every new thread.
     * The per-thread Logger-instance can be reset to nullptr to fall back to the DEFAULT_LOGGER again.
     *
     * Since this reference is only used for the current active thread, it is safe to replace the per-thread
     * Logger-instance at any point in the program execution (heeding the below notes).
     *
     * NOTE: This function does not take ownership of the logger instance, so the caller has to guarantee the
     * Logger-instance to outlive the logging calls in this thread (or at least until the per-thread Logger-instance is
     * overwritten).
     *
     * NOTE: Since the Logger for multiple threads can be set to the same instance, the Logger implementation may still
     * need to synchronize the output.
     *
     */
    void setThreadLogger(Logger* logger);
} // namespace cpplog

/*!
 * Convenience-wrapper to allow writing std::string into std::wostream
 */
std::wostream& operator<<(std::wostream& stream, const std::string& string);

/*!
 * Convenience macro for lazy logging.
 * Within content, the logging-stream 'log' is available.
 *
 * Example usage:
 * CPPLOG_LAZY(Level::DEBUG, log << "Hello World! << endl);
 */
#define CPPLOG_LAZY(level, content) cpplog::logLazy(level, [&](std::wostream& log) { content; })

/*!
 * Convenience macro for lazy logging.
 *
 * Example usage:
 * CPPLOG_LAZY_BLOCK(Level::DEBUG, debug() << "Hello World! << endl; debug() << "Second statement!" << endl);
 */
#define CPPLOG_LAZY_BLOCK(level, content) cpplog::logLazy(level, [&]() { content; })

#ifdef CPPLOG_NAMESPACE
namespace CPPLOG_NAMESPACE
{
    using namespace cpplog;
} // namespace CPPLOG_NAMESPACE
#endif

#endif /* CPPLOG_LOG_H */
