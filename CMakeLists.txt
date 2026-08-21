cmake_minimum_required(VERSION 3.21)

project(CyberCBA2077 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug)
endif()

option(CYBERCBA_WARNINGS_AS_ERRORS "Treat first-party compiler warnings as errors" OFF)
option(CYBERCBA_ENABLE_CLANG_TIDY "Run clang-tidy on first-party targets" OFF)
option(CYBERCBA_ENABLE_SANITIZERS "Build with ASan+UBSan" OFF)
option(CYBERCBA_BUILD_TESTS "Build cybercba_tests" ON)
option(CYBERCBA_BUILD_APP "Build cybercba_app (requires raylib)" ON)

find_package(Doxygen QUIET)
if(Doxygen_FOUND)
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating Doxygen API documentation"
        VERBATIM
    )
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(CompilerWarnings)
include(Sanitizers)
include(StaticAnalysis)

add_subdirectory(src)

if(CYBERCBA_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
