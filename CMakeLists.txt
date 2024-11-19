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
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

link_directories(${CMAKE_SOURCE_DIR}/lib)

include(cmake/modules/find_paths.cmake)

# Essential modules cannot be omitted
find_package(GLFW REQUIRED)
find_package(ASSIMP REQUIRED)
find_package(GLM REQUIRED)
find_package(IMGUI REQUIRED)

option(ENABLE_SQLite "SQLite is used as the platform for storing data." ON)
if (ENABLE_SQLite)
  find_package(SQLite REQUIRED)
endif ()

# Obtain the installation directory of the application
configure_file(configuration/root_directory.h.in ${CMAKE_BINARY_DIR}/configuration/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/configuration)

if (MSVC)
  set(LIBS ${GLFW_TARGET} ${ASSIMP_TARGET} opengl32 IMGUI)
  if (ENABLE_SQLite)
	set_source_files_properties(includes/sqlite/shell.c PROPERTIES COMPILE_FLAGS "/utf-8")
	set(LIBS ${LIBS} SQLite)
  endif ()
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(LIBS ${GLFW_TARGET} ${ASSIMP_TARGET} opengl32 IMGUI)
  if (ENABLE_SQLite)
	set_source_files_properties(includes/sqlite/shell.c PROPERTIES COMPILE_FLAGS "-finput-charset=UTF-8")
	set(LIBS ${LIBS} SQLite)
  endif ()
endif ()

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

# Define the makeLink macro
macro(makeLink src dest target)
  add_custom_command(TARGET ${target} POST_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink ${src} ${dest} DEPENDS ${dest} COMMENT "mklink ${src} -> ${dest}")
endmacro()

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

	list(APPEND SOURCE_FILES ${HEADER_FILES} ${SHADER_FILES} ${MAIN_SOURCE_FILES})

	add_executable(${NAME} ${SOURCE_FILES} ${MAIN_SOURCE_FILES})
	target_include_directories(${NAME} PRIVATE ${MAIN_HEADER_FILES_DIR})

	target_link_libraries(${NAME} ${LIBS})

	set_target_properties(${NAME} PROPERTIES FOLDER "Executables")

	set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${NAME}")

	if (MSVC)
	  target_compile_options(${NAME} PRIVATE /std:c++17)
	  target_link_options(${NAME} PUBLIC /ignore:4099)
	endif (MSVC)

	# Copy shaders to the target folder if they do not exist
	foreach (SHADER ${SHADER_FILES})
	  add_custom_command(TARGET ${NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SHADER} $<TARGET_FILE_DIR:${NAME}>)
	endforeach ()

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
	add_dependencies(${NAME} ${ASSIMP_TARGET})  # Move this after target creation
  endif ()
endforeach ()

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