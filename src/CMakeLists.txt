# set headers
set(header_path "${QOSC_SOURCE_DIR}/include")
file(GLOB headers ${header_path}/QO* ${header_path}/qo*)

# set sources
file(GLOB src *.cpp)

# define library target
add_library(QOsc ${headers} ${src})
target_link_libraries(QOsc PUBLIC Qt::Core Qt::Gui Qt::Network)
target_include_directories(
    QOsc
    PUBLIC
        $<BUILD_INTERFACE:${header_path}>
        $<INSTALL_INTERFACE:${include_dest}>

    PRIVATE
        ${${Qt}Core_PRIVATE_INCLUDE_DIRS}
)
target_compile_definitions(QOsc PRIVATE QOSC_LIBRARY)
target_compile_definitions(QOsc PRIVATE SOFT_VERSION=${PROJECT_VERSION})
target_compile_definitions(QOsc PRIVATE GIT_VERSION=${GIT_VERSION})

# install
install(TARGETS QOsc EXPORT QOsc DESTINATION ${lib_dest})
install(FILES ${headers} DESTINATION ${include_dest})
