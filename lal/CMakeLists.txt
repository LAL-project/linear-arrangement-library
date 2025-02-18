cmake_minimum_required(VERSION 3.10.2)

# project's version
set(LAL_MAJOR 99.99)
set(LAL_PATCH 99)

# project declaration
project(LAL VERSION "${LAL_MAJOR}.${LAL_PATCH}")
project(LAL DESCRIPTION "Linear Arrangement Library")

# needed includes to make CMake work
include(CheckCXXCompilerFlag)
# installation directories (platform dependent)
include(GNUInstallDirs)

# IPO: Inter-Procedural Optimization
#include(CheckIPOSupported)
#check_ipo_supported(RESULT IPO_IS_SUPPORTED OUTPUT IPO_CHECK_ERROR)
#message(STATUS "IPO_IS_SUPPORTED='${IPO_IS_SUPPORTED}'")
#if("${IPO_IS_SUPPORTED}" STREQUAL "YES")
#	message(STATUS "    IPO is supported.")
#else()
#	message(STATUS "    IPO is not supported.")
#	message(STATUS "    Error message: ${IPO_CHECK_ERROR}")
#endif()

# ******************************************************************************
# Custom functions
function(add_compiler_flag flag)
	check_cxx_compiler_flag(${flag} CXX_HAS_FLAG_${flag})
	
	if (CXX_HAS_FLAG_${flag})
		target_compile_options(${TargetStringLibrary} PRIVATE ${flag})
	endif()
endfunction(add_compiler_flag)

# ******************************************************************************
# Use C++23
set(CMAKE_CXX_STANDARD 23)
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

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	message(STATUS "Target library lal")
	set(TargetStringLibrary "lal")
else()
	message(STATUS "Target library laloptimized")
	set(TargetStringLibrary "laloptimized")
endif()

# ******************************************************************************
# ******************************************************************************

# declare library
add_library(${TargetStringLibrary} SHARED "")

# define library's properties
set_target_properties(${TargetStringLibrary} PROPERTIES VERSION "${LAL_MAJOR}.${LAL_PATCH}")
set_target_properties(${TargetStringLibrary} PROPERTIES SOVERSION 1)
set_target_properties(${TargetStringLibrary} PROPERTIES LINKER_LANGUAGE CXX)

