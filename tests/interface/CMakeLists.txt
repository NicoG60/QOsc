add_executable(interface tst_interface.cpp)
target_link_libraries(interface PUBLIC QOsc Qt5::Test)
add_test(NAME interface COMMAND interface)

