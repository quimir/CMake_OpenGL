# - Try to find Assimp
# Once done, this will define
#
# ASSIMP_FOUND - system has Assimp
# ASSIMP_INCLUDE_DIR - the Assimp include directories
# ASSIMP_LIBRARIES - link these to use Assimp

set(_assimp_HEADER_SEARCH_DIRS
	"/usr/include"
	"/usr/local/include"
	"${CMAKE_SOURCE_DIR}/includes"
	"C:/Program Files (x86)/glfw/include"
	"${CMAKE_BINARY_DIR}/_deps/assimp-src/include")

set(_assimp_LIB_SEARCH_DIRS
	"/usr/lib"
	"/usr/local/lib"
	"${CMAKE_SOURCE_DIR}/lib"
	"C:/Program Files (x86)/glfw/lib-msvc110"
	"${CMAKE_BINARY_DIR}/_deps/assimp-build/"
	"${CMAKE_BINARY_DIR}/lib")

set(_assimp_ENV_ROOT $ENV{ASSIMP_ROOT})
if (NOT ASSIMP_ROOT AND _assimp_ENV_ROOT)
  set(ASSIMP_ROOT ${_assimp_ENV_ROOT})
endif ()

if (ASSIMP_ROOT)
  list(INSERT _assimp_HEADER_SEARCH_DIRS 0 "${ASSIMP_ROOT}/include")
  list(INSERT _assimp_LIB_SEARCH_DIRS 0 "${ASSIMP_ROOT}/lib")
endif ()

if (MSVC)
  # Dynamic calculation tool chain identifiers, such as vc143, vc144, etc
  math(EXPR TOOLSET "(${MSVC_VERSION}/10)-50")

  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(ASSIMP_MODE_SUFFIX "mtd")
  else ()
	set(ASSIMP_MODE_SUFFIX "mt")
  endif ()

  set(ASSIMP_LIB_NAME "${CMAKE_STATIC_LIBRARY_PREFIX}assimp-vc${TOOLSET}-${ASSIMP_MODE_SUFFIX}${CMAKE_STATIC_LIBRARY_SUFFIX}")
  set(ASSIMP_DLL_NAME "${CMAKE_STATIC_LIBRARY_PREFIX}assimp-vc${TOOLSET}-${ASSIMP_MODE_SUFFIX}${CMAKE_SHARED_LIBRARY_SUFFIX}")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(ASSIMP_MODE_SUFFIX "d")
  else ()
	set(ASSIMP_MODE_SUFFIX "")
  endif ()

  if (BUILD_SHARED_LIBS)
	set(ASSIMP_LIB_NAME "${CMAKE_STATIC_LIBRARY_PREFIX}assimp${ASSIMP_MODE_SUFFIX}${CMAKE_SHARED_LIBRARY_SUFFIX}")
  else ()
	set(ASSIMP_LIB_NAME "${CMAKE_STATIC_LIBRARY_PREFIX}assimp${ASSIMP_MODE_SUFFIX}${CMAKE_SHARED_LIBRARY_SUFFIX}")
  endif ()
endif ()

message(STATUS "ASSIMP_LIB_NAME = ${ASSIMP_LIB_NAME}")

# Search for the header
find_path(ASSIMP_INCLUDE_DIR "assimp/mesh.h"
	PATHS ${_assimp_HEADER_SEARCH_DIRS})

# Search for the assimp
find_library(ASSIMP_LIBRARY NAMES ${ASSIMP_LIB_NAME}
	PATHS ${_assimp_LIB_SEARCH_DIRS})

if (ASSIMP_INCLUDE_DIR AND ASSIMP_LIBRARY)
  message(STATUS "Construct Assimp from source files.")

  message(STATUS "ASSIMP_INCLUDE_DIR = ${ASSIMP_INCLUDE_DIR}")
  message(STATUS "ASSIMP_LIBRARY = ${ASSIMP_LIBRARY}")

  if (ASSIMP_INCLUDE_DIR AND ASSIMP_LIBRARY)
	# Create an imported target for Assimp
	add_library(ASSIMP::ASSIMP UNKNOWN IMPORTED)
	set_target_properties(ASSIMP::ASSIMP PROPERTIES
		IMPORTED_LOCATION "${ASSIMP_LIBRARY}"
		INTERFACE_INCLUDE_DIRECTORIES "${ASSIMP_INCLUDE_DIR}")
	set(ASSIMP_TARGET ASSIMP::ASSIMP)
  else ()
	message(FATAL_ERROR "ASSIMP could not be found.")
  endif ()
else ()
  set(ASSIMP_ENABLE_BOOST_WORKAROUND OFF CACHE BOOL "" FORCE)
  set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
  set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
  set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
  # Fetch Assimp from network
  message(STATUS "Downloading Assimp from the network.")
  include(FetchContent)
  FetchContent_Declare(
	  assimp
	  GIT_REPOSITORY https://github.com/assimp/assimp.git
	  GIT_TAG v5.2.5
  )
  FetchContent_MakeAvailable(assimp)

  file(COPY "${assimp_BINARY_DIR}/include/assimp/"
	  DESTINATION "${assimp_SOURCE_DIR}/include/assimp"
	  FILES_MATCHING PATTERN "*")

  set(ASSIMP_TARGET assimp)
  set(ASSIMP_INCLUDE_DIR "${assimp_SOURCE_DIR}/include")
  message(STATUS "ASSIMP_INCLUDE_DIR = ${ASSIMP_INCLUDE_DIR}")
  set(ASSIMP_LIBRARY "${CMAKE_BINARY_DIR}/lib/${ASSIMP_LIB_NAME}")

  set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
endif ()