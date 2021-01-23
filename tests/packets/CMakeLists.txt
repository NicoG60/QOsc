add_executable(packets tst_packets.cpp)
target_link_libraries(packets QOsc Qt5::Test)
add_test(NAME packets COMMAND packets)
