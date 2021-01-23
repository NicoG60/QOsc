cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(QOsc VERSION 1.2)
set(QOSC_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

#Configure the project
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()


# Check if Qosc is being used directly or via add_subdirectory, but allow overriding
if(NOT DEFINED QOSC_MASTER_PROJECT)
    if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(QOSC_MASTER_PROJECT ON)
    else()
        set(QOSC_MASTER_PROJECT OFF)
    endif()
endif()

option(QOSC_INSTALL     "Generate install targets" ${NUMPYCPP_MASTER_PROJECT})
option(QOSC_BUILD_TESTS "Build the unit tests"  OFF)

if(${QOSC_BUILD_TESTS})
    enable_testing()
endif()

#install var
set(include_dest "include/QOsc-${QOsc_VERSION}")
set(lib_dest "lib/QOsc-${QOsc_VERSION}")

# add subdirs
add_subdirectory(src)
add_subdirectory(tests)

# install
if(${QOsc_INSTALL})
    install(FILES QOsc-config.cmake DESTINATION ${lib_dest})
    install(EXPORT QOsc DESTINATION ${lib_dest})
endif()