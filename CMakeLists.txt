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

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/dll")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  "${CMAKE_BINARY_DIR}/lib")

set(THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/third_party")

# Set GLFW build options to ensure static libraries are generated
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

if (EXISTS "${THIRD_PARTY_DIR}/glfw-3.4")
  set(GLFW_INCLUDE_DIR "${THIRD_PARTY_DIR}/glfw-3.4/include")
else ()
  message(STATUS "Downloading GLFW from the network")
  include(FetchContent)
  FetchContent_Declare(
	  glfw
	  GIT_REPOSITORY https://github.com/glfw/glfw.git
	  GIT_TAG 3.4
  )
  FetchContent_MakeAvailable(glfw)
  
  set(GLFW_INCLUDE_DIR "${glfw_SOURCE_DIR}/include")
endif ()

# Automatically select the file name of the GLFW library based on the platform
if (MSVC)
  set(GLFW_LIB_NAME "glfw3.lib")  # Static library file suffix under Windows
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(GLFW_LIB_NAME "libglfw3d.a")  # Static library file suffix under Linux/macOS
  else ()
	set(GLFW_LIB_NAME "libglfw3.a")
  endif ()
endif ()
set(GLFW_LIBRARY "${CMAKE_BINARY_DIR}/lib/${GLFW_LIB_NAME}")

# Set Assimp build options to ensure static libraries are generated
set(ASSIMP_ENABLE_BOOST_WORKAROUND OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)

if(EXISTS "${THIRD_PARTY_DIR}/assimp-5.2.5")
  set(ASSIMP_INCLUDE_DIR "${THIRD_PARTY_DIR}/assimp-5.2.5/include/assimp")
else()
  include(FetchContent)
  FetchContent_Declare(
	  assimp
	  GIT_REPOSITORY https://github.com/assimp/assimp.git
	  GIT_TAG v5.2.5
  )
  FetchContent_MakeAvailable(assimp)
  
  set(ASSIMP_INCLUDE_DIR "${assimp_SOURCE_DIR}/include/assimp")
endif()

if (MSVC)
  # Dynamic calculation tool chain identifiers, such as vc143, vc144, etc
  math(EXPR TOOLSET "(${MSVC_VERSION}/10)-50")

  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(ASSIMP_MODE_SUFFIX "mtd")
  else ()
	set(ASSIMP_MODE_SUFFIX "mt")
  endif ()

  set(ASSIMP_LIB_NAME "Assimp-vc${TOOLSET}-${ASSIMP_MODE_SUFFIX}.lib")
  set(ASSIMP_DLL_NAME "Assimp-vc${TOOLSET}-${ASSIMP_MODE_SUFFIX}.dll")

  set(ASSIMP_LIBRARY "${CMAKE_BINARY_DIR}/lib/${ASSIMP_LIB_NAME}")

  message(STATUS "MSVC Assimp library path: ${ASSIMP_LIBRARY}")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(ASSIMP_MODE_SUFFIX "d")
  else ()
	set(ASSIMP_MODE_SUFFIX "")
  endif ()

  if (BUILD_SHARED_LIBS)
	set(ASSIMP_LIB_NAME "libassimp${ASSIMP_MODE_SUFFIX}.dll.a")
  else ()
	set(ASSIMP_LIB_NAME "libassimp${ASSIMP_MODE_SUFFIX}.a")
  endif ()

  set(ASSIMP_LIBRARY "${CMAKE_BINARY_DIR}/lib/${ASSIMP_LIB_NAME}")

  message(STATUS "GNU Assimp library path:${ASSIMP_LIBRARY}")
endif ()

include_directories(${GLFW_INCLUDE_DIR} ${ASSIMP_INCLUDE_DIR})
set(LIBS glfw assimp)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

link_directories(${CMAKE_SOURCE_DIR}/lib)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
# Essential modules cannot be omitted
find_package(GLM REQUIRED)
#find_package(GLFW3 REQUIRED)
#find_package(ASSIMP REQUIRED)
find_package(MODEL REQUIRED)
find_package(IMGUI REQUIRED)
# For dependent libraries, wait for the main library to be built before building.
add_dependencies(IMGUI glfw)
add_dependencies(MODEL assimp)

option(ENABLE_SQLite "SQLite is used as the platform for storing data." ON)
if (ENABLE_SQLite)
  find_package(SQLite REQUIRED)
endif ()

# Obtain the installation directory of the application
configure_file(configuration/root_directory.h.in ${CMAKE_BINARY_DIR}/configuration/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/configuration)

if (MSVC)
  set(LIBS ${LIBS} opengl32 MODEL IMGUI)
  set_source_files_properties(includes/sqlite/shell.c PROPERTIES COMPILE_FLAGS "/utf-8")
  set(LIBS ${LIBS} SQLite)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(LIBS ${LIBS} opengl32 MODEL IMGUI)
  if (ENABLE_SQLite)
	set_source_files_properties(includes/sqlite/shell.c PROPERTIES COMPILE_FLAGS "-finput-charset=UTF-8")
	set(LIBS ${LIBS} SQLite)
  endif ()
endif ()

set(HEADER_FILES_DIR "${CMAKE_SOURCE_DIR}/includes/include")
set(SOURCE_FILES_DIR "${CMAKE_SOURCE_DIR}/src/implementation")
set(CORE_HEADER_FILES_DIR "${HEADER_FILES_DIR}/Core")
set(CORE_SOURCE_FILES_DIR "${SOURCE_FILES_DIR}/Core")
set(TIME_HEADER_FILES_DIR "${HEADER_FILES_DIR}/Time")
set(TIME_SOURCE_FILES_DIR "${SOURCE_FILES_DIR}/Time")
# Create a master file set
set(MAIN_HEADER_FILES_DIR "${HEADER_FILES_DIR};${TIME_HEADER_FILES_DIR};${CORE_HEADER_FILES_DIR}")
set(MAIN_SOURCE_FILES_DIR "${SOURCE_FILES_DIR};${TIME_SOURCE_FILES_DIR};${CORE_SOURCE_FILES_DIR}")

set(IMGUI_HEADER_FILES_DIR "${HEADER_FILES_DIR}/ImGui")
set(IMGUI_SOURCE_FILES_DIR "${SOURCE_FILES_DIR}/ImGui")
set(IMGUI_FRONTS_HEADER_FILES_DIR "${IMGUI_HEADER_FILES_DIR}/Fonts")
set(IMGUI_FRONTS_SOURCES_FILES_DIR "${SOURCE_FILES_DIR}/ImGui/Fonts")
list(APPEND MAIN_HEADER_FILES_DIR "${IMGUI_HEADER_FILES_DIR}")
list(APPEND MAIN_SOURCE_FILES_DIR "${IMGUI_SOURCE_FILES_DIR}")
list(APPEND MAIN_HEADER_FILES_DIR "${IMGUI_FRONTS_HEADER_FILES_DIR}")
list(APPEND MAIN_SOURCE_FILES_DIR "${IMGUI_FRONTS_SOURCES_FILES_DIR}")

option(ENABLE_EXPERIMENTAL "A quick extension. It could be experimental (unsafe), or it could be a defragment of 
OpenGL code that doesn't fit in the main code area and is therefore placed here. s details will be noted at 
the beginning of each.h with ///." ON)
if (ENABLE_EXPERIMENTAL)
  set(EXPERIMENTAL_HEADER_FILES_DIR "${HEADER_FILES_DIR}/EXPERIMENTAL")
  set(EXPERIMENTAL_SOURCE_FILES_DIR "${SOURCE_FILES_DIR}/EXPERIMENTAL")
  list(APPEND MAIN_HEADER_FILES_DIR "${EXPERIMENTAL_HEADER_FILES_DIR}")
  list(APPEND MAIN_SOURCE_FILES_DIR "${EXPERIMENTAL_SOURCE_FILES_DIR}")
endif ()

# Traverse the master file set.
message(STATUS "Checking directories in MAIN_HEADER_FILES_DIR: ${MAIN_HEADER_FILES_DIR}")
set(MAIN_HEADER_FILES "")

foreach (HEADER_DIR IN LISTS MAIN_HEADER_FILES_DIR)
  file(GLOB_RECURSE HEADER_FILES_TMP
	  "${HEADER_DIR}/*.h"
	  "${HEADER_DIR}/*.hpp"
	  "${HEADER_DIR}/*inl")
  list(APPEND MAIN_HEADER_FILES ${HEADER_FILES_TMP})
endforeach ()

message(STATUS "Checking directories in MAIN_SOURCE_FILES_DIR: ${MAIN_SOURCE_FILES_DIR}")
set(MAIN_SOURCE_FILES "")

foreach (SOURCE_DIR IN LISTS MAIN_SOURCE_FILES_DIR)
  file(GLOB_RECURSE SOURCE_FILES_TMP
	  "${SOURCE_DIR}/*.cpp"
	  "${SOURCE_DIR}/*.cc")
  list(APPEND MAIN_SOURCE_FILES ${SOURCE_FILES_TMP})
endforeach ()

# First create relevant static libraries required for other projects
add_library(STB_IMAGE "src/stb_image.cpp")
set(LIBS ${LIBS} STB_IMAGE)

add_library(GLAD "${CMAKE_SOURCE_DIR}/src/glad.c")
target_include_directories(GLAD PUBLIC "${CMAKE_SOURCE_DIR}/includes")
set(LIBS ${LIBS} GLAD)

# Use CMake to check the number of cores and try to use multi-core synchronous builds.
cmake_host_system_information(RESULT CORE_COUNT QUERY NUMBER_OF_LOGICAL_CORES)
if (MSVC)
  add_compile_options(/MP${CORE_COUNT})
  set(CMAKE_LINK_INCREMENTAL YES)
  set(CMAKE_BUILD_PARALLEL_LEVEL ${CORE_COUNT})
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(CMAKE_MAKE_PROGRAM ${CMAKE_MAKE_PROGRAM} -j${CORE_COUNT})
  set(CMAKE_LINK_INCREMENTAL OFF)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections,-O0")
  set(CMAKE_LINK_INCREMENTAL YES)
  set(CMAKE_BUILD_PARALLEL_LEVEL ${CORE_COUNT})
endif ()

# Search all directories under OpenGL without having to manually enter folders.
file(GLOB DIRECTORIES LIST_DIRECTORIES true ${CMAKE_SOURCE_DIR}/src/OpenGL/*)
foreach (chapter ${DIRECTORIES})
  if (IS_DIRECTORY ${chapter})
	get_filename_component(NAME ${chapter} NAME)

	file(GLOB HEADER_FILES "${chapter}/*.h" "${chapter}/*.hpp" "${chapter}/*.inl")
	file(GLOB SOURCE_FILES "${chapter}/*.cpp" "${chapter}/*.cc")
	file(GLOB SHADER_FILES
		"${chapter}/*.glsl"
		"${chapter}/*.vert"
		"${chapter}/*.frag"
		"${chapter}/*.geom"
		"${chapter}/*.tesc"
		"${chapter}/*.tese"
		"${chapter}/*.gsh"
		"${chapter}/*.fsh"
		"${chapter}/*.comp"
		"${chapter}/*.vsh")

	list(APPEND SOURCE_FILES ${HEADER_FILES} ${SHADER_FILES} ${MAIN_HEADER_FILES} ${MAIN_SOURCE_FILES})

	add_executable(${NAME} ${SOURCE_FILES})
	target_link_libraries(${NAME} ${LIBS})

	set_target_properties(${NAME} PROPERTIES FOLDER "Executables")

	set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${NAME}")

	if (MSVC)
	  target_compile_options(${NAME} PRIVATE /std:c++17)
	  target_link_options(${NAME} PUBLIC /ignore:4099)
	endif (MSVC)

	# Copy shaders to the target folder if they do not exist
	foreach (SHADER ${SHADER_FILES})
	  get_filename_component(SHADER_NAME ${SHADER} NAME)
	  set(TARGET_SHADER_PATH "$<TARGET_FILE_DIR:${NAME}>/${SHADER_NAME}")
	  add_custom_command(TARGET ${NAME} POST_BUILD
		  COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${NAME}>
		  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SHADER} ${TARGET_SHADER_PATH}
		  COMMENT "Copying Shader: ${SHADER_NAME} to target folder if not already present.")
	endforeach ()

	add_custom_command(TARGET ${NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${NAME}>
    COMMENT "Creating target directory if not present.")

	# Gather all DLLs for this project
	file(GLOB DLLS "${CMAKE_BINARY_DIR}/dll/*.dll")

	# Ensure DLL files are copied after the executable is built
	if (DLLS)
	  foreach (DLL ${DLLS})
		get_filename_component(DLL_NAME ${DLL} NAME)
		set(TARGET_DLL_PATH "$<TARGET_FILE_DIR:${NAME}>/${DLL_NAME}")

		# Ensure DLL is copied only after the executable is built
		add_custom_command(TARGET ${NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${NAME}>
			COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DLL} ${TARGET_DLL_PATH}
			COMMENT "Copying DLL: ${DLL_NAME} to target folder if not already present.")
	  endforeach ()
	endif ()

	# Add the dependency on assimp here after the target is created
	add_dependencies(${NAME} assimp)  # Move this after target creation
  endif ()
endforeach ()

# Define the makeLink macro
macro(makeLink src dest target)
  add_custom_command(TARGET ${target} POST_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink ${src} ${dest} DEPENDS ${dest} COMMENT "mklink ${src} -> ${dest}")
endmacro()

# then create a project file per tutorial
foreach (CHAPTER ${CHAPTERS})
  create_project_from_sources(${CHAPTER})
endforeach (CHAPTER)

include_directories(${CMAKE_SOURCE_DIR}/includes)

# Load the necessary parameters for the test.
set(MAIN_HEADER_FILES "${MAIN_HEADER_FILES}" CACHE STRING "Main header files")
set(MAIN_SOURCE_FILES "${MAIN_SOURCE_FILES}" CACHE STRING "Main source files")
set(LIBS "${LIBS}" CACHE STRING "All the link libraries")
# Test module.
add_subdirectory(src/ctests)