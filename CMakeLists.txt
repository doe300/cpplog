cmake_minimum_required (VERSION 3.13)

# Set C++ standard to C++11 without any extensions (e.g. GNU)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project (cpplog VERSION 0.7)

option(CPPLOG_BUILD_TEST "Build the concurrency test-application" OFF)
option(ENABLE_SANITIZERS "Enable build with various sanitizers" OFF)
option(ENABLE_CLANG_TIDY "Enable static analysis with clang-tidy" OFF)

set(SRCS
	include/log.h
	include/logger.h
	src/log.cpp
	src/logger.cpp
	src/log_impl.h
	src/log_impl.cpp
)

# For in-tree build, move libraries to build
if (${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
endif()

if(ENABLE_CLANG_TIDY)
	set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*,-abseil*,-android*,-altera*,-boost*,-fuchsia*,-google*,-llvm*,-*objc*,-openmp*,-clang-analyzer-osx*,-hicpp*,-modernize*,-readability*,-cert*,-cppcoreguidelines-avoid-magic-numbers,-cppcoreguidelines-avoid-non-const-global-variables")
endif()

if(WIN32)
	add_library(cpplog STATIC ${SRCS})
	target_compile_options(cpplog PRIVATE /W3 /D_UNICODE /DUNICODE)
else()
	add_library(cpplog SHARED ${SRCS})
	add_library(cpplog-static STATIC ${SRCS})
	target_compile_options(cpplog PRIVATE -O3 -Wall -Wextra -Wold-style-cast -Wpedantic -pthread -fPIC)
	target_compile_options(cpplog-static PRIVATE -O3 -Wall -Wextra -Wold-style-cast -Wpedantic -pthread -fPIC)
endif()

# If CPPLOG_CUSTOM_LOGGER is set via command-line, the global cpplog::LOGGER variable needs to be given by the application including this library
if(CPPLOG_CUSTOM_LOGGER)
	target_compile_definitions(cpplog PUBLIC -DCPPLOG_CUSTOM_LOGGER)
endif()

if(TARGET cpplog-static)
	target_include_directories(cpplog-static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

	if(CPPLOG_CUSTOM_LOGGER)
		target_compile_definitions(cpplog-static PUBLIC -DCPPLOG_CUSTOM_LOGGER)
	endif()
endif()

# Enable sanitizers
if(ENABLE_SANITIZERS)
	target_compile_options(cpplog PRIVATE -fsanitize=address -fsanitize=leak -fsanitize=undefined)
	target_compile_options(cpplog PRIVATE -fdelete-null-pointer-checks -Wnull-dereference -Wuninitialized -Wsuggest-attribute=pure -Wsuggest-attribute=const -Wsuggest-attribute=noreturn -Wsuggest-attribute=format -Wsuggest-override -Wconversion -Wzero-as-null-pointer-constant)
	target_link_options(cpplog PUBLIC -fsanitize=address -fsanitize=leak -fsanitize=undefined)

	if(TARGET cpplog-static)
		target_compile_options(cpplog-static PRIVATE -fsanitize=address -fsanitize=leak -fsanitize=undefined)
		target_compile_options(cpplog-static PRIVATE -fdelete-null-pointer-checks -Wnull-dereference -Wuninitialized -Wsuggest-attribute=pure -Wsuggest-attribute=const -Wsuggest-attribute=noreturn -Wsuggest-attribute=format -Wsuggest-override -Wconversion -Wzero-as-null-pointer-constant)
		target_link_options(cpplog-static PUBLIC -fsanitize=address -fsanitize=leak -fsanitize=undefined)
	endif()
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

	find_package(Threads)
	target_link_libraries(cpplog-test ${CMAKE_THREAD_LIBS_INIT})
endif()


##
# Installation targets
##
# Adds the public headers to the target, so they are exported
target_include_directories(cpplog PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include/cpplog>)
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
