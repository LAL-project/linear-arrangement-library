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

# **********************************************************************
# Custom functions
function(add_compiler_flag flag)
	check_cxx_compiler_flag(${flag} CXX_HAS_FLAG_${flag})
	
	if (CXX_HAS_FLAG_${flag})
		target_compile_options(${TargetStringLibrary} PRIVATE ${flag})
	endif()
endfunction(add_compiler_flag)

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

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	message(STATUS "Target library lal")
	set(TargetStringLibrary "lal")
else()
	message(STATUS "Target library laloptimized")
	set(TargetStringLibrary "laloptimized")
endif()

# **********************************************************************
# **********************************************************************

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

# **********************************************************************
# SOURCE FILES FOR COMPILATION

# source files of the library
target_sources(
	${TargetStringLibrary}
	PRIVATE
		# detail namespace -- not to be installed (except for a few exceptions)
		# -- headers
		#      generate
		detail/generate/make_arrangement.hpp
		#      graphs
		detail/graphs/conversions.hpp
		detail/graphs/cycles.hpp
		detail/graphs/enumerate_sets.hpp
		detail/graphs/is_tree.hpp
		detail/graphs/reachability.hpp
		detail/graphs/retrieve_subtrees.hpp
		detail/graphs/size_subtrees.hpp
		detail/graphs/traversal.hpp
		detail/graphs/tree_classification.hpp
		detail/graphs/tree_type.hpp
		detail/graphs/union_find.hpp
		detail/graphs/utils.hpp
		#      io
		detail/io/check_correctness.hpp
		#      iterators
		#          nothing
		#      linarr
		detail/linarr/C/brute_force.hpp
		detail/linarr/C/crossings.hpp
		detail/linarr/C/dyn_prog.hpp
		detail/linarr/C/ladder.hpp
		detail/linarr/C/predict.hpp
		detail/linarr/C/stack_based.hpp
		detail/linarr/chunking/Anderson.hpp
		detail/linarr/chunking/Macutek.hpp
		detail/linarr/D/DMax/1_eq_thistle_AEF.hpp
		detail/linarr/D/DMax/Bipartite_AEF.hpp
		detail/linarr/D/DMax/necessary_conditions.hpp
		detail/linarr/D/DMax/Planar_AEF.hpp
		detail/linarr/D/DMax/Projective_AEF.hpp
		detail/linarr/D/DMax/utils.hpp
		detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/BnB.hpp
		detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/set_maximum_arrangements.hpp
		detail/linarr/D/Dmin/Bipartite_AEF.hpp
		detail/linarr/D/Dmin/Planar_AEF.hpp
		detail/linarr/D/Dmin/Planar_HS.hpp
		detail/linarr/D/Dmin/Projective_AEF.hpp
		detail/linarr/D/Dmin/Projective_HS.hpp
		detail/linarr/D/Dmin/Unconstrained_FC.hpp
		detail/linarr/D/Dmin/Unconstrained_YS.hpp
		detail/linarr/D/Dmin/utils.hpp
		detail/linarr/D/bipartite_opt_utils.hpp
		detail/linarr/D/D.hpp
		detail/linarr/D/Dopt_utils.hpp
		detail/linarr/dependency_flux.hpp
		detail/linarr/formal_constraints.hpp
		detail/linarr/headedness.hpp
		detail/linarr/level_signature.hpp
		detail/linarr/syntactic_dependency_tree.hpp
		#      macros
		detail/macros/basic_convert.hpp
		#      numeric
		detail/numeric/utils.hpp
		#      properties
		detail/properties/bipartite_graph_colorability.hpp
		detail/properties/branchless_path_find.hpp
		detail/properties/tree_centre.hpp
		detail/properties/tree_centroid.hpp
		detail/properties/tree_diameter.hpp
		detail/properties/tree_maximum_caterpillar.hpp
		#      sorting
		detail/sorting/bit_sort.hpp
		detail/sorting/counting_sort.hpp
		detail/sorting/insertion_sort.hpp
		detail/sorting/sorted_vector.hpp
		detail/sorting/sorting_types.hpp
		#      type_traits
		detail/type_traits/bool_sequence.hpp
		detail/type_traits/conditional_list.hpp
		detail/type_traits/first_true.hpp
		detail/type_traits/is_pointer_iterator.hpp
		detail/type_traits/ith_type.hpp
		detail/type_traits/type_sequence.hpp
		#      utilities
		detail/utilities/tree_isomorphism.hpp
		#      others
		detail/arrangement_wrapper.hpp
		detail/avl.hpp
		detail/data_array.hpp
		detail/linear_queue.hpp
		detail/linear_set.hpp
		detail/make_array.hpp
		detail/pairs_utils.hpp
		
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
		detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/BnB_constraints.cpp
		detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/BnB_execution_independent_set.cpp
		detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/BnB_execution.cpp
		detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/BnB_initialisation.cpp
		detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/BnB_next_action.cpp
		detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/BnB_state_manipulation.cpp
		detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/BnB_utils.cpp
		#      numeric
		#          nothing
		#      properties
		#          nothing
		#      sorting
		#          nothing
		#      utilities
		#          nothing
		#      others
		#          nothing
		
		# generate namespace
		# -- headers
		generate/all_arrangements.hpp
		generate/all_bipartite_arrangements.hpp
		generate/all_lab_free_trees.hpp
		generate/all_lab_rooted_trees.hpp
		generate/all_planar_arrangements.hpp
		generate/all_projective_arrangements.hpp
		generate/all_ulab_free_trees.hpp
		generate/all_ulab_rooted_trees.hpp
		generate/rand_arrangements.hpp
		generate/rand_bipartite_arrangements.hpp
		generate/rand_lab_free_trees.hpp
		generate/rand_lab_rooted_trees.hpp
		generate/rand_planar_arrangements.hpp
		generate/rand_projective_arrangements.hpp
		generate/rand_ulab_free_trees.hpp
		generate/rand_ulab_rooted_trees.hpp
		generate/tree_generator.hpp
		generate/tree_generator_type.hpp
		# -- sources
		generate/all_arrangements.cpp
		generate/all_bipartite_arrangements.cpp
		generate/all_lab_free_trees.cpp
		generate/all_planar_arrangements.cpp
		generate/all_projective_arrangements.cpp
		generate/all_ulab_free_trees.cpp
		generate/all_ulab_rooted_trees.cpp
		generate/rand_arrangements.cpp
		generate/rand_bipartite_arrangements.cpp
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
		graphs/tree_literals.hpp
		graphs/tree_type.hpp
		graphs/undirected_graph.hpp
		# -- sources
		graphs/conversions.cpp
		graphs/directed_graph.cpp
		graphs/free_tree.cpp
		graphs/graph.cpp
		graphs/rooted_tree.cpp
		graphs/tree.cpp
		graphs/tree_literals.cpp
		graphs/undirected_graph.cpp

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
		linarr/aggregations/1level.hpp
		linarr/aggregations/2level.hpp
		linarr/C/algorithms_C.hpp
		linarr/C/C.hpp
		linarr/chunking/algorithms.hpp
		linarr/chunking/chunk.hpp
		linarr/chunking/chunking.hpp
		linarr/chunking/output.hpp
		linarr/D/algorithms_Dmin.hpp
		linarr/D/algorithms_Dmin_planar.hpp
		linarr/D/algorithms_Dmin_projective.hpp
		linarr/D/D.hpp
		linarr/D/DMax.hpp
		linarr/D/Dmin.hpp
		linarr/syntactic_dependency_tree/classify.hpp
		linarr/syntactic_dependency_tree/type.hpp
		linarr/dependency_flux.hpp
		linarr/formal_constraints.hpp
		linarr/head_initial.hpp
		# -- sources
		linarr/C/C.cpp
		linarr/C/predict.cpp
		linarr/chunking/chunking.cpp
		linarr/D/D.cpp
		linarr/D/DMax.cpp
		linarr/D/DMax_unconstrained_all.cpp
		linarr/D/Dmin.cpp
		linarr/syntactic_dependency_tree/classify.cpp
		linarr/dependency_flux.cpp
		linarr/formal_constraints.cpp
		linarr/head_initial.cpp

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
		properties/bipartite_graph_colorability.hpp
		properties/bipartite_graph_coloring.hpp
		properties/branchless_path_find.hpp
		properties/branchless_path.hpp
		properties/C_rla.hpp
		properties/D_rla.hpp
		properties/degrees.hpp
		properties/hierarchical_distance.hpp
		properties/maximum_spanning_trees.hpp
		properties/Q.hpp
		properties/tree_centre.hpp
		properties/tree_centroid.hpp
		properties/tree_diameter.hpp
		properties/vertex_orbits.hpp
		# -- sources
		properties/bipartite_graph_colorability.cpp
		properties/branchless_path_find.cpp
		properties/C_rla.cpp
		properties/D_rla.cpp
		properties/hierarchical_distance.cpp
		properties/maximum_spanning_trees.cpp
		properties/Q.cpp
		properties/tree_centre.cpp
		properties/tree_centroid.cpp
		properties/tree_diameter.cpp
		properties/variance_C_forests.cpp
		properties/variance_C_gen_graphs.cpp
		properties/vertex_orbits.cpp
		
		# utilities namespace
		# -- headers
		utilities/aggregations.hpp
		utilities/tree_isomorphism.hpp
		# -- sources
		utilities/tree_isomorphism.cpp

		# lal namespace
		# -- heaers
		basic_types.hpp
		C.hpp
		D.hpp
		generate.hpp
		graphs.hpp
		io.hpp
		iterators.hpp
		lal_version.hpp
		linarr.hpp
		linear_arrangement.hpp
		numeric.hpp
		properties.hpp
		utilities.hpp
)

