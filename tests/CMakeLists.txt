if(${QOSC_BUILD_TESTS})
    find_package(Qt5 COMPONENTS Test REQUIRED)
    add_subdirectory(interface)
    add_subdirectory(packets)
    add_subdirectory(types)
endif()


