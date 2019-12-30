cmake_minimum_required(VERSION 3.10.2)

# needed includes to make CMake work
include(CheckCXXCompilerFlag)
# installation directories (platform dependent)
include(GNUInstallDirs)

# **********************************************************************
# Custom functions
function(add_comp_flag flag)
	check_cxx_compiler_flag(${flag} CXX_HAS_FLAG)
	if (CXX_HAS_FLAG)
		message(STATUS "    Adding flag ${flag}...")
		target_compile_options(${TargetStringLibrary} PRIVATE ${flag})
	else()
		message(STATUS "    ! Compiler does not support flag ${flag}.")
	endif()
endfunction(add_comp_flag)

function(add_debug_flag flag)
	check_cxx_compiler_flag(${flag} HAS_FLAG)
	if (HAS_FLAG)
		message(STATUS "    Adding flag ${flag}...")
		SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${flag}" PARENT_SCOPE)
	else()
		message(STATUS "    ! Compiler does not support flag ${flag}.")
	endif()
endfunction(add_debug_flag)

function(add_release_flag flag)
	check_cxx_compiler_flag(${flag} HAS_FLAG)
	if (HAS_FLAG)
		message(STATUS "    Adding flag ${flag}...")
		SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${flag}" PARENT_SCOPE)
	else()
		message(STATUS "    ! Compiler does not support flag ${flag}.")
	endif()
endfunction(add_release_flag)

# **********************************************************************
# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# **********************************************************************
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

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	message(STATUS "Target library laldebug")
	set(TargetStringLibrary "laldebug")
else()
	message(STATUS "Target library lal")
	set(TargetStringLibrary "lal")
endif()

# **********************************************************************
# **********************************************************************

# project's version
set(LAL_MAJOR 2019)
set(LAL_MINOR 07)
set(LAL_PATCH 00)

# project declaration
project(LAL VERSION "${LAL_MAJOR}.${LAL_MINOR}.${LAL_PATCH}")
project(LAL DESCRIPTION "Linear Arrangement Library")

# declare library
add_library(${TargetStringLibrary} SHARED "")

# define library's properties
set_target_properties(${TargetStringLibrary} PROPERTIES VERSION "${LAL_MAJOR}.${LAL_MINOR}.${LAL_PATCH}")
set_target_properties(${TargetStringLibrary} PROPERTIES SOVERSION 1)
set_target_properties(${TargetStringLibrary} PROPERTIES LINKER_LANGUAGE CXX)

# **********************************************************************
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

# ******
# OpenMP
find_package(OpenMP REQUIRED)

# **********************************************************************
# SOURCE FILES FOR COMPILATION

# source files of the library
target_sources(
	${TargetStringLibrary}
	PRIVATE
		generation/lab_free_trees.cpp
		generation/ulab_free_trees.cpp
		generation/rand_lab_free_trees.cpp
		generation/rand_ulab_free_trees.cpp
		generation/rand_lab_dir_rooted_trees.cpp
		generation/rand_ulab_rooted_trees.cpp
		graphs/dgraph.cpp
		graphs/drtree.cpp
		graphs/dtree.cpp
		graphs/graph.cpp
		graphs/rtree.cpp
		graphs/tree.cpp
		graphs/ugraph.cpp
		graphs/urtree.cpp
		graphs/utree.cpp
		io/edge_list.cpp
		io/treebank_dataset.cpp
		io/treebank_processor.cpp
		io/treebank_reader.cpp
		iterators/edge_iterator.cpp
		iterators/Q_iterator.cpp
		linarr/C.cpp
		linarr/C_approx.cpp
		linarr/C_brute_force.cpp
		linarr/C_dyn_prog.cpp
		linarr/C_ladder.cpp
		linarr/C_stack_based.cpp
		linarr/classification.cpp
		linarr/D.cpp
		linarr/headedness.cpp
		numeric/gmp_utils.cpp
		numeric/integer.cpp
		numeric/rational.cpp
		properties/C_rla.cpp
		properties/D_rla.cpp
		properties/degrees.cpp
		properties/mhd.cpp
		properties/Q.cpp
		properties/variance_C_forests.cpp
		properties/variance_C_gen_graphs.cpp
		properties/variance_C_trees.cpp
		utils/conversions.cpp
)

