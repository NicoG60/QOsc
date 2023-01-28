add_executable(types tst_types.cpp)
target_link_libraries(types PUBLIC QOsc Qt::Test)
add_test(NAME types COMMAND types)
