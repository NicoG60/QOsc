# set headers
set(header_path "${QOSC_SOURCE_DIR}/include")
file(GLOB headers ${header_path}/QO* ${header_path}/qo*)

# set sources
file(GLOB src *.cpp)

# setup deps
find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Gui REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)

# define library target
add_library(QOsc ${headers} ${src})
target_link_libraries(QOsc PUBLIC Qt5::Core Qt5::Gui Qt5::Network)
target_include_directories(
    QOsc
    PUBLIC
        $<BUILD_INTERFACE:${header_path}>
        $<INSTALL_INTERFACE:${lib_dest}>

    PRIVATE
        ${Qt5Core_PRIVATE_INCLUDE_DIRS}
)
target_compile_definitions(QOsc PRIVATE QOSC_LIBRARY)

# install
install(TARGETS QOsc EXPORT QOsc DESTINATION ${lib_dest})
install(FILES ${headers} DESTINATION ${include_dest})