# **********************************************************************
# PUBLIC HEADER FILES FOR INSTALLATION

# Public header files of the library (to be installed in the system).
# These are not installed through the usual PUBLIC_HEADER property,
# but via several custom variables
set(LIBLAL_MAIN_HEADER
	C.hpp
	D.hpp
	definitions.hpp
	generation.hpp
	graphs.hpp
	io.hpp
	iterators.hpp
	linarr.hpp
	numeric.hpp
	properties.hpp
)
set(LIBLAL_GENERATION_HEADER
	generation/lab_free_trees.hpp
	generation/ulab_free_trees.hpp
	generation/rand_lab_free_trees.hpp
	generation/rand_ulab_free_trees.hpp
	generation/rand_lab_dir_rooted_trees.hpp
	generation/rand_ulab_rooted_trees.hpp
)
set(LIBLAL_GRAPHS_HEADER
	graphs/dgraph.hpp
	graphs/drtree.hpp
	graphs/dtree.hpp
	graphs/graph.hpp
	graphs/output.hpp
	graphs/rtree.hpp
	graphs/tree.hpp
	graphs/ugraph.hpp
	graphs/urtree.hpp
	graphs/utree.hpp
)
set(LIBLAL_IO_HEADER
	io/basic_output.hpp
	io/dataset_error.hpp
	io/edge_list.hpp
	io/treebank_dataset.hpp
	io/treebank_processor.hpp
	io/treebank_reader.hpp
)
set(LIBLAL_ITERATORS_HEADER
	iterators/edge_iterator.hpp
	iterators/Q_iterator.hpp
)
set(LIBLAL_LINARR_HEADER
	linarr/1level.hpp
	linarr/1level_impl.hpp
	linarr/2level.hpp
	linarr/2level_impl.hpp
	linarr/algorithms_crossings.hpp
	linarr/C.hpp
	linarr/classification.hpp
	linarr/D.hpp
	linarr/headedness.hpp
	linarr/tree_structure_type.hpp
)
set(LIBLAL_NUMERIC_HEADER
	numeric/gmp_utils.hpp
	numeric/integer.hpp
	numeric/output.hpp
	numeric/rational.hpp
)
set(LIBLAL_PROPERTIES_HEADER
	properties/C_rla.hpp
	properties/D_rla.hpp
	properties/degrees.hpp
	properties/mhd.hpp
	properties/Q.hpp
)

# **********************************************************************
# INSTALLATION OF LIBRARY

message("-- LAL binaries will be installed in: ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
message("-- LAL headers  will be installed in: ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}/lal")

install(
	TARGETS ${TargetStringLibrary}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(FILES ${LIBLAL_MAIN_HEADER}			DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lal)
install(FILES ${LIBLAL_GENERATION_HEADER}	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lal/generation)
install(FILES ${LIBLAL_GRAPHS_HEADER}		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lal/graphs)
install(FILES ${LIBLAL_IO_HEADER}			DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lal/io)
install(FILES ${LIBLAL_ITERATORS_HEADER}	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lal/iterators)
install(FILES ${LIBLAL_LINARR_HEADER}		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lal/linarr)
install(FILES ${LIBLAL_NUMERIC_HEADER}		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lal/numeric)
install(FILES ${LIBLAL_PROPERTIES_HEADER}	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lal/properties)

# **********************************************************************
# COMPILATION FLAGS

# Warning flags
message(STATUS "Adding warning flags")
add_comp_flag(-Wpedantic Wshadow)
add_comp_flag(-Wall)
add_comp_flag(-Wextra)
add_comp_flag(-Wconversion)
add_comp_flag(-Wold-style-cast)
add_comp_flag(-Wrestrict)
add_comp_flag(-Wduplicated-cond)
add_comp_flag(-Wnon-virtual-dtor)
add_comp_flag(-Woverloaded-virtual)
add_comp_flag(-Wrestrict)

# OpenMP
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

# Optimisation flags
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	message(STATUS "Adding flags for 'debug' mode")
	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
	SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG")
endif()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
	message(STATUS "Adding flags for 'release' mode")
	add_release_flag(-fstrict-aliasing)
endif()

# include path (required by the compiler to find the headers)
target_include_directories(${TargetStringLibrary} PRIVATE ..)

target_link_libraries(${TargetStringLibrary} gmp)
