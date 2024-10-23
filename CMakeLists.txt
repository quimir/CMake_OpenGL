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

# Essential modules cannot be omitted
find_package(GLM REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(ASSIMP REQUIRED)
find_package(MODEL REQUIRED)

# Optional modules can be turned on or off to be omitted.
# Use imgui as an additional window. imgui can use opengl to create a simple window, better create graphical 
# interface and debug functions. Details please see: https://github.com/ocornut/imgui
option(ENABLE_IMGUI "Use imgui as an additional window. imgui can use opengl to create a simple window, 
better create graphical interface and debug functions. Details please see: https://github.com/ocornut/imgui" ON)
if (ENABLE_IMGUI)
  find_package(IMGUI REQUIRED)
endif ()

option(ENABLE_SQLite "SQLite is used as the platform for storing data." ON)
if (ENABLE_SQLite)
  find_package(SQLite REQUIRED)
endif ()

# Obtain the installation directory of the application
configure_file(configuration/root_directory.h.in ${CMAKE_BINARY_DIR}/configuration/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/configuration)

if (WIN32)
  set(LIBS glfw3 opengl32 assimp MODEL)
  if (ENABLE_IMGUI)
	set(LIBS ${LIBS} IMGUI)
  endif ()

  if (ENABLE_SQLite)
	set_source_files_properties(includes/sqlite/shell.c PROPERTIES COMPILE_FLAGS "/utf-8")
	set(LIBS ${LIBS} SQLite)
  endif ()
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
elseif (APPLE)
  INCLUDE_DIRECTORIES(/System/Library/Frameworks)
  FIND_LIBRARY(COCOA_LIBRARY Cocoa)
  FIND_LIBRARY(OpenGL_LIBRARY OpenGL)
  FIND_LIBRARY(IOKit_LIBRARY IOKit)
  FIND_LIBRARY(CoreVideo_LIBRARY CoreVideo)
  MARK_AS_ADVANCED(COCOA_LIBRARY OpenGL_LIBRARY)
  SET(APPLE_LIBS ${COCOA_LIBRARY} ${IOKit_LIBRARY} ${OpenGL_LIBRARY} ${CoreVideo_LIBRARY})
  SET(APPLE_LIBS ${APPLE_LIBS} ${GLFW3_LIBRARY} ${ASSIMP_LIBRARY} ${FREETYPE_LIBRARIES})
  set(LIBS ${LIBS} ${APPLE_LIBS})
endif (WIN32)

set(HEADER_FILES_DIR "${CMAKE_SOURCE_DIR}/includes/include")
set(SOURCE_FILES_DIR "${CMAKE_SOURCE_DIR}/src/implementation")
set(CORE_HEADER_FILES_DIR "${HEADER_FILES_DIR}/Core")
set(CORE_SOURCE_FILES_DIR "${SOURCE_FILES_DIR}/Core")
set(TIME_HEADER_FILES_DIR "${HEADER_FILES_DIR}/Time")
set(TIME_SOURCE_FILES_DIR "${SOURCE_FILES_DIR}/Time")
# Initialize ALL_HEADER_FILES_DIR and ALL_SOURCE_FILES_DIR
set(ALL_HEADER_FILES_DIR "${HEADER_FILES_DIR};${TIME_HEADER_FILES_DIR};${CORE_HEADER_FILES_DIR}")
set(ALL_SOURCE_FILES_DIR "${SOURCE_FILES_DIR};${TIME_SOURCE_FILES_DIR};${CORE_SOURCE_FILES_DIR}")

if (ENABLE_IMGUI)
  set(IMGUI_HEADER_FILES_DIR "${HEADER_FILES_DIR}/ImGui")
  set(IMGUI_SOURCE_FILES_DIR "${SOURCE_FILES_DIR}/ImGui")
  set(IMGUI_FRONTS_HEADER_FILES_DIR "${IMGUI_HEADER_FILES_DIR}/Fonts")
  set(IMGUI_FRONTS_SOURCES_FILES_DIR "${SOURCE_FILES_DIR}/ImGui/Fonts")
  list(APPEND ALL_HEADER_FILES_DIR "${IMGUI_HEADER_FILES_DIR}")
  list(APPEND ALL_SOURCE_FILES_DIR "${IMGUI_SOURCE_FILES_DIR}")
  list(APPEND ALL_HEADER_FILES_DIR "${IMGUI_FRONTS_HEADER_FILES_DIR}")
  list(APPEND ALL_SOURCE_FILES_DIR "${IMGUI_FRONTS_SOURCES_FILES_DIR}")
endif ()

option(ENABLE_EXPERIMENTAL "A quick extension. It could be experimental (unsafe), or it could be a defragment of 
OpenGL code that doesn't fit in the main code area and is therefore placed here. s details will be noted at 
the beginning of each.h with ///." ON)
if (ENABLE_EXPERIMENTAL)
  set(EXPERIMENTAL_HEADER_FILES_DIR "${HEADER_FILES_DIR}/EXPERIMENTAL")
  set(EXPERIMENTAL_SOURCE_FILES_DIR "${SOURCE_FILES_DIR}/EXPERIMENTAL")
  list(APPEND ALL_HEADER_FILES_DIR "${EXPERIMENTAL_HEADER_FILES_DIR}")
  list(APPEND ALL_SOURCE_FILES_DIR "${EXPERIMENTAL_SOURCE_FILES_DIR}")
endif ()

message(STATUS "Header directories: ${ALL_HEADER_FILES_DIR}")
message(STATUS "Source directories: ${ALL_SOURCE_FILES_DIR}")

# First create relevant static libraries required for other projects
add_library(STB_IMAGE "src/stb_image.cpp")
set(LIBS ${LIBS} STB_IMAGE)

add_library(GLAD "${CMAKE_SOURCE_DIR}/src/glad.c")
target_include_directories(GLAD PUBLIC "${CMAKE_SOURCE_DIR}/includes")
set(LIBS ${LIBS} GLAD)

set(CHAPTERS
	started_opengl
	model_loading
	started_opengl_texture
	skeletal_animation
	frame_buffers
	advanced_OpenGL
	advanced_Lighting)

function(create_project_from_sources chapter)
  # Please do not delete this paragraph, because this is adding the files in the project folder to the project.
  list(APPEND ALL_HEADER_FILES_DIR "src/${chapter}")
  # Glob all header files from the directories
  foreach (HEADER_DIR IN LISTS ALL_HEADER_FILES_DIR)
	file(GLOB HEADER_FILES_TMP
		"${HEADER_DIR}/*.h"
		"${HEADER_DIR}/*.hpp"
		"${HEADER_DIR}/*.inl")
	list(APPEND HEADER_FILES ${HEADER_FILES_TMP})
  endforeach ()

  # Please do not delete this paragraph, because this is adding the files in the project folder to the project.
  list(APPEND ALL_SOURCE_FILES_DIR "src/${chapter}")
  # Glob all source files from the directories
  foreach (SOURCE_DIR IN LISTS ALL_SOURCE_FILES_DIR)
	file(GLOB SOURCE_FILES_TMP
		"${SOURCE_DIR}/*.cpp"
		"${SOURCE_DIR}/*.cc")
	list(APPEND SOURCE_FILES ${SOURCE_FILES_TMP})
  endforeach ()

  file(GLOB SHADER_FILES
	  "src/${chapter}/*.glsl"
	  "src/${chapter}/*.vert"
	  "src/${chapter}/*.frag"
	  "src/${chapter}/*.geom"
	  "src/${chapter}/*.tesc"
	  "src/${chapter}/*.tese"
	  "src/${chapter}/*.gsh"
	  "src/${chapter}/*.fsh"
	  "src/${chapter}/*.comp"
	  "src/${chapter}/*.vsh")

  list(APPEND SOURCE_FILES
	  ${HEADER_FILES}
	  ${SHADER_FILES})

  set(NAME "${chapter}")

  add_executable(${NAME} ${SOURCE_FILES})
  target_link_libraries(${NAME} ${LIBS})

  if (MSVC)
	target_compile_options(${NAME} PRIVATE /std:c++17 /MP)
	target_link_options(${NAME} PUBLIC /ignore:4099)
  endif (MSVC)

  if (WIN32)
	set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${chapter}")
	set_target_properties(${NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${chapter}/Debug")
  elseif (APPLE)
	set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${chapter}")
	set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/${chapter}")
	set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/${chapter}")
  endif (WIN32)

  foreach (SHADER ${SHADER_FILES})
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
set(ALL_HEADER_FILES_DIR "${ALL_HEADER_FILES_DIR}" CACHE STRING "All header files directory")
set(ALL_SOURCE_FILES_DIR "${ALL_SOURCE_FILES_DIR}" CACHE STRING "All source files directory")
set(LIBS "${LIBS}" CACHE STRING "All the link libraries")
# Test module.
add_subdirectory(src/ctests)