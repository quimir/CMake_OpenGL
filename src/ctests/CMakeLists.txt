enable_testing()

if (EXISTS "${CMAKE_SOURCE_DIR}/src/googletest-1.14.0")
  add_subdirectory(${CMAKE_SOURCE_DIR}/src/googletest-1.14.0 ${CMAKE_BINARY_DIR}/googletest)
else ()
  include(FetchContent)
  FetchContent_Declare(
	  googletest
	  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  )
  FetchContent_MakeAvailable(googletest)
  # Ensure googletest is correctly set up
  FetchContent_GetProperties(googletest)
  if(NOT googletest_POPULATED)
	FetchContent_Populate(googletest)
	add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
  endif()
endif ()

# Get all .cpp files in the current directory
file(GLOB TEST_SOURCES "*.cpp")

foreach(TEST_SOURCE ${TEST_SOURCES})
  # Get the name of the test without the directory and extension
  get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)

  # Add the test executable
  add_executable(${TEST_NAME} ${TEST_SOURCE})
  target_link_libraries(${TEST_NAME} gtest gtest_main)

  # Add the test
  add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

  # Set the working directory for the test
  set_tests_properties(${TEST_NAME} PROPERTIES WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test_data")
endforeach()
