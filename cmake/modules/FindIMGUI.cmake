# - Try to find Imgui
# Once done, this will define
#
# Locate the IMGUI library
# This will define the following variables:
#   IMGUI_FOUND - True if the library was found
#   IMGUI_INCLUDE_DIRS - where to find the headers
#   IMGUI_LIBRARIES - the libraries to link against

# Find include directory
find_path(IMGUI_INCLUDE_DIRS "imgui.h"
	HINTS /usr/include
	/usr/local/include
	/opt/local/include
	"${CMAKE_CURRENT_SOURCE_DIR}/includes/imgui"
	"${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui"
)

# Find library
find_library(IMGUI_LIBRARIES NAMES IMGUI
	HINTS "${CMAKE_CURRENT_BINARY_DIR}/includes/imgui"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib"
)

# If the library was not found, manually add it
if(NOT IMGUI_LIBRARIES)
  file(GLOB IMGUI_SOURCE_FILES
	  "${CMAKE_CURRENT_SOURCE_DIR}/includes/imgui/*.cpp")
  add_library(IMGUI ${IMGUI_SOURCE_FILES})
  set(IMGUI_LIBRARIES IMGUI)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(IMGUI DEFAULT_MSG
	IMGUI_INCLUDE_DIRS IMGUI_LIBRARIES)

mark_as_advanced(IMGUI_INCLUDE_DIRS IMGUI_LIBRARIES)
