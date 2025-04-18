TEMPLATE = lib

VERSION_MAJOR = 99.99
VERSION_BUILD = 99
VERSION = $${VERSION_MAJOR}.$${VERSION_BUILD}

CONFIG += c++23
CONFIG -= app_bundle
QT -= core gui

CONFIG(release, debug|release) {
TARGET = laloptimized
}
CONFIG(debug, debug|release) {
TARGET = lal
}

isEmpty(ENVIR) {
    ENVIR = "HOME"
}

isEmpty(ADDRESS_SANITIZER) {
    ADDRESS_SANITIZER = "NO"
}

QMAKE_CXXFLAGS = -std=c++23 -fPIC -fopenmp
QMAKE_CXXFLAGS +=			\
	-Wall					\
	-Wextra					\ # reasonable and standard
	-Wshadow				\ # warn if a variable declaration shadows one from
							\ # a parent context
	-Wnon-virtual-dtor		\ # warn if a class with virtual functions has
							\ # non-virtual destructors
	-Wold-style-cast		\ # warn for c-style casts
	-Wcast-align			\ # warn for potential performance problem casts
	-Wunused				\ # warn on anything being unused
	-Woverloaded-virtual	\ # warn if a virtual is overloaded (not overridden)
	-Wpedantic				\ # warn if non-standard C++ is used
	-Wconversion			\ # warn on type conversions that may lose data
	-Wsign-conversion		\ # warn on sign conversions
	-Wnull-dereference		\ # warn if a null dereference is detected
	-Wdouble-promotion		\ # warn if float is implicitly promoted to double
	-Wformat=2				\ # warn on security issues around functions that
							\ # format output
	-Wduplicated-cond		\ # warn if if-then-else chan has duplicated conditions
	-Wduplicated-branches	\ # warn if if-then-else have duplicated code
	-Wlogical-op			\ # warn about logical operations being used where
							\ # bitwise were probably prefered
	-Wuseless-cast			\ # warn if you perform a cast to the same type
	-Wrestrict

QMAKE_CXXFLAGS_DEBUG += -DDEBUG -D_GLIBCXX_DEBUG -Og
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -UDEBUG -DNDEBUG -fstrict-aliasing -O3

QMAKE_LFLAGS += -fPIC -O3 -Wl,-O3
QMAKE_LFLAGS_RELEASE += -DNDEBUG -UDEBUG
QMAKE_LFLAGS_DEBUG += -DDEBUG -D_GLIBCXX_DEBUG

equals(ADDRESS_SANITIZER, "YES") {
    # not to be used in combination with valgrind
	QMAKE_CXXFLAGS += -fsanitize=address
	LIBS += -lasan
}

# libraries
LIBS += -lgmp -fopenmp -lpthread

# configure cluster compiler
equals(ENVIR, "CLUSTER") {
	QMAKE_CXX = /home/soft/gcc-11.4.0/bin/g++
	QMAKE_LINK = /home/soft/gcc-11.4.0/bin/g++
}

INCLUDEPATH += ..

# lal/detail/avl.hpp
#DEFINES += LAL_DEBUG_AVL

# lal/detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/*.cpp *.hpp
# lal/linarr/D/DMax_unconstrained_all.cpp
#DEFINES += LAL_DEBUG_DMax_Unc_BnB

# lal/detail/linarr/D/DMax/1_eq_thistle_AEF.hpp
#DEFINES += LAL_DEBUG_DMax_1_thistle

DEFINES += LAL_REGISTER_BIBLIOGRAPHY

# -------------
# PROJECT FILES
DISTFILES += \
	.clang-format

