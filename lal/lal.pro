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

QMAKE_CXXFLAGS_DEBUG += -g -DDEBUG -D_GLIBCXX_DEBUG
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
    linarr/algorithms_Dmin.hpp \
    linarr/tree_structure.hpp \
    linarr/tree_structure_class.hpp \
    namespaces.hpp \
    definitions.hpp \
    generate.hpp \
    generate/all_lab_free_trees.hpp \
    generate/all_ulab_free_trees.hpp \
    generate/all_ulab_rooted_trees.hpp \
    generate/rand_lab_free_trees.hpp \
    generate/rand_lab_rooted_trees.hpp \
    generate/rand_ulab_free_trees.hpp \
    generate/rand_ulab_rooted_trees.hpp \
    graphs.hpp \
    graphs/dgraph.hpp \
    graphs/drtree.hpp \
    graphs/dtree.hpp \
    graphs/graph.hpp \
    graphs/output.hpp \
    graphs/rtree.hpp \
    graphs/tree.hpp \
    graphs/ugraph.hpp \
    graphs/urtree.hpp \
    graphs/utree.hpp \
    io/basic_output.hpp \
    iterators.hpp \
    iterators/E_iterator.hpp \
    iterators/Q_iterator.hpp \
    linarr.hpp \
    linarr/1level.hpp \
    linarr/1level_impl.hpp \
    linarr/2level.hpp \
    linarr/2level_impl.hpp \
    linarr/algorithms_crossings.hpp \
    linarr/headedness.hpp \
    io/dataset_error.hpp \
    io/treebank_dataset.hpp \
    numeric.hpp \
    numeric/integer.hpp \
    numeric/output.hpp \
    numeric/rational.hpp \
    numeric/gmp_utils.hpp \
    io/edge_list.hpp \
    properties.hpp \
    properties/degrees.hpp \
    properties/Q.hpp \
    linarr/C.hpp \
    linarr/D.hpp \
    io/treebank_processor.hpp \
    io/treebank_reader.hpp \
    properties/D_rla.hpp \
    properties/C_rla.hpp \
    C.hpp \
    D.hpp \
    properties/mhd.hpp \
    utils/graphs/bfs.hpp \
    utils/graphs/cycles.hpp \
    utils/graphs/reachability.hpp \
    utils/graphs/trees/conversions.hpp \
    utils/macros.hpp \
    utils/avl.hpp \
    io.hpp \
    utils/graphs/trees/is_tree.hpp \
    utils/graphs/trees/size_subtrees.hpp \
    utils/graphs/trees/tree_centre.hpp \
    utils/sorting/bit_sort.hpp \
    utils/sorting/counting_sort.hpp \
    utils/sorting/insertion_sort.hpp \
    utils/std_utils.hpp

SOURCES += \
    generate/all_lab_free_trees.cpp \
    generate/all_ulab_free_trees.cpp \
    generate/all_ulab_rooted_trees.cpp \
    generate/rand_lab_free_trees.cpp \
    generate/rand_lab_rooted_trees.cpp \
    generate/rand_ulab_free_trees.cpp \
    generate/rand_ulab_rooted_trees.cpp \
    graphs/dgraph.cpp \
    graphs/drtree.cpp \
    graphs/dtree.cpp \
    graphs/graph.cpp \
    graphs/rtree.cpp \
    graphs/tree.cpp \
    graphs/ugraph.cpp \
    graphs/urtree.cpp \
    graphs/utree.cpp \
    iterators/E_iterator.cpp \
    iterators/Q_iterator.cpp \
    linarr/C.cpp \
    linarr/C_approx.cpp \
    linarr/Dmin.cpp \
    linarr/headedness.cpp \
    io/treebank_dataset.cpp \
    linarr/C_stack_based.cpp \
    linarr/tree_structure_class.cpp \
    numeric/integer.cpp \
    numeric/rational.cpp \
    numeric/gmp_utils.cpp \
    io/edge_list.cpp \
    properties/Q.cpp \
    linarr/D.cpp \
    properties/degrees.cpp \
    properties/mhd.cpp \
    linarr/C_brute_force.cpp \
    linarr/C_dyn_prog.cpp \
    linarr/C_ladder.cpp \
    properties/variance_C_gen_graphs.cpp \
    properties/variance_C_trees.cpp \
    io/treebank_processor.cpp \
    io/treebank_reader.cpp \
    properties/variance_C_forests.cpp \
    properties/D_rla.cpp \
    properties/C_rla.cpp \
    utils/graphs/trees/conversions.cpp
