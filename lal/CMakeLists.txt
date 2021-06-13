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
set(LAL_MAJOR xx.yy)
set(LAL_PATCH pp)

# project declaration
project(LAL VERSION "${LAL_MAJOR}.${LAL_PATCH}")
project(LAL DESCRIPTION "Linear Arrangement Library")

# declare library
add_library(${TargetStringLibrary} SHARED "")

# define library's properties
set_target_properties(${TargetStringLibrary} PROPERTIES VERSION "${LAL_MAJOR}.${LAL_PATCH}")
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
		# generate namespace
		# -- headers
		generate/all_arrangements.hpp
		generate/all_lab_free_trees.hpp
		generate/all_lab_rooted_trees.hpp
		generate/all_planar_arrangements.hpp
		generate/all_projective_arrangements.hpp
		generate/all_ulab_free_trees.hpp
		generate/all_ulab_rooted_trees.hpp
		generate/rand_arrangements.hpp
		generate/rand_lab_free_trees.hpp
		generate/rand_lab_rooted_trees.hpp
		generate/rand_planar_arrangements.hpp
		generate/rand_projective_arrangements.hpp
		generate/rand_ulab_free_trees.hpp
		generate/rand_ulab_rooted_trees.hpp
		generate/tree_generator.hpp
		generate/tree_generator_type.hpp
		# -- sources
		generate/all_lab_free_trees.cpp
		generate/all_planar_arrangements.cpp
		generate/all_projective_arrangements.cpp
		generate/all_ulab_free_trees.cpp
		generate/all_ulab_rooted_trees.cpp
		generate/rand_lab_free_trees.cpp
		generate/rand_planar_arrangements.cpp
		generate/rand_projective_arrangements.cpp
		generate/rand_ulab_free_trees.cpp
		generate/rand_ulab_rooted_trees.cpp
		
		# graphs namespace
		# --headers
		graphs/conversions.hpp
		graphs/directed_graph.hpp
		graphs/free_tree.hpp
		graphs/graph.hpp
		graphs/output.hpp
		graphs/rooted_tree.hpp
		graphs/tree.hpp
		graphs/tree_type.hpp
		graphs/undirected_graph.hpp
		# -- sources
		graphs/conversions.cpp
		graphs/directed_graph.cpp
		graphs/free_tree.cpp
		graphs/graph.cpp
		graphs/rooted_tree.cpp
		graphs/tree.cpp
		graphs/undirected_graph.cpp

		# internal namespace -- not to be installed
		# -- headers
		#      generate
		#          nothing
		#      graphs
		internal/graphs/conversions.hpp
		internal/graphs/cycles.hpp
		internal/graphs/enumerate_sets.hpp
		internal/graphs/is_tree.hpp
		internal/graphs/make_arrangement.hpp
		internal/graphs/reachability.hpp
		internal/graphs/retrieve_subtrees.hpp
		internal/graphs/size_subtrees.hpp
		internal/graphs/traversal.hpp
		internal/graphs/tree_classification.hpp
		internal/graphs/tree_type.hpp
		internal/graphs/union_find.hpp
		internal/graphs/utils.hpp
		#      io
		internal/io/check_correctness.hpp
		internal/io/treebank_feature.hpp
		#      iterators
		#          nothing
		#      linarr
		internal/linarr/algorithms_crossings.hpp
		internal/linarr/Dmin.hpp
		internal/linarr/syntactic_dependency_structure.hpp
		#      numeric
		internal/numeric/utils.hpp
		#      properties
		internal/properties/tree_centre.hpp
		internal/properties/tree_centroid.hpp
		internal/properties/tree_diameter.hpp
		#      sorting
		internal/sorting/bit_sort.hpp
		internal/sorting/counting_sort.hpp
		internal/sorting/insertion_sort.hpp
		#      utilities
		internal/utilities/tree_isomorphism.hpp
		#      others
		internal/avl.hpp
		internal/data_array.hpp
		internal/macros.hpp
		internal/make_array.hpp
		internal/sorted_vector.hpp
		
		# -- sources
		#      generate
		#          nothing
		#      graphs
		#          nothing
		#      io
		#          nothing
		#      iterators
		#          nothing
		#      linarr
		internal/linarr/C_brute_force.cpp
		internal/linarr/C_dyn_prog.cpp
		internal/linarr/C_ladder.cpp
		internal/linarr/C_stack_based.cpp
		internal/linarr/Dmin_Planar.cpp
		internal/linarr/Dmin_Projective.cpp
		internal/linarr/Dmin_Unconstrained_FC.cpp
		internal/linarr/Dmin_Unconstrained_YS.cpp
		#      numeric
		internal/numeric/utils.cpp
		#      properties
		#          nothing
		#      sorting
		#          nothing
		#      utilities
		#          nothing
		#      others
		#          nothing

		# io namespace
		# -- header
		io/basic_output.hpp
		io/check_correctness.hpp
		io/edge_list.hpp
		io/head_vector.hpp
		io/process_treebank_base.hpp
		io/report_correctness.hpp
		io/treebank_collection_processor.hpp
		io/treebank_collection_reader.hpp
		io/treebank_error.hpp
		io/treebank_error_type.hpp
		io/treebank_feature.hpp
		io/treebank_processor.hpp
		io/treebank_reader.hpp
		# -- sources
		io/check_correctness.cpp
		io/edge_list.cpp
		io/head_vector.cpp
		io/treebank_collection_processor.cpp
		io/treebank_collection_reader.cpp
		io/treebank_processor.cpp
		io/treebank_reader.cpp

		# iterators namespace
		# -- headers
		iterators/E_iterator.hpp
		iterators/Q_iterator.hpp
		# -- sources
				# no sources

		# linarr namespace
		# --headers
		linarr/1level.hpp
		linarr/2level.hpp
		linarr/algorithms_C.hpp
		linarr/algorithms_Dmin.hpp
		linarr/C.hpp
		linarr/classify_syntactic_dependency_structure.hpp
		linarr/D.hpp
		linarr/dependency_flux.hpp
		linarr/Dmin.hpp
		linarr/flux.hpp
		linarr/formal_constraints.hpp
		linarr/head_initial.hpp
		linarr/syntactic_dependency_structure.hpp
		# -- sources
		linarr/C.cpp
		linarr/classify_syntactic_dependency_structure.cpp
		linarr/D.cpp
		linarr/Dmin.cpp
		linarr/flux.cpp
		linarr/head_initial.cpp
		linarr/predicted_C.cpp

		# numeric namespace
		# -- headers
		numeric/integer.hpp
		numeric/integer_output.hpp
		numeric/output.hpp
		numeric/rational.hpp
		numeric/rational_output.hpp
		# -- sources
		numeric/integer.cpp
		numeric/rational.cpp

		# properties namespace
		# -- headers
		properties/C_rla.hpp
		properties/D_rla.hpp
		properties/degrees.hpp
		properties/mean_hierarchical_distance.hpp
		properties/Q.hpp
		properties/tree_centre.hpp
		properties/tree_centroid.hpp
		properties/tree_diameter.hpp
		# -- sources
		properties/C_rla.cpp
		properties/D_rla.cpp
		properties/degrees.cpp
		properties/mean_hierarchical_distance.cpp
		properties/Q.cpp
		properties/tree_centre.cpp
		properties/tree_centroid.cpp
		properties/tree_diameter.cpp
		properties/variance_C_forests.cpp
		properties/variance_C_gen_graphs.cpp
		
		# utilities namespace
		# -- headers
		utilities/tree_isomorphism.hpp
		# -- sources
		utilities/tree_isomorphism.cpp

		# lal namespace
		# -- heaers
		C.hpp
		D.hpp
		definitions.hpp
		generate.hpp
		graphs.hpp
		io.hpp
		iterators.hpp
		lal_version.hpp
		linarr.hpp
		namespaces.hpp
		numeric.hpp
		properties.hpp
		utilities.hpp
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
	generate.hpp
	graphs.hpp
	io.hpp
	iterators.hpp
	lal_version.hpp
	linarr.hpp
	numeric.hpp
	properties.hpp
	utilities.hpp
)
set(LIBLAL_GENERATION_HEADER
	generate/all_arrangements.hpp
	generate/all_lab_free_trees.hpp
	generate/all_lab_rooted_trees.hpp
	generate/all_planar_arrangements.hpp
	generate/all_projective_arrangements.hpp
	generate/all_ulab_free_trees.hpp
	generate/all_ulab_rooted_trees.hpp
	generate/rand_arrangements.hpp
	generate/rand_lab_free_trees.hpp
	generate/rand_lab_rooted_trees.hpp
	generate/rand_planar_arrangements.hpp
	generate/rand_projective_arrangements.hpp
	generate/rand_ulab_free_trees.hpp
	generate/rand_ulab_rooted_trees.hpp
	generate/tree_generator.hpp
	generate/tree_generator_type.hpp
)
set(LIBLAL_GRAPHS_HEADER
	graphs/conversions.hpp
	graphs/directed_graph.hpp
	graphs/free_tree.hpp
	graphs/graph.hpp
	graphs/output.hpp
	graphs/rooted_tree.hpp
	graphs/tree.hpp
	graphs/tree_type.hpp
	graphs/undirected_graph.hpp
)
set(LIBLAL_INTERNAL_HEADER
	internal/data_array.hpp
)
set(LIBLAL_IO_HEADER
	io/basic_output.hpp
	io/check_correctness.hpp
	io/edge_list.hpp
	io/head_vector.hpp
	io/process_treebank_base.hpp
	io/report_correctness.hpp
	io/treebank_collection_processor.hpp
	io/treebank_collection_reader.hpp
	io/treebank_error.hpp
	io/treebank_error_type.hpp
	io/treebank_feature.hpp
	io/treebank_processor.hpp
	io/treebank_reader.hpp
)
set(LIBLAL_ITERATORS_HEADER
	iterators/E_iterator.hpp
	iterators/Q_iterator.hpp
)
set(LIBLAL_LINARR_HEADER
	linarr/1level.hpp
	linarr/2level.hpp
	linarr/algorithms_C.hpp
	linarr/algorithms_Dmin.hpp
	linarr/C.hpp
	linarr/classify_syntactic_dependency_structure.hpp
	linarr/D.hpp
	linarr/dependency_flux.hpp
	linarr/Dmin.hpp
	linarr/flux.hpp
	linarr/formal_constraints.hpp
	linarr/head_initial.hpp
	linarr/syntactic_dependency_structure.hpp
)
set(LIBLAL_NUMERIC_HEADER
	numeric/integer.hpp
	numeric/integer_output.hpp
	numeric/output.hpp
	numeric/rational.hpp
	numeric/rational_output.hpp
)
set(LIBLAL_PROPERTIES_HEADER
	properties/C_rla.hpp
	properties/D_rla.hpp
	properties/degrees.hpp
	properties/mean_hierarchical_distance.hpp
	properties/Q.hpp
	properties/tree_centre.hpp
	properties/tree_centroid.hpp
	properties/tree_diameter.hpp
)
set(LIBLAL_UTILITIES_HEADER
	utilities/tree_isomorphism.hpp
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
install(FILES ${LIBLAL_GENERATION_HEADER}	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lal/generate)
install(FILES ${LIBLAL_GRAPHS_HEADER}		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lal/graphs)
install(FILES ${LIBLAL_INTERNAL_HEADER}		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lal/internal)
install(FILES ${LIBLAL_IO_HEADER}			DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lal/io)
install(FILES ${LIBLAL_ITERATORS_HEADER}	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lal/iterators)
install(FILES ${LIBLAL_LINARR_HEADER}		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lal/linarr)
install(FILES ${LIBLAL_NUMERIC_HEADER}		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lal/numeric)
install(FILES ${LIBLAL_PROPERTIES_HEADER}	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lal/properties)
install(FILES ${LIBLAL_UTILITIES_HEADER}	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lal/utilities)

# **********************************************************************
# COMPILATION FLAGS

# Warning flags
message(STATUS "Adding warning flags")
add_comp_flag(-Wall)
add_comp_flag(-Wextra)
add_comp_flag(-Wshadow)
add_comp_flag(-Wnon-virtual-dtor)
add_comp_flag(-Wold-style-cast)
add_comp_flag(-Wcast-align)
add_comp_flag(-Wunused)
add_comp_flag(-Woverloaded-virtual)
add_comp_flag(-Wpedantic)
add_comp_flag(-Wconversion)
add_comp_flag(-Wsign-conversion)
add_comp_flag(-Wnull-dereference)
add_comp_flag(-Wdouble-promotion)
add_comp_flag(-Wformat=2)
add_comp_flag(-Wduplicated-cond)
add_comp_flag(-Wduplicated-branches)
add_comp_flag(-Wlogical-op)
add_comp_flag(-Wuseless-cast)
add_comp_flag(-Wrestrict)

# OpenMP
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

# Optimisation flags
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	add_debug_flag(-O3)
	add_debug_flag(-DDEBUG)
	add_debug_flag(-D_GLIBCXX_DEBUG)
else()
	# flags added by default:
	#   -O3
	#   -DNDEBUG
	add_release_flag(-UDEBUG)
endif()

# include path (required by the compiler to find the headers)
target_include_directories(${TargetStringLibrary} PRIVATE ..)

target_link_libraries(${TargetStringLibrary} gmp)
