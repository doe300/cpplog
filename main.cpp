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

/*
 * 
 */
int main(int argc, char** argv)
{
    log::LOGGER.reset(new log::ColoredLogger(std::cout));

    log::info() << "Dummy" << log::endl;
    
    while(true)
    {
        log::debug() << "Test"<< log::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds{10});
        log::error() << "Test2"<< log::endl;
    }
    
    return 0;
}

