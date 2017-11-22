cmake_minimum_required (VERSION 3.1)

project (cpplog VERSION 0.4)

option(CPPLOG_BUILD_TEST "Build the concurrency test-application" OFF)
option(ENABLE_SANITIZERS "Enable build with various sanitizers" OFF)
set(CPPLOG_NAMESPACE "log" CACHE STRING "Namespace for the cpplog library" )

#Include the header and source files
file( GLOB HDRS "include/*.h" )
file( GLOB SRCS "src/*.cpp" )
include_directories ("include/")

add_definitions(-DCPPLOG_NAMESPACE=${CPPLOG_NAMESPACE})

# For in-tree build, move libraries to build
if (${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
endif()

# If CPPLOG_CUSTOM_LOGGER is set via command-line, the global CPPLOG_NAMESPACE::LOGGER variable needs to be given by the application including this library
if(CPPLOG_CUSTOM_LOGGER)
	add_definitions(-DCPPLOG_CUSTOM_LOGGER)
endif()

if(WIN32)
	set(CMAKE_CXX_FLAGS "/W3 /D_UNICODE /DUNICODE")
	add_library(cpplog STATIC ${SRCS})
else()
	SET(CMAKE_CXX_FLAGS "-std=c++0x -O3 -Wall -Wextra -Wold-style-cast -Wno-unused-parameter -Wno-missing-field-initializers -Wpedantic -pthread -fPIC")
	add_library(cpplog SHARED ${SRCS})
	add_library(cpplog-static STATIC ${SRCS})
endif()

# Enable sanitizers
if(ENABLE_SANITIZERS)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=leak -fsanitize=undefined ")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdelete-null-pointer-checks -Wnull-dereference -Wuninitialized -Wsuggest-attribute=pure -Wsuggest-attribute=const -Wsuggest-attribute=noreturn -Wsuggest-attribute=format -Wsuggest-override -Wconversion -Wzero-as-null-pointer-constant")
endif()

# "For shared libraries VERSION and SOVERSION can be used to specify the build version and API version respectively."
set_target_properties(
	cpplog PROPERTIES
	# This corresponds to the project/library-version
	VERSION "${PROJECT_VERSION}"
	# This corresponds to the API-version
	SOVERSION "0"
)


# Test-program
if(CPPLOG_BUILD_TEST)
	add_executable(cpplog-test "main.cpp")
	target_link_libraries(cpplog-test cpplog)
	set_target_properties(cpplog-test PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")
endif()


##
# Installation targets
##
# Adds the public headers to the target, so they are exported
target_include_directories(cpplog PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  $<INSTALL_INTERFACE:include/cpplog>)
# Creates the install target for the library
install(TARGETS cpplog EXPORT cpplog-targets LIBRARY DESTINATION lib)
# Creates the export target (to be used by CMake to find the INSTALLED library)
install(EXPORT cpplog-targets DESTINATION share/cpplog)
# Creates the install target for the headers
install(DIRECTORY include/ DESTINATION include/cpplog FILES_MATCHING PATTERN "*.h")
# Exports the target (to be used by CMake to find the SOURCE library)
export(TARGETS cpplog FILE cpplog-exports.cmake)
# Adds custom uninstall command
add_custom_target(uninstall "${CMAKE_COMMAND}" -P "cmake_uninstall.cmake") 