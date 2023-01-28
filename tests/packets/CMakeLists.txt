add_executable(packets tst_packets.cpp)
target_link_libraries(packets PUBLIC QOsc Qt::Test)
add_test(NAME packets COMMAND packets)
