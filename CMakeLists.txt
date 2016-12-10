cmake_minimum_required (VERSION 2.6)

project (cpplog)

option(CPPLOG_BUILD_TEST ON)
set(CPPLOG_NAMESPACE "log" CACHE STRING "Namespace for the cpplog library" )
set(CPPLOG_LOG_INTERVAL "100" CACHE STRING "The log interval (in ms)" )
option(CPPLOG_NO_THREADS "Disable dedicated logging-thread, log is written in callind thread" OFF)


#Include the header and source files
file( GLOB HDRS "include/*.h" )
file( GLOB SRCS "src/*.cpp" )
include_directories ("include/")

add_definitions(-DCPPLOG_NAMESPACE=${CPPLOG_NAMESPACE})
add_definitions(-DCPPLOG_LOG_INTERVAL=${CPPLOG_LOG_INTERVAL})
if(CPPLOG_NO_THREADS)
	add_definitions(-DCPPLOG_NO_THREADS=1)
endif()

if(WIN32)
	set(CMAKE_CXX_FLAGS "/W3 /D_UNICODE /DUNICODE")
	add_library(cpplog STATIC ${SRCS})
else()
	SET(CMAKE_CXX_FLAGS "-std=c++0x -O3 -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -pthread -fPIC -Wpedantic")
	add_library(cpplog SHARED ${SRCS})
endif()


# Test-program
if(CPPLOG_BUILD_TEST)
	add_executable(cpplog-test "main.cpp")
	target_link_libraries(cpplog-test cpplog)
	set_target_properties(cpplog-test PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")
endif()