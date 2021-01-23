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

if(MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    set(BUILD_SHARED_LIBS TRUE)
endif()

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
install(FILES QOsc-config.cmake DESTINATION ${lib_dest})
install(EXPORT QOsc DESTINATION ${lib_dest})
