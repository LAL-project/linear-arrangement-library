TEMPLATE = lib

CONFIG += c++1z static
CONFIG -= app_bundle
QT     -= core gui

CONFIG(release, debug|release) {
TARGET = lal
}
CONFIG(debug, debug|release) {
TARGET = laldebug
}

QMAKE_CXXFLAGS_DEBUG += -O0 -DDEBUG -D_GLIBCXX_DEBUG
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -DNDEBUG -fstrict-aliasing

QMAKE_CXXFLAGS +=										\
    -Wpedantic -Wshadow -Wall -Wextra -Wconversion		\
    -Wold-style-cast -Wrestrict -Wduplicated-cond		\
    -Wnon-virtual-dtor -Woverloaded-virtual

QMAKE_CXXFLAGS += -fopenmp

INCLUDEPATH += ..

# -------------
# PROJECT FILES
HEADERS += \
    graphs/directed_graph.hpp \
    graphs/free_tree.hpp \
    graphs/rooted_tree.hpp \
	graphs/undirected_graph.hpp \
    lal_version.hpp \
	namespaces.hpp \
	definitions.hpp \
	generate.hpp \
	generate/all_lab_free_trees.hpp \
	generate/all_ulab_free_trees.hpp \
	generate/all_ulab_rooted_trees.hpp \
	generate/all_lab_rooted_trees.hpp \
	generate/all_projective_arrangements.hpp \
	generate/rand_lab_free_trees.hpp \
	generate/rand_lab_rooted_trees.hpp \
	generate/rand_ulab_free_trees.hpp \
	generate/rand_ulab_rooted_trees.hpp \
	generate/rand_projective_arrangements.hpp \
	graphs.hpp \
	graphs/graph.hpp \
	graphs/output.hpp \
	graphs/tree.hpp \
	iterators.hpp \
	iterators/E_iterator.hpp \
	iterators/Q_iterator.hpp \
	linarr.hpp \
	linarr/Dmin.hpp \
	linarr/algorithms_C.hpp \
	linarr/algorithms_Dmin.hpp \
	linarr/tree_structure.hpp \
	linarr/tree_structure_class.hpp \
	linarr/1level.hpp \
	linarr/1level_impl.hpp \
	linarr/2level.hpp \
	linarr/2level_impl.hpp \
	linarr/headedness.hpp \
	linarr/C.hpp \
	linarr/D.hpp \
	numeric.hpp \
	numeric/integer.hpp \
	numeric/output.hpp \
	numeric/rational.hpp \
	io/edge_list.hpp \
	io.hpp \
	io/basic_output.hpp \
	io/dataset_error.hpp \
	io/treebank_dataset.hpp \
	io/treebank_processor.hpp \
	io/treebank_reader.hpp \
	C.hpp \
	D.hpp \
	properties.hpp \
	properties/degrees.hpp \
	properties/Q.hpp \
	properties/D_rla.hpp \
	properties/C_rla.hpp \
	internal/gmp/utils.hpp \
	internal/graphs/trees/centre_centroid_utils.hpp \
	internal/graphs/utils.hpp \
	internal/macros.hpp \
	internal/avl.hpp \
	internal/std_utils.hpp \
	internal/graphs/cycles.hpp \
	internal/graphs/reachability.hpp \
	internal/graphs/traversal.hpp \
	internal/graphs/trees/convert_to_ftree.hpp \
	internal/graphs/trees/convert_to_rtree.hpp \
	internal/graphs/trees/make_projective_arr.hpp \
	internal/graphs/trees/is_tree.hpp \
	internal/graphs/trees/size_subtrees.hpp \
	internal/graphs/trees/tree_centre.hpp \
	internal/graphs/trees/tree_centroid.hpp \
	internal/sorting/bit_sort.hpp \
	internal/sorting/counting_sort.hpp \
	internal/sorting/insertion_sort.hpp \
    properties/mean_hierarchical_distance.hpp

SOURCES += \
    generate/all_lab_free_trees.cpp \
	generate/all_lab_rooted_trees.cpp \
	generate/all_projective_arrangements.cpp \
	generate/all_ulab_free_trees.cpp \
	generate/all_ulab_rooted_trees.cpp \
	generate/rand_lab_free_trees.cpp \
	generate/rand_lab_rooted_trees.cpp \
	generate/rand_projective_arrangements.cpp \
	generate/rand_ulab_free_trees.cpp \
	generate/rand_ulab_rooted_trees.cpp \
    graphs/directed_graph.cpp \
    graphs/free_tree.cpp \
	graphs/graph.cpp \
    graphs/rooted_tree.cpp \
	graphs/tree.cpp \
	graphs/undirected_graph.cpp \
	iterators/E_iterator.cpp \
	iterators/Q_iterator.cpp \
	linarr/C.cpp \
	linarr/C_approx.cpp \
	linarr/Dmin.cpp \
    linarr/Dmin_Planar.cpp \
	linarr/Dmin_Projective.cpp \
	linarr/Dmin_Unconstrained_FC.cpp \
	linarr/Dmin_Unconstrained_YS.cpp \
	linarr/headedness.cpp \
	linarr/C_stack_based.cpp \
	linarr/tree_structure_class.cpp \
	linarr/D.cpp \
	linarr/C_brute_force.cpp \
	linarr/C_dyn_prog.cpp \
	linarr/C_ladder.cpp \
	io/treebank_dataset.cpp \
	io/edge_list.cpp \
	io/treebank_processor.cpp \
	io/treebank_reader.cpp \
	numeric/integer.cpp \
	numeric/rational.cpp \
	properties/Q.cpp \
	properties/degrees.cpp \
    properties/mean_hierarchical_distance.cpp \
	properties/variance_C_gen_graphs.cpp \
	properties/variance_C_trees.cpp \
	properties/variance_C_forests.cpp \
	properties/D_rla.cpp \
	properties/C_rla.cpp \
	internal/gmp/utils.cpp \
	internal/graphs/trees/convert_to_ftree.cpp \
	internal/graphs/trees/convert_to_rtree.cpp
