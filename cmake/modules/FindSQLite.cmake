# - Try to find SQLite
# Once done, this will define
#
# Locate the MODEL library
# This will define the following variables:
#   SQLite_FOUND - True if the library was found
#   SQLite_INCLUDE_DIRS - where to find the headers
#   SQLite_LIBRARIES - the libraries to link against

# Find include directory
find_path(SQLite_INCLUDE_DIRS "sqlite3.h"
	HINTS /usr/include
	/usr/local/include
	/opt/local/include
	"${CMAKE_CURRENT_SOURCE_DIR}/includes/sqlite"
)

if(NOT SQLite_LIBRARIES)
  file(GLOB SQLite_SOURCE_FILES
	  "${CMAKE_CURRENT_SOURCE_DIR}/includes/sqlite/*.c")
  add_library(SQLite ${SQLite_SOURCE_FILES})
  set(SQLite_LIBRARIES SQLite)
endif()

if (MSVC)
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	find_library(SQLite_LIBRARIES NAMES SQLited SQLite
		HINTS "${CMAKE_CURRENT_SOURCE_DIR}/lib"
	)
  else()
	find_library(SQLite_LIBRARIES NAMES SQLite
		HINTS "${CMAKE_CURRENT_SOURCE_DIR}/lib"
	)
  endif()
endif ()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SQLite DEFAULT_MSG
	SQLite_INCLUDE_DIRS SQLite_LIBRARIES)

mark_as_advanced(SQLite_INCLUDE_DIRS SQLite_LIBRARIES)