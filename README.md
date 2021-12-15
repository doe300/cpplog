# CppLog
A very light-weight thread-safe extensible logging framework for C++.

[![Build Status](https://github.com/doe300/cpplog/actions/workflows/build.yml/badge.svg)](https://github.com/doe300/cpplog/actions/workflows/build.yml)
[![GitHub license](https://img.shields.io/github/license/doe300/cpplog.svg)](https://github.com/doe300/cpplog/blob/master/LICENSE)
[![Release](https://img.shields.io/github/tag/doe300/cpplog.svg)](https://github.com/doe300/cpplog/releases/latest)

This framework makes data-races in log-outputs a thing of the past.

## Framework
CppLog is a very small library consisting of only two public header files:

- log.h: This header defines the logging-methods, include it in any source-file you want to log something in
- logger.h: This header contains the Logger-classes. You only need to include this file if you want to customize the logging output (style, destination, ...)

To support multi-threaded applications, any thread writing logs has its own instance of an output-stream where it assembles the logging output in.
With the call of the *endl* function, the message is written to the output in a thread-safe fashion.

To support parallel logging into different outputs, the per-thread Logger-instance can be set at any point without affecting any other thread.

## Usage

    //Top of the file:
    #include "log.h"
    
    //...
    
    //Where you want to log:
    log::debug() << "This is a debug message" << log::endl;
    log::info() << "This is purely informational" << log::endl;
    log::warn() << "Supports full C++ stream syntax, hence we can log " << 12 << " as well as " << 42.42 << log::endl;
    log::error() << "Something bad happened here!" << log::endl;
    log::severe() << "Something really catastrophic happened! Do somthing, NOW!!" << log::endl;

## Configuration

CppLog can optionally be configured in several ways:

- The macro **CPPLOG_NAMESPACE** determines the namespace, the logging framework is accessible in (defaults to *"log"*).
**CPPLOG_NAMESPACE** can be chosen freely, as long as it remains a valid C++11 namespace name.
- Setting the global variable **DEFAULT_LOGGER** in the framework's namespace (by default **log::DEFAULT_LOGGER**) determines the logger to be used.
Resetting **DEFAULT_LOGGER** to the *nullptr* disables logging for the remainder of the program's life.
This also shuts down the logging-thread, so no CPU time is wasted. By default, the **DEFAULT_LOGGER** is set to **ConsoleLogger** (see section Extensions).
- The function *setThreadLogger* can be used to set a custom logger for the current thread to allow for logging parallel executions to different outputs.

## Extensions
In the current version, CppLog comes with three built-in Loggers:

- ConsoleLogger: writes every log to *std::cout* and *std::cerr*, 
where any log with the levels **ERROR** and **SEVERE** are written to *std::cerr* and any other log goes to *std::cout*.
This Logger is enabled by default.
- FileLogger: Logs the output to a file, specified by a file-name given as constructor-argument.
- ColoredLogger: Writes any log to *std::cout* but uses console colors to highlight more critical messages.

To write your own Logger, simply follow this easy steps:

1. Write a Logger-class publicly extending **Logger** (in the header-file **logger.h**) and overwrite at least **logMessage**.
2. Use the global variable **DEFAULT_LOGGER** to set the logger to an instance of your custom class
3. Done!