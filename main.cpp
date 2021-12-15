/*
 * File:   main.cpp
 * Author: doe300
 *
 * Created on September 16, 2016, 2:36 PM
 */

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

#include "log.h"
#include "logger.h"

using namespace std;

static void logThread()
{
    const int threadID = std::rand();
    while(true)
    {
        CPPLOG_NAMESPACE::debug() << "Test from: " << threadID << CPPLOG_NAMESPACE::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        CPPLOG_NAMESPACE::error() << "Test 2 from : " << threadID << CPPLOG_NAMESPACE::endl;
        CPPLOG_LAZY(CPPLOG_NAMESPACE::Level::INFO, log << "Lazy logging!" << CPPLOG_NAMESPACE::endl);
        CPPLOG_LAZY_BLOCK(
            CPPLOG_NAMESPACE::Level::INFO, CPPLOG_NAMESPACE::info() << "More lazy logging" << CPPLOG_NAMESPACE::endl;
            CPPLOG_NAMESPACE::debug() << "Even more lazy logging" << CPPLOG_NAMESPACE::endl);
    }
}

struct NonCopyMoveableLogCallback
{
    explicit NonCopyMoveableLogCallback() = default;
    NonCopyMoveableLogCallback(const NonCopyMoveableLogCallback&) = delete;
    NonCopyMoveableLogCallback(NonCopyMoveableLogCallback&&) noexcept = delete;
    ~NonCopyMoveableLogCallback() noexcept = default;

    NonCopyMoveableLogCallback& operator=(const NonCopyMoveableLogCallback&) = delete;
    NonCopyMoveableLogCallback& operator=(NonCopyMoveableLogCallback&&) noexcept = delete;

    void operator()(std::wostream& out) const
    {
        out << "SNAFU" << CPPLOG_NAMESPACE::endl;
    }

    void operator()() const
    {
        CPPLOG_NAMESPACE::warn() << "This is also fine!" << CPPLOG_NAMESPACE::endl;
    }
};

/*
 *
 */
int main(int argc, char** argv)
{
    (void) argc;
    (void) argv;
    CPPLOG_NAMESPACE::DEFAULT_LOGGER.reset(new CPPLOG_NAMESPACE::ColoredLogger(std::wcout));

    CPPLOG_NAMESPACE::info() << "Dummy" << CPPLOG_NAMESPACE::endl;

    for(int i = 0; i < 10; i++)
    {
        new std::thread(&logThread);
    }

    unsigned counter = 0;

    CPPLOG_NAMESPACE::setThreadLogger(CPPLOG_NAMESPACE::DEFAULT_LOGGER.get());
    auto start = std::chrono::steady_clock::now();
    auto end = start + std::chrono::seconds{10};
    if(argc > 1 && argv[1] == std::string{"--short-test"})
    {
        end = start + std::chrono::seconds{2};
    }
    while(std::chrono::steady_clock::now() < end)
    {
        CPPLOG_NAMESPACE::debug() << std::string("Test") << CPPLOG_NAMESPACE::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        CPPLOG_NAMESPACE::error() << "Test2" << CPPLOG_NAMESPACE::endl;
        CPPLOG_NAMESPACE::logLazy(CPPLOG_NAMESPACE::Level::INFO,
            [](std::wostream& out)
            {
                NonCopyMoveableLogCallback logCallback;
                logCallback(out);
            });
        CPPLOG_NAMESPACE::logLazy(CPPLOG_NAMESPACE::Level::WARNING,
            []()
            {
                NonCopyMoveableLogCallback logCallback;
                logCallback();
            });
        CPPLOG_LAZY(CPPLOG_NAMESPACE::Level::DEBUG, log << "FOOBAR" << CPPLOG_NAMESPACE::endl);
        CPPLOG_LAZY_BLOCK(
            CPPLOG_NAMESPACE::Level::WARNING, CPPLOG_NAMESPACE::warn() << "This is fine!" << CPPLOG_NAMESPACE::endl);
        ++counter;
        if(counter > 100)
        {
            // tests the ability to reset the logger while using it
            CPPLOG_NAMESPACE::warn() << "Changing logger..." << CPPLOG_NAMESPACE::endl;
            // TODO this might still randomly crash if the other threads are accessing the global logger instance while
            // it is being reset, but this behavior is discouraged anyway...
            CPPLOG_NAMESPACE::DEFAULT_LOGGER.reset(new CPPLOG_NAMESPACE::ColoredLogger(std::wcout));
            // test manually changing per-thread logger. This line would crash if the logger is not updated
            CPPLOG_NAMESPACE::setThreadLogger(CPPLOG_NAMESPACE::DEFAULT_LOGGER.get());
            counter = 0;
        }
    }

    return EXIT_SUCCESS;
}
