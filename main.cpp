/*
 * Author: doe300
 *
 * See the file "LICENSE" for the full license governing this code.
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
        cpplog::debug() << "Test from: " << threadID << cpplog::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        cpplog::error() << "Test 2 from : " << threadID << cpplog::endl;
        CPPLOG_LAZY(cpplog::Level::INFO, log << "Lazy logging!" << cpplog::endl);
        CPPLOG_LAZY_BLOCK(cpplog::Level::INFO, cpplog::info() << "More lazy logging" << cpplog::endl;
                          cpplog::debug() << "Even more lazy logging" << cpplog::endl);
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
        out << "SNAFU" << cpplog::endl;
    }

    void operator()() const
    {
        cpplog::warn() << "This is also fine!" << cpplog::endl;
    }
};

/*
 *
 */
int main(int argc, char** argv)
{
    (void) argc;
    (void) argv;
    cpplog::DEFAULT_LOGGER.reset(new cpplog::ColoredLogger(std::wcout));

    cpplog::info() << "Dummy" << cpplog::endl;

    for(int i = 0; i < 10; i++)
    {
        new std::thread(&logThread);
    }

    unsigned counter = 0;

    cpplog::setThreadLogger(cpplog::DEFAULT_LOGGER.get());
    auto start = std::chrono::steady_clock::now();
    auto end = start + std::chrono::seconds{10};
    if(argc > 1 && argv[1] == std::string{"--short-test"})
    {
        end = start + std::chrono::seconds{2};
    }
    while(std::chrono::steady_clock::now() < end)
    {
        cpplog::debug() << std::string("Test") << cpplog::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        cpplog::error() << "Test2" << cpplog::endl;
        cpplog::logLazy(cpplog::Level::INFO,
            [](std::wostream& out)
            {
                NonCopyMoveableLogCallback logCallback;
                logCallback(out);
            });
        cpplog::logLazy(cpplog::Level::WARNING,
            []()
            {
                NonCopyMoveableLogCallback logCallback;
                logCallback();
            });
        CPPLOG_LAZY(cpplog::Level::DEBUG, log << "FOOBAR" << cpplog::endl);
        CPPLOG_LAZY_BLOCK(cpplog::Level::WARNING, cpplog::warn() << "This is fine!" << cpplog::endl);
        ++counter;
        if(counter > 100)
        {
            // tests the ability to reset the logger while using it
            cpplog::warn() << "Changing logger..." << cpplog::endl;
            // TODO this might still randomly crash if the other threads are accessing the global logger instance while
            // it is being reset, but this behavior is discouraged anyway...
            cpplog::DEFAULT_LOGGER.reset(new cpplog::ColoredLogger(std::wcout));
            // test manually changing per-thread logger. This line would crash if the logger is not updated
            cpplog::setThreadLogger(cpplog::DEFAULT_LOGGER.get());
            counter = 0;
        }
    }

    return EXIT_SUCCESS;
}
