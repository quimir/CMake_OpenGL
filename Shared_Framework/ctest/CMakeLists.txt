enable_testing()

# Check if googletest is available
if (EXISTS "${CMAKE_BINARY_DIR}/_deps/googletest-src")
  message(STATUS "Construct googletest from source files.")
  add_subdirectory(${CMAKE_BINARY_DIR}/_deps/googletest-src ${CMAKE_BINARY_DIR}/googletest)
else ()
  message(STATUS "Download googletest from the web")
  include(FetchContent)
  FetchContent_Declare(
	  googletest
	  GIT_REPOSITORY https://github.com/google/googletest.git
	  GIT_TAG v1.14.0
  )
  FetchContent_MakeAvailable(googletest)
  # Ensure googletest is correctly set up
  FetchContent_GetProperties(googletest)
  if (NOT googletest_POPULATED)
	FetchContent_Populate(googletest)
	add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
  endif ()
endif ()

# Get all the test .cpp files
file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")

# Ensure files are found
if (NOT TEST_SOURCES)
  message(FATAL_ERROR "No test source files found!")
else()
  message(STATUS "Test sources found: ${TEST_SOURCES}")
endif()

foreach(TEST_SOURCE ${TEST_SOURCES})
  # Get the test name without path and extension
  get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)

  # Add the test executable
  add_executable(${TEST_NAME} ${TEST_SOURCE})
  target_link_libraries(${TEST_NAME} Shared_Framework gtest gtest_main)

  set_target_properties(${TEST_NAME} PROPERTIES FOLDER "Tests")

  # Add the test to CTest
  add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

  # Set the working directory for the test
  set_tests_properties(${TEST_NAME} PROPERTIES WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test_data")
endforeach()