# linker time optimization
if("${IPO_IS_SUPPORTED}" STREQUAL "YES")
	message(STATUS "Turning on Inter-Procedural Optimization (IPO)")
	set_target_properties(${TargetStringLibrary} PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

# ******************************************************************************
# LIBRARIES
# check if libraries are installed in the system

# ***
# GMP
find_library(GMP_LIB gmp)
if (GMP_LIB)
	message(STATUS "Found gmp: ${GMP_LIB}")
else()
	message(FATAL_ERROR "Error: gmp library not found")
endif()

# ***
# pthread
find_library(PTHREAD_LIB pthread)
if (PTHREAD_LIB)
	message(STATUS "Found pthread: ${PTHREAD_LIB}")
else()
	message(FATAL_ERROR "Error: pthread library not found")
endif()

# ******
# OpenMP
find_package(OpenMP REQUIRED)

# ******************************************************************************
# HEADER AND SOURCE FILES FOR COMPILATION

file(GLOB_RECURSE SOURCES ../lal/*)
target_sources(${TargetStringLibrary} PRIVATE ${SOURCES})

# **************************************************************************** #
# PUBLIC HEADER FILES FOR INSTALLATION

# Public header files of the library (to be installed in the system).
# These are not installed through the usual PUBLIC_HEADER property,
# but via several custom variables

# ---------------------------------------------------------------------------- #
file(GLOB LIBLAL_MAIN_HEADER ../lal/*.hpp)
# ---------------------------------------------------------------------------- #
set(LIBLAL_DETAIL_HEADER
	detail/array.hpp
	detail/arrangement_wrapper.hpp
)
# ---------------------------------------------------------------------------- #
file(GLOB_RECURSE LIBLAL_GENERATION_HEADER ../lal/generate/*.hpp)
# ---------------------------------------------------------------------------- #
file(GLOB_RECURSE LIBLAL_GRAPHS_HEADER ../lal/graphs/*.hpp)
# ---------------------------------------------------------------------------- #
file(GLOB_RECURSE LIBLAL_IO_HEADER ../lal/io/*.hpp)
# ---------------------------------------------------------------------------- #
file(GLOB_RECURSE LIBLAL_ITERATORS_HEADER ../lal/iterators/*.hpp)
# ---------------------------------------------------------------------------- #
file(GLOB_RECURSE LIBLAL_LINARR_MAIN_HEADER ../lal/linarr/*.hpp)
file(GLOB_RECURSE LIBLAL_LINARR_aggregations_HEADER ../lal/linarr/aggregations/*.hpp)
file(GLOB_RECURSE LIBLAL_LINARR_C_HEADER ../lal/linarr/C/*.hpp)
file(GLOB_RECURSE LIBLAL_LINARR_chunking_HEADER ../lal/linarr/chunking/*.hpp)
file(GLOB_RECURSE LIBLAL_LINARR_D_HEADER ../lal/linarr/D/*.hpp)
file(GLOB_RECURSE LIBLAL_LINARR_syndeptree_HEADER ../lal/linarr/syntactic_dependency_tree/*.hpp)
# ---------------------------------------------------------------------------- #
file(GLOB_RECURSE LIBLAL_NUMERIC_HEADER ../lal/numeric/*.hpp)
# ---------------------------------------------------------------------------- #
file(GLOB_RECURSE LIBLAL_PROPERTIES_HEADER ../lal/properties/*.hpp)
# ---------------------------------------------------------------------------- #
file(GLOB_RECURSE LIBLAL_UTILITIES_HEADER ../lal/utilities/*.hpp)
# ******************************************************************************

# ******************************************************************************
# INSTALLATION OF LIBRARY

message("-- LAL binaries will be installed in: ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
message("-- LAL headers  will be installed in: ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}/lal")

install(
	TARGETS ${TargetStringLibrary}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(FILES ${LIBLAL_MAIN_HEADER}					DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lal)
install(FILES ${LIBLAL_DETAIL_HEADER}				DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lal/detail)
install(FILES ${LIBLAL_GENERATION_HEADER}			DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lal/generate)
install(FILES ${LIBLAL_GRAPHS_HEADER}				DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lal/graphs)
install(FILES ${LIBLAL_IO_HEADER}					DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lal/io)
install(FILES ${LIBLAL_ITERATORS_HEADER}			DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lal/iterators)
install(FILES ${LIBLAL_LINARR_MAIN_HEADER}			DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lal/linarr)
install(FILES ${LIBLAL_LINARR_aggregations_HEADER}	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lal/linarr/aggregations)
install(FILES ${LIBLAL_LINARR_C_HEADER}				DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lal/linarr/C)
install(FILES ${LIBLAL_LINARR_chunking_HEADER}		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lal/linarr/chunking)
install(FILES ${LIBLAL_LINARR_D_HEADER}				DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lal/linarr/D)
install(FILES ${LIBLAL_LINARR_syndeptree_HEADER}	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lal/linarr/syntactic_dependency_tree)
install(FILES ${LIBLAL_NUMERIC_HEADER}				DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lal/numeric)
install(FILES ${LIBLAL_PROPERTIES_HEADER}			DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lal/properties)
install(FILES ${LIBLAL_UTILITIES_HEADER}			DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lal/utilities)

# ******************************************************************************
# COMPILATION FLAGS

# Warning flags
message(STATUS "Adding warning flags")
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

# OpenMP
add_compiler_flag(${OpenMP_CXX_FLAGS})

# Optimisation flags
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	add_compiler_flag(-Og)
	add_compiler_flag(-DDEBUG)
	add_compiler_flag(-D_GLIBCXX_DEBUG)
else()
	# flags added by default:
	#   -O3
	#   -DNDEBUG
	add_compiler_flag(-UDEBUG)
endif()

# include path (required by the compiler to find the headers)
target_include_directories(${TargetStringLibrary} PRIVATE ..)

# ******************************************************************************
# DEPENDENCIES

# GMP
target_link_libraries(${TargetStringLibrary} 	gmp)

# pthread
target_link_libraries(${TargetStringLibrary}	pthread)

# OpenMP
target_link_libraries(${TargetStringLibrary}	${OpenMP_CXX_LIBRARIES})
