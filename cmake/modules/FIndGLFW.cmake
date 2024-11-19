# Locate the glfw3 library
#
# This module defines the following variables:
#
# GLFW3_LIBRARY the name of the library;
# GLFW3_INCLUDE_DIR where to find glfw include files.
# GLFW3_FOUND true if both the GLFW3_LIBRARY and GLFW3_INCLUDE_DIR have been found.
#
# To help locate the library and include file, you can define a
# variable called GLFW3_ROOT which points to the root of the glfw library
# installation.
#
# default search dirs
# 
# Cmake file from: https://github.com/daw42/glslcookbook
# 
# This file has now been improved by QuiMir. Now the file can be constructed in two ways, either from locally as 
# it was originally constructed, or from the network.

set(_glfw_HEADER_SEARCH_DIRS
	"/usr/include"
	"/usr/local/include"
	"${CMAKE_SOURCE_DIR}/includes"
	"C:/Program Files (x86)/glfw/include"
	"${CMAKE_BINARY_DIR}/_deps/glfw-src/include")

set(_glfw_LIB_SEARCH_DIRS
	"/usr/lib"
	"/usr/local/lib"
	"${CMAKE_SOURCE_DIR}/lib"
	"C:/Program Files (x86)/glfw/lib-msvc110"
	"${CMAKE_BINARY_DIR}/_deps/glfw-build/src"
	"${CMAKE_BINARY_DIR}/lib")

# Check environment for root search directory
set(_glfw_ENV_ROOT $ENV{GLFW_ROOT})
if (NOT GLFW_ROOT AND _glfw_ENV_ROOT)
  set(GLFW_ROOT ${_glfw_ENV_ROOT})
endif ()

# Put user specified location at beginning of search
if (GLFW_ROOT)
  list(INSERT _glfw_HEADER_SEARCH_DIRS 0 "${GLFW_ROOT}/include")
  list(INSERT _glfw_LIB_SEARCH_DIRS 0 "${GLFW_ROOT}/lib")
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(GLFW_LIB_SUFFIX "d")
else ()
  set(GLFW_LIB_SUFFIX "")
endif ()

# Automatically select the file name of the GLFW library based on the platform
set(GLFW_LIB_NAME "${CMAKE_STATIC_LIBRARY_PREFIX}glfw3${GLFW_LIB_SUFFIX}${CMAKE_STATIC_LIBRARY_SUFFIX}")

# Search for the header
find_path(GLFW_INCLUDE_DIR "GLFW/glfw3.h"
	PATHS ${_glfw_HEADER_SEARCH_DIRS})

# Search for the library
find_library(GLFW_LIBRARY NAMES ${GLFW_LIB_NAME} "${CMAKE_STATIC_LIBRARY_PREFIX}glfw3${CMAKE_STATIC_LIBRARY_SUFFIX}"
	PATHS ${_glfw_LIB_SEARCH_DIRS})

if (GLFW_INCLUDE_DIR AND GLFW_LIBRARY)
  message(STATUS "Construct GLFW from source files.")
  
  message(STATUS "GLFW_INCLUDE_DIR = ${GLFW_INCLUDE_DIR}")
  message(STATUS "GLFW_LIBRARY = ${GLFW_LIBRARY}")

  if (GLFW_LIBRARY AND GLFW_INCLUDE_DIR)
	# Create an imported target for GLFW
	add_library(GLFW::GLFW UNKNOWN IMPORTED)
	set_target_properties(GLFW::GLFW PROPERTIES
		IMPORTED_LOCATION "${GLFW_LIBRARY}"
		INTERFACE_INCLUDE_DIRECTORIES "${GLFW_INCLUDE_DIR}")
	set(GLFW_TARGET GLFW::GLFW)
  else ()
	message(FATAL_ERROR "GLFW could not be found.")
  endif ()
else ()
  # Set GLFW build options to ensure static libraries are generated if fetched
  set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
  # Fetch GLFW from network
  message(STATUS "Downloading GLFW from the network.")
  include(FetchContent)
  FetchContent_Declare(
	  glfw
	  GIT_REPOSITORY https://github.com/glfw/glfw.git
	  GIT_TAG 3.4
  )
  FetchContent_MakeAvailable(glfw)
  
  # Set paths for the fetched GLFW
  set(GLFW_INCLUDE_DIR "${glfw_SOURCE_DIR}/include" "${glfw_BINARY_DIR}/include")
  message(STATUS "GLFW_INCLUDE_DIR = ${GLFW_INCLUDE_DIR}")
  set(GLFW_LIBRARY "${CMAKE_BINARY_DIR}/lib/${GFLFW_LIB_NAME}")
  
  set(GLFW_TARGET glfw)
endif ()