HEADERS += \
	basic_types.hpp \
	bibliography.hpp \
	bibliography_entries.hpp \
	detail/arrangement_wrapper.hpp \
	detail/array.hpp \
	detail/avl.hpp \
	detail/bibliography.hpp \
	detail/generate/make_arrangement.hpp \
	detail/linarr/C/brute_force.hpp \
	detail/linarr/C/crossings.hpp \
	detail/linarr/C/dyn_prog.hpp \
	detail/linarr/C/ladder.hpp \
	detail/linarr/C/predict.hpp \
	detail/linarr/C/stack_based.hpp \
	detail/linarr/D/D.hpp \
	detail/linarr/D/DMax/1_eq_thistle_AEF.hpp \
	detail/linarr/D/DMax/Bipartite_AEF.hpp \
	detail/linarr/D/DMax/Planar_AEF.hpp \
	detail/linarr/D/DMax/Projective_AEF.hpp \
	detail/linarr/D/DMax/necessary_conditions.hpp \
	detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/BnB.hpp \
	detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/level_value_propagation_origin.hpp \
	detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/next_action.hpp \
	detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/propagation_result.hpp \
	detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/reason_discard.hpp \
	detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/set_maximum_arrangements.hpp \
	detail/linarr/D/DMax/utils.hpp \
	detail/linarr/D/Dmin/Bipartite_AEF.hpp \
	detail/linarr/D/Dmin/Planar_AEF.hpp \
	detail/linarr/D/Dmin/Planar_HS.hpp \
	detail/linarr/D/Dmin/Projective_AEF.hpp \
	detail/linarr/D/Dmin/Projective_HS.hpp \
	detail/linarr/D/Dmin/Unconstrained_FC.hpp \
	detail/linarr/D/Dmin/Unconstrained_YS.hpp \
	detail/linarr/D/Dmin/utils.hpp \
	detail/linarr/D/Dopt_utils.hpp \
	detail/linarr/D/bipartite_opt_utils.hpp \
	detail/linarr/chunking/Anderson.hpp \
	detail/linarr/chunking/Macutek.hpp \
	detail/linarr/chunking/generic.hpp \
	detail/linarr/dependency_flux.hpp \
	detail/linarr/formal_constraints.hpp \
	detail/linarr/headedness.hpp \
	detail/linarr/level_signature.hpp \
	detail/macros/basic_convert.hpp \
	detail/macros/search.hpp \
	detail/properties/bipartite_graph_colorability.hpp \
	detail/properties/branchless_paths_compute.hpp \
	detail/properties/connected_components_compute.hpp \
	detail/properties/tree_maximum_caterpillar.hpp \
	detail/queue_array.hpp \
	detail/set_array.hpp \
	detail/type_traits/bool_sequence.hpp \
	detail/type_traits/conditional_list.hpp \
	detail/type_traits/first_true.hpp \
	detail/type_traits/is_pointer_iterator.hpp \
	detail/type_traits/ith_type.hpp \
	detail/make_array.hpp \
	detail/graphs/conversions.hpp \
	detail/graphs/enumerate_sets.hpp \
	detail/graphs/is_tree.hpp \
	detail/graphs/retrieve_subtrees.hpp \
	detail/graphs/size_subtrees.hpp \
	detail/graphs/tree_classification.hpp \
	detail/graphs/tree_type.hpp \
	detail/io/check_correctness.hpp \
	detail/linarr/syntactic_dependency_tree.hpp \
	detail/numeric/utils.hpp \
	detail/pairs_utils.hpp \
	detail/properties/tree_centre.hpp \
	detail/properties/tree_centroid.hpp \
	detail/properties/tree_diameter.hpp \
	detail/sorting/bit_sort.hpp \
	detail/sorting/counting_sort.hpp \
	detail/sorting/insertion_sort.hpp \
	detail/graphs/union_find.hpp \
	detail/graphs/utils.hpp \
	detail/graphs/cycles.hpp \
	detail/graphs/reachability.hpp \
	detail/graphs/traversal.hpp \
	detail/sorting/sorted_vector.hpp \
	detail/sorting/sorting_types.hpp \
	detail/type_traits/type_sequence.hpp \
	detail/utilities/tree_isomorphism.hpp \
	generate.hpp \
	generate/all_arrangements.hpp \
	generate/all_bipartite_arrangements.hpp \
	generate/all_lab_free_trees.hpp \
	generate/all_planar_arrangements.hpp \
	generate/all_ulab_free_bistar_trees.hpp \
	generate/all_ulab_free_trees.hpp \
	generate/all_ulab_rooted_trees.hpp \
	generate/all_lab_rooted_trees.hpp \
	generate/all_projective_arrangements.hpp \
	generate/rand_arrangements.hpp \
	generate/rand_bipartite_arrangements.hpp \
	generate/rand_lab_free_trees.hpp \
	generate/rand_lab_rooted_trees.hpp \
	generate/rand_planar_arrangements.hpp \
	generate/rand_ulab_free_trees.hpp \
	generate/rand_ulab_rooted_trees.hpp \
	generate/rand_projective_arrangements.hpp \
	generate/tree_generator.hpp \
	generate/tree_generator_type.hpp \
	graphs.hpp \
	graphs/conversions.hpp \
	graphs/graph.hpp \
	graphs/output.hpp \
	graphs/tree.hpp \
	graphs/directed_graph.hpp \
	graphs/free_tree.hpp \
	graphs/rooted_tree.hpp \
	graphs/tree_literals.hpp \
	graphs/tree_type.hpp \
	graphs/undirected_graph.hpp \
	io/check_correctness.hpp \
	io/edge_list.hpp \
	io.hpp \
	io/basic_output.hpp \
	io/head_vector.hpp \
	io/head_vector_error.hpp \
	io/head_vector_error_type.hpp \
	io/treebank_collection_processor.hpp \
	io/treebank_collection_reader.hpp \
	io/treebank_collection_report.hpp \
	io/treebank_collection_report_location.hpp \
	io/treebank_feature_type.hpp \
	io/treebank_file_error.hpp \
	io/treebank_file_error_type.hpp \
	io/treebank_file_report.hpp \
	io/treebank_processor.hpp \
	io/treebank_processor_base.hpp \
	io/treebank_reader.hpp \
	iterators.hpp \
	iterators/E_iterator.hpp \
	iterators/Q_iterator.hpp \
	linarr.hpp \
	C.hpp \
	D.hpp \
	linarr/C/C.hpp \
	linarr/C/algorithms_C.hpp \
	linarr/D/D.hpp \
	linarr/D/DMax.hpp \
	linarr/D/Dmin.hpp \
	linarr/D/algorithms_Dmin.hpp \
	linarr/D/algorithms_Dmin_planar.hpp \
	linarr/D/algorithms_Dmin_projective.hpp \
	linarr/aggregations/1level.hpp \
	linarr/aggregations/2level.hpp \
	linarr/chunking/algorithms.hpp \
	linarr/chunking/chunk.hpp \
	linarr/chunking/chunk_sequence.hpp \
	linarr/chunking/chunking.hpp \
	linarr/chunking/output.hpp \
	linarr/dependency_flux.hpp \
	linarr/dependency_flux_compute.hpp \
	linarr/formal_constraints.hpp \
	linarr/head_initial.hpp \
	linarr/syntactic_dependency_tree/classify.hpp \
	linarr/syntactic_dependency_tree/type.hpp \
	linear_arrangement.hpp \
	numeric.hpp \
	numeric/integer.hpp \
	numeric/integer_output.hpp \
	numeric/output.hpp \
	numeric/rational.hpp \
	numeric/rational_output.hpp \
	properties.hpp \
	properties/Q.hpp \
	properties/D_rla.hpp \
	properties/C_rla.hpp \
	properties/bipartite_graph_colorability.hpp \
	properties/bipartite_graph_coloring.hpp \
	properties/branchless_path.hpp \
	properties/branchless_paths_compute.hpp \
	properties/connected_components.hpp \
	properties/connected_components_compute.hpp \
	properties/degrees.hpp \
	properties/hierarchical_distance.hpp \
	properties/maximum_spanning_trees.hpp \
	properties/tree_centre.hpp \
	properties/tree_centroid.hpp \
	properties/tree_diameter.hpp \
	properties/vertex_orbits.hpp \
	utilities.hpp \
	utilities/aggregations.hpp \
	utilities/tree_isomorphism.hpp \
	lal_version.hpp

