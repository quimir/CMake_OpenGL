enable_testing()

# Add TimeUtils test executable
add_executable(TimeUtilsTest TimeUtilsTest.cpp)
target_link_libraries(TimeUtilsTest gtest gtest_main)
add_test(NAME TimeUtilsTest COMMAND TimeUtilsTest)

add_executable(LoggSystemTest LoggSystemTest.cpp)
target_link_libraries(LoggSystemTest gtest gtest_main)
add_test(NAME LoggSystemTest COMMAND LoggSystemTest)