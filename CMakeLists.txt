cmake_minimum_required(VERSION 3.24)

project(cmake_open VERSION 0.1)

set(OPENGL_TARGET_NAME cmake_open)

# Define supported build types
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build, options are: Debug Release" FORCE)
endif ()

# Set C++ configuration
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

link_directories(${CMAKE_SOURCE_DIR}/lib)

find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")
find_package(ASSIMP REQUIRED)
message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")

# Obtain the installation directory of the application
configure_file(configuration/root_directory.h.in ${CMAKE_BINARY_DIR}/configuration/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/configuration)

set(LIBS glfw3 opengl32 assimp)

# First create relevant static libraries required for other projects
add_library(STB_IMAGE "src/stb_image.cpp")
set(LIBS ${LIBS} STB_IMAGE)
add_library(GLAD "${CMAKE_SOURCE_DIR}/src/glad.c")
target_include_directories(GLAD PUBLIC "${CMAKE_SOURCE_DIR}/includes")
set(LIBS ${LIBS} GLAD)

if (MSVC)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif ()

set(HEADER_FILES_DIR "${CMAKE_SOURCE_DIR}/includes/include")
set(SOURCE_FILES_DIR "${CMAKE_SOURCE_DIR}/src/implementation")

set(CHAPTERS
	started_opengl
	model_loading
	started_opengl_texture)

function(create_project_from_sources chapter)
  set(HEADER_FILES "${HEADER_FILES_DIR}/*.h"
	  "${HEADER_FILES_DIR}/*.hpp"
	  "${HEADER_FILES_DIR}/*.inl"
	  "src/${chapter}/*.h")

  set(SOURCE_FILES "${SOURCE_FILES_DIR}/*.cc"
	  "${SOURCE_FILES_DIR}/*.cpp"
	  "src/${chapter}/*.cpp"
	  "src/${chapter}/*.cc")

  set(SHADER_FILES "src/${chapter}/*.glsl"
	  "src/${chapter}/*.vert"
	  "src/${chapter}/*.frag"
	  "src/${chapter}/*.geom"
	  "src/${chapter}/*.tesc"
	  "src/${chapter}/*.tese"
	  "src/${chapter}/*.gsh"
	  "src/${chapter}/*.fsh"
	  "src/${chapter}/*.comp"
	  "src/${chapter}/*.vsh")

  file(GLOB SOURCE
	  ${HEADER_FILES}
	  ${SOURCE_FILES}
	  ${SHADER_FILES}
  )

  set(NAME "${chapter}")

  add_executable(${NAME} ${SOURCE})
  target_link_libraries(${NAME} ${LIBS})

  if (MSVC)
	target_compile_options(${NAME} PRIVATE /std:c++17 /MP)
	target_link_options(${NAME} PUBLIC /ignore:4099)
  endif (MSVC)

  if (WIN32)
	set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${chapter}")
	set_target_properties(${NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${chapter}/Debug")
  endif (WIN32)

  # copy shader files to build directory
  file(GLOB SHADERS
	  ${SHADER_FILES})

  foreach (SHADER ${SHADERS})
	if (WIN32)
	  add_custom_command(TARGET ${NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SHADER} $<TARGET_FILE_DIR:${NAME}>)
	endif (WIN32)
  endforeach ()

  # copy dlls
  if (WIN32)
	file(GLOB DLLS "${CMAKE_SOURCE_DIR}/dlls/*.dll")
	foreach (DLL ${DLLS})
	  add_custom_command(TARGET ${NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${DLL} $<TARGET_FILE_DIR:${NAME}>)
	endforeach ()
  endif (WIN32)
endfunction()

# then create a project file per tutorial
foreach (CHAPTER ${CHAPTERS})
  create_project_from_sources(${CHAPTER})
endforeach (CHAPTER)

include_directories(${CMAKE_SOURCE_DIR}/includes)

# Add option to enable or disable tests
option(ENABLE_TESTS "Enable test" ON)

if (ENABLE_TESTS)
  add_subdirectory(${CMAKE_SOURCE_DIR}/src/googletest-1.14.0)

  add_subdirectory(src/ctests)

  aux_source_directory(/src/ctests SRC_LIST)

  # Create the test executable
  foreach (test ${SRC_LIST})
	string(REGEX REPLACE "\\.cpp$" "" test_name${test})
	add_executable(${test_name} ${test})
	target_link_libraries(${test_name} PRIVATE your_library)
	add_test(NAME ${test_name} COMMAND${test_name})
  endforeach ()

  # Set test properties such as WORKING DIRECTORY for the test
  set_tests_properties(${test_name} PROPERTIES
	  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test_data"
  )
endif ()