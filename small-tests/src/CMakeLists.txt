cmake_minimum_required(VERSION 3.10.2)

project(SMALL_TESTS)

# needed includes to make CMake work
include(CheckCXXCompilerFlag)
# installation directories (platform dependent)
include(GNUInstallDirs)

# ******************************************************************************
# Custom functions
function(add_compiler_flag flag)
	check_cxx_compiler_flag(${flag} CXX_HAS_FLAG_${flag})
	
	if (CXX_HAS_FLAG_${flag})
		SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}" PARENT_SCOPE)
	else()
		message(STATUS "    ! Compiler does not support flag ${flag}.")
	endif()
endfunction(add_compiler_flag)

function(add_debug_flag flag)
	check_cxx_compiler_flag(${flag} CXX_HAS_FLAG_${flag})
	
	if (CXX_HAS_FLAG_${flag})
		SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${flag}" PARENT_SCOPE)
	else()
		message(STATUS "    ! Compiler does not support flag ${flag}.")
	endif()
endfunction(add_debug_flag)

function(add_release_flag flag)
	check_cxx_compiler_flag(${flag} CXX_HAS_FLAG_${flag})
	
	if (CXX_HAS_FLAG_${flag})
		SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${flag}" PARENT_SCOPE)
	else()
		message(STATUS "    ! Compiler does not support flag ${flag}.")
	endif()
endfunction(add_release_flag)

# ******************************************************************************
# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ******************************************************************************
# Set default build type
# (following the guide in https://blog.kitware.com/cmake-and-the-default-build-type/)

set(default_build_type "Release")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
	set(
		CMAKE_BUILD_TYPE "${default_build_type}"
		CACHE STRING "Choose the type of build."
		FORCE
	)
	# Set the possible values of build type for cmake-gui
	set_property(
		CACHE CMAKE_BUILD_TYPE
		PROPERTY
			STRINGS "Debug" "Release"
	)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Looking for the Linear Arrangement Library
if (NOT DEFINED LAL_DEV_DIR)
	message(STATUS "LAL's development root directory was not given.")
	message(STATUS "    Note that even if the library is installed in the system, these tests")
	message(STATUS "    use 'private' headers of the library, which are not distributed.")
	message(STATUS "    Please, rerun 'cmake' the location of LAL's development root directory:")
	message(STATUS "        -DLAL_DEV_DIR=/path/to/the/library")
	message(STATUS "    Make sure it is an absolute path!")
	message(FATAL_ERROR "Please, read the last few lines and follow the instructions given.")
else()
	if (NOT DEFINED LAL_DEV_DIR)
		message(STATUS "Path to the root directory of the linear arrangement library not given.")
		message(STATUS "    Use -DLAL_DIR=/path/to/the/library")
		message(STATUS "Rerun 'cmake' with the appropriate arguments")
		message(FATAL_ERROR "Please, read the last few lines and follow the instructions given.")
	else()
		if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
			message(STATUS "Link against laldebug (debug compilation of lal)")
			set(LAL_LIB_DIR ${LAL_DEV_DIR}/lal-debug)
			set(LAL_LIB_NAME lal)
		else()
			message(STATUS "Link against lal (release compilation of lal)")
			set(LAL_LIB_DIR ${LAL_DEV_DIR}/lal-release)
			set(LAL_LIB_NAME laloptimized)
		endif()
		message(STATUS "    LAL_LIB_DIR:  ${LAL_LIB_DIR}")
		message(STATUS "    LAL_LIB_NAME: ${LAL_LIB_NAME}")
	endif()
endif()

# ----------------------
# Find system libraries

# ***
# GMP
find_library(GMP_LIB gmp)
if (GMP_LIB)
	message(STATUS "Found gmp: ${GMP_LIB}")
else()
	message(FATAL_ERROR "Error: gmp library not found.")
endif()

# ******
# OpenMP
find_package(OpenMP REQUIRED)

# ******************************************************************************
# PROJECT

add_executable(
	small_tests
		main.cpp
)

# ******************************************************************************
# DEPENDENCIES

# ----------------------
# Add external libraries

# self includes
target_include_directories(small_tests	PUBLIC .)

# LAL
# lal includes
target_include_directories(small_tests	PUBLIC ${LAL_DEV_DIR})
# lal linking
if (UNIX)
target_link_libraries(small_tests	${LAL_LIB_DIR}/lib${LAL_LIB_NAME}.so)
endif (UNIX)
if (WIN32)
target_link_libraries(small_tests	${LAL_LIB_DIR}/lib${LAL_LIB_NAME}.dll)
endif (WIN32)

# ******************************************************************************
# COMPILATION FLAGS

# OpenMP
target_link_libraries(small_tests	${OpenMP_CXX_LIBRARIES})

# GMP
target_link_libraries(small_tests	gmp)

# Optimisation and warning flags
add_compiler_flag(-Wall)
add_compiler_flag(-Wextra)
add_compiler_flag(-Wshadow)
add_compiler_flag(-Wnon-virtual-dtor)
add_compiler_flag(-Wold-style-cast)
add_compiler_flag(-Wcast-align)
add_compiler_flag(-Wunused)
add_compiler_flag(-Woverloaded-virtual)
add_compiler_flag(-Wpedantic)
add_compiler_flag(-Wconversion)
add_compiler_flag(-Wsign-conversion)
add_compiler_flag(-Wnull-dereference)
add_compiler_flag(-Wdouble-promotion)
add_compiler_flag(-Wformat=2)
add_compiler_flag(-Wduplicated-cond)
add_compiler_flag(-Wduplicated-branches)
add_compiler_flag(-Wlogical-op)
add_compiler_flag(-Wuseless-cast)
add_compiler_flag(-Wrestrict)
add_compiler_flag(-D__LAL_INSPECT)
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	add_debug_flag(-Og)
	add_debug_flag(-DDEBUG)
	add_debug_flag(-D_GLIBCXX_DEBUG)
else()
	# these two are added by default
	#add_release_flag(-O3)
	#add_release_flag(-DNDEBUG)
	add_release_flag(-UDEBUG)
	add_release_flag(-fstrict-aliasing)
endif()

