/* 
 * File:   main.cpp
 * Author: doe300
 *
 * Created on September 16, 2016, 2:36 PM
 */

#include <chrono>
#include <thread>
#include <iostream>


#include <cstdlib>
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
    }
}

/*
 * 
 */
int main(int argc, char** argv)
{
    log::LOGGER.reset(new log::ColoredLogger(std::wcout));

    log::info() << "Dummy" << log::endl;
    
    for(int i= 0; i < 10; i++)
    {
        new std::thread(&logThread);
    }
    
    while(true)
    {
        log::debug() << std::string("Test") << log::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        log::error() << "Test2" << log::endl;
    }
    
    return 0;
}

