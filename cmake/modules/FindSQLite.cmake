# - Try to find SQLite
# Once done, this will define
#
# Locate the MODEL library
# This will define the following variables:
#   SQLite_FOUND - True if the library was found
#   SQLite_INCLUDE_DIRS - where to find the headers
#   SQLite_LIBRARIES - the libraries to link against

# Find include directory
find_path(SQLite_INCLUDE_DIRS "sqlite3.h" "sqlite3ext.h"
	HINTS /usr/include
	/usr/local/include
	/opt/local/include
	"${CMAKE_CURRENT_SOURCE_DIR}/includes/sqlite"
)

# Find library
find_library(SQLite_LIBRARIES NAMES sqlite3
	HINTS "${CMAKE_CURRENT_BINARY_DIR}/includes/sqlite"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib"
)

# If the library was not found, manually add it
if(NOT SQLite_LIBRARIES)
  file(GLOB SQLite_SOURCE_FILES
	  "${CMAKE_CURRENT_SOURCE_DIR}/includes/sqlite/*.c")
  add_library(SQLite ${SQLite_SOURCE_FILES})
  set(SQLite_LIBRARIES SQLite)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SQLite DEFAULT_MSG
	SQLite_INCLUDE_DIRS SQLite_LIBRARIES)

mark_as_advanced(SQLite_INCLUDE_DIRS SQLite_LIBRARIES)
