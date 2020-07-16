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

QMAKE_CXXFLAGS_DEBUG += -DDEBUG -D_GLIBCXX_DEBUG
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
	graphs/dgraph.hpp \
	graphs/graph.hpp \
	graphs/output.hpp \
	graphs/rtree.hpp \
	graphs/tree.hpp \
	graphs/ftree.hpp \
	graphs/ugraph.hpp \
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
	numeric/gmp_utils.hpp \
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
	properties/mhd.hpp \
	properties/D_rla.hpp \
	properties/C_rla.hpp \
    utils/graphs/trees/centre_centroid_utils.hpp \
	utils/macros.hpp \
	utils/avl.hpp \
	utils/std_utils.hpp \
	utils/Dmin/common.hpp \
	utils/graphs/cycles.hpp \
	utils/graphs/reachability.hpp \
	utils/graphs/traversal.hpp \
	utils/graphs/trees/convert_to_ftree.hpp \
	utils/graphs/trees/convert_to_rtree.hpp \
	utils/graphs/trees/make_projective_arr.hpp \
	utils/graphs/trees/is_tree.hpp \
	utils/graphs/trees/size_subtrees.hpp \
	utils/graphs/trees/tree_centre.hpp \
	utils/sorting/bit_sort.hpp \
	utils/sorting/counting_sort.hpp \
	utils/sorting/insertion_sort.hpp

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
	graphs/dgraph.cpp \
	graphs/graph.cpp \
	graphs/rtree.cpp \
	graphs/tree.cpp \
	graphs/ftree.cpp \
	graphs/ugraph.cpp \
	iterators/E_iterator.cpp \
	iterators/Q_iterator.cpp \
	linarr/C.cpp \
	linarr/C_approx.cpp \
	linarr/Dmin.cpp \
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
	numeric/gmp_utils.cpp \
	properties/Q.cpp \
	properties/degrees.cpp \
	properties/mhd.cpp \
	properties/variance_C_gen_graphs.cpp \
	properties/variance_C_trees.cpp \
	properties/variance_C_forests.cpp \
	properties/D_rla.cpp \
	properties/C_rla.cpp \
	utils/Dmin/common.cpp \
	utils/graphs/trees/convert_to_ftree.cpp \
	utils/graphs/trees/convert_to_rtree.cpp