# ******************************************************************** #
# PUBLIC HEADER FILES FOR INSTALLATION

# Public header files of the library (to be installed in the system).
# These are not installed through the usual PUBLIC_HEADER property,
# but via several custom variables

# -------------------------------------------------------------------- #
set(LIBLAL_MAIN_HEADER
	basic_types.hpp
	C.hpp
	D.hpp
	generate.hpp
	graphs.hpp
	io.hpp
	iterators.hpp
	lal_version.hpp
	linarr.hpp
	linear_arrangement.hpp
	numeric.hpp
	properties.hpp
	utilities.hpp
)
# -------------------------------------------------------------------- #
set(LIBLAL_DETAIL_HEADER
	detail/data_array.hpp
	detail/arrangement_wrapper.hpp
)
# -------------------------------------------------------------------- #
set(LIBLAL_GENERATION_HEADER
	generate/all_arrangements.hpp
	generate/all_bipartite_arrangements.hpp
	generate/all_lab_free_trees.hpp
	generate/all_lab_rooted_trees.hpp
	generate/all_planar_arrangements.hpp
	generate/all_projective_arrangements.hpp
	generate/all_ulab_free_trees.hpp
	generate/all_ulab_rooted_trees.hpp
	generate/rand_arrangements.hpp
	generate/rand_bipartite_arrangements.hpp
	generate/rand_lab_free_trees.hpp
	generate/rand_lab_rooted_trees.hpp
	generate/rand_planar_arrangements.hpp
	generate/rand_projective_arrangements.hpp
	generate/rand_ulab_free_trees.hpp
	generate/rand_ulab_rooted_trees.hpp
	generate/tree_generator.hpp
	generate/tree_generator_type.hpp
)
# -------------------------------------------------------------------- #
set(LIBLAL_GRAPHS_HEADER
	graphs/conversions.hpp
	graphs/directed_graph.hpp
	graphs/free_tree.hpp
	graphs/graph.hpp
	graphs/output.hpp
	graphs/rooted_tree.hpp
	graphs/tree.hpp
	graphs/tree_literals.hpp
	graphs/tree_type.hpp
	graphs/undirected_graph.hpp
)
# -------------------------------------------------------------------- #
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
# -------------------------------------------------------------------- #
set(LIBLAL_ITERATORS_HEADER
	iterators/E_iterator.hpp
	iterators/Q_iterator.hpp
)
# -------------------------------------------------------------------- #
set(LIBLAL_LINARR_MAIN_HEADER
	linarr/dependency_flux.hpp
	linarr/formal_constraints.hpp
	linarr/head_initial.hpp
)
set(LIBLAL_LINARR_aggregations_HEADER
	linarr/aggregations/1level.hpp
	linarr/aggregations/2level.hpp
)
set(LIBLAL_LINARR_C_HEADER
	linarr/C/algorithms_C.hpp
	linarr/C/C.hpp
)
set(LIBLAL_LINARR_chunking_HEADER
	linarr/chunking/algorithms.hpp
	linarr/chunking/chunk.hpp
	linarr/chunking/chunking.hpp
	linarr/chunking/output.hpp
)
set(LIBLAL_LINARR_D_HEADER
	linarr/D/algorithms_Dmin.hpp
	linarr/D/algorithms_Dmin_planar.hpp
	linarr/D/algorithms_Dmin_projective.hpp
	linarr/D/D.hpp
	linarr/D/DMax.hpp
	linarr/D/Dmin.hpp
)
set(LIBLAL_LINARR_syndeptree_HEADER
	linarr/syntactic_dependency_tree/classify.hpp
	linarr/syntactic_dependency_tree/type.hpp
)
# -------------------------------------------------------------------- #
set(LIBLAL_NUMERIC_HEADER
	numeric/integer.hpp
	numeric/integer_output.hpp
	numeric/output.hpp
	numeric/rational.hpp
	numeric/rational_output.hpp
)
# -------------------------------------------------------------------- #
set(LIBLAL_PROPERTIES_HEADER
	properties/bipartite_graph_colorability.hpp
	properties/bipartite_graph_coloring.hpp
	properties/branchless_path_find.hpp
	properties/branchless_path.hpp
	properties/C_rla.hpp
	properties/D_rla.hpp
	properties/degrees.hpp
	properties/hierarchical_distance.hpp
	properties/maximum_spanning_trees.hpp
	properties/Q.hpp
	properties/tree_centre.hpp
	properties/tree_centroid.hpp
	properties/tree_diameter.hpp
)
# -------------------------------------------------------------------- #
set(LIBLAL_UTILITIES_HEADER
	utilities/aggregations.hpp
	utilities/tree_isomorphism.hpp
)
# **********************************************************************

# **********************************************************************
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

# **********************************************************************
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
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

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

target_link_libraries(${TargetStringLibrary} gmp)
target_link_libraries(${TargetStringLibrary} pthread)
