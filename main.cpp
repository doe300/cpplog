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
        log::debug() << "Test from: " << threadID << log::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        log::error() << "Test 2 from : " << threadID << log::endl;
        CPPLOG_LAZY(log::Level::INFO, log << "Lazy logging!" << log::endl);
        CPPLOG_LAZY_BLOCK(log::Level::INFO, log::info() << "More lazy logging" << log::endl;
                          log::debug() << "Even more lazy logging" << log::endl);
    }
}

/*
 *
 */
int main(int argc, char** argv)
{
    (void) argc;
    (void) argv;
    log::DEFAULT_LOGGER.reset(new log::ColoredLogger(std::wcout));

    log::info() << "Dummy" << log::endl;

    for(int i = 0; i < 10; i++)
    {
        new std::thread(&logThread);
    }

    unsigned counter = 0;

    log::setThreadLogger(log::DEFAULT_LOGGER.get());
    while(true)
    {
        log::debug() << std::string("Test") << log::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        log::error() << "Test2" << log::endl;
        ++counter;
        if(counter > 100)
        {
            // tests the ability to reset the logger while using it
            log::warn() << "Changing logger..." << log::endl;
            // TODO this might still randomly crash if the other threads are accessing the global logger instance while
            // it is being reset, but this behavior is discouraged anyway...
            log::DEFAULT_LOGGER.reset(new log::ColoredLogger(std::wcout));
            // test manually changing per-thread logger. This line would crash if the logger is not updated
            log::setThreadLogger(log::DEFAULT_LOGGER.get());
            counter = 0;
        }
    }

    return EXIT_SUCCESS;
}
