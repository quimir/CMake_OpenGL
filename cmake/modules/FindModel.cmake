# - Try to find MODEL
# Once done, this will define
#
# Locate the MODEL library
# This will define the following variables:
#   MODEL_FOUND - True if the library was found
#   MODEL_INCLUDE_DIRS - where to find the headers
#   MODEL_LIBRARIES - the libraries to link against

# Find include directory
find_path(MODEL_INCLUDE_DIRS "model.h"
	HINTS /usr/include
	/usr/local/include
	/opt/local/include
	"${CMAKE_CURRENT_SOURCE_DIR}/includes/include/Model"
	"${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui"
)

# Find library
find_library(MODEL_LIBRARIES NAMES MODEL
	HINTS "${CMAKE_CURRENT_BINARY_DIR}/includes/include/Model"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib"
)

# If the library was not found, manually add it
if(NOT MODEL_LIBRARIES)
  file(GLOB MODEL_SOURCE_FILES
	  "${CMAKE_CURRENT_SOURCE_DIR}/src/implementation/Model/*.cc"
	  "${CMAKE_CURRENT_SOURCE_DIR}/src/implementation/Model/*.cpp")
  add_library(MODEL STATIC ${MODEL_SOURCE_FILES})
  set(MODEL_LIBRARIES MODEL)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MODEL DEFAULT_MSG
	MODEL_INCLUDE_DIRS MODEL_LIBRARIES)

mark_as_advanced(MODEL_INCLUDE_DIRS MODEL_LIBRARIES)
