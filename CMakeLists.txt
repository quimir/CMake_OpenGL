cmake_minimum_required(VERSION 3.20)

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

set(OPENGL_TARGET_DLLS ${CMAKE_SOURCE_DIR}/dlls)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

link_directories(${CMAKE_SOURCE_DIR}/lib)

find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")

# Obtain the installation directory of the application
configure_file(configuration/root_directory.h.in configuration/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/configuration)

set(LIBS glfw3 opengl32)

# First create relevant static libraries required for other projects
add_library(STB_IMAGE "src/stb_image.cpp")
set(LIBS ${LIBS} STB_IMAGE)
add_library(GLAD "${CMAKE_SOURCE_DIR}/src/glad.c")
set(LIBS ${LIBS} GLAD)

if (MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif ()

set(CHAPTERS
		started_opengl)

function(create_project_from_sources chapter)
	file(GLOB SOURCE
			"src/${chapter}/*.h"
			"src/${chapter}/*.cpp"
			"src/${chapter}/*.cc"
			"src/${chapter}/*.glsl"
			"src/${chapter}/*.vert"
			"src/${chapter}/*.frag"
			"src/${chapter}/*.geom"
			"src/${chapter}/*.tesc"
			"src/${chapter}/*.tese"
			"src/${chapter}/*.gsh"
			"src/${chapter}/*.fsh"
			"src/${chapter}/*.comp"
			"src/${chapter}/*.vsh"
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

	# copy dlls
	file(GLOB DLLS "dlls/*.dll")
	foreach (SHADER ${SHADERS})
		if (WIN32)
			# configure_file(${SHADER} "test")
			add_custom_command(TARGET ${NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SHADER} $<TARGET_FILE_DIR:${NAME}>)
			add_custom_command(TARGET ${NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${DLLS} $<TARGET_FILE_DIR:${NAME}>)
		endif (WIN32)
	endforeach ()
endfunction()

# then create a project file per tutorial
foreach (CHAPTER ${CHAPTERS})
	create_project_from_sources(${CHAPTER})
endforeach (CHAPTER)

#file(GLOB SOURCE_FILES
#        "${CMAKE_SOURCE_DIR}/includes/include/*.h"
#        "${CMAKE_SOURCE_DIR}/src/*.cc")
#
#add_executable(${OPENGL_TARGET_NAME} ${SOURCE_FILES})

#target_link_libraries(${OPENGL_TARGET_NAME} PRIVATE ${LIBS})

include_directories(${CMAKE_SOURCE_DIR}/includes)