SOURCES += \
	bibliography.cpp \
	detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/BnB_constraints.cpp \
	detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/BnB_execution.cpp \
	detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/BnB_execution_independent_set.cpp \
	detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/BnB_initialisation.cpp \
	detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/BnB_next_action.cpp \
	detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/BnB_propagate.cpp \
	detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/BnB_propagate__antenna.cpp \
	detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/BnB_propagate__bridge.cpp \
	detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/BnB_roll_back.cpp \
	detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/BnB_roll_back__antenna.cpp \
	detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/BnB_roll_back__bridge.cpp \
	detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/BnB_state_manipulation.cpp \
	detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/BnB_utils.cpp \
	generate/all_arrangements.cpp \
	generate/all_bipartite_arrangements.cpp \
	generate/all_lab_free_trees.cpp \
	generate/all_planar_arrangements.cpp \
	generate/all_projective_arrangements.cpp \
	generate/all_ulab_free_bistar_trees.cpp \
	generate/all_ulab_free_trees.cpp \
	generate/all_ulab_rooted_trees.cpp \
	generate/rand_arrangements.cpp \
	generate/rand_bipartite_arrangements.cpp \
	generate/rand_lab_free_trees.cpp \
	generate/rand_planar_arrangements.cpp \
	generate/rand_projective_arrangements.cpp \
	generate/rand_ulab_free_trees.cpp \
	generate/rand_ulab_rooted_trees.cpp \
	graphs/conversions.cpp \
	graphs/directed_graph.cpp \
	graphs/free_tree.cpp \
	graphs/graph.cpp \
	graphs/rooted_tree.cpp \
	graphs/tree.cpp \
	graphs/tree_literals.cpp \
	graphs/undirected_graph.cpp \
	io/check_correctness.cpp \
	io/edge_list.cpp \
	io/head_vector.cpp \
	io/treebank_collection_processor.cpp \
	io/treebank_collection_reader.cpp \
	io/treebank_processor.cpp \
	io/treebank_reader.cpp \
	linarr/C/C.cpp \
	linarr/C/predict.cpp \
	linarr/D/D.cpp \
	linarr/D/DMax.cpp \
	linarr/D/DMax_unconstrained_all.cpp \
	linarr/D/Dmin.cpp \
	linarr/chunking/chunking.cpp \
	linarr/dependency_flux.cpp \
	linarr/formal_constraints.cpp \
	linarr/head_initial.cpp \
	linarr/syntactic_dependency_tree/classify.cpp \
	numeric/integer.cpp \
	numeric/rational.cpp \
	properties/Q.cpp \
	properties/bipartite_graph_colorability.cpp \
	properties/branchless_paths_compute.cpp \
	properties/connected_components_compute.cpp \
	properties/degrees.cpp \
	properties/hierarchical_distance.cpp \
	properties/maximum_spanning_trees.cpp \
	properties/tree_centre.cpp \
	properties/tree_centroid.cpp \
	properties/tree_diameter.cpp \
	properties/variance_C_gen_graphs.cpp \
	properties/variance_C_forests.cpp \
	properties/D_rla.cpp \
	properties/C_rla.cpp \
	properties/vertex_orbits.cpp \
	utilities/tree_isomorphism.cpp
