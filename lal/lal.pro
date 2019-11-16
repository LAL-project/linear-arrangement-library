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

QMAKE_CXXFLAGS_RELEASE -= -O2

QMAKE_CXXFLAGS_DEBUG += -DDEBUG -D_GLIBCXX_DEBUG
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
    definitions.hpp \
    generation.hpp \
    generation/rand_rooted_lab_dir_trees.hpp \
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
    iterators/Q_iterator.hpp \
    iterators/edge_iterator.hpp \
    linarr.hpp \
    linarr/1level.hpp \
    linarr/1level_impl.hpp \
    linarr/2level.hpp \
    linarr/2level_impl.hpp \
    linarr/algorithms_crossings.hpp \
    linarr/classification.hpp \
    linarr/headedness.hpp \
    io/dataset_error.hpp \
    io/treebank_dataset.hpp \
    linarr/tree_structure_type.hpp \
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
    generation/free_ulab_trees.hpp \
    generation/free_lab_trees.hpp \
    generation/rand_free_lab_trees.hpp \
    io/treebank_processor.hpp \
    io/treebank_reader.hpp \
    generation/rand_free_ulab_trees.hpp \
    properties/D_rla.hpp \
    properties/C_rla.hpp \
    C.hpp \
    D.hpp \
    properties/mhd.hpp \
    utils/bfs.hpp \
    utils/conversions.hpp \
    utils/cycles.hpp \
    utils/is_tree.hpp \
    utils/macros.hpp \
    utils/avl.hpp \
    io.hpp \
    utils/reachability.hpp \
    utils/sort_integers.hpp

SOURCES += \
    generation/rand_rooted_lab_dir_trees.cpp \
    graphs/dgraph.cpp \
    graphs/drtree.cpp \
    graphs/dtree.cpp \
    graphs/graph.cpp \
    graphs/rtree.cpp \
    graphs/tree.cpp \
    graphs/ugraph.cpp \
    graphs/urtree.cpp \
    graphs/utree.cpp \
    iterators/Q_iterator.cpp \
    iterators/edge_iterator.cpp \
    linarr/C.cpp \
    linarr/C_approx.cpp \
    linarr/classification.cpp \
    linarr/headedness.cpp \
    io/treebank_dataset.cpp \
    linarr/C_stack_based.cpp \
    numeric/integer.cpp \
    numeric/rational.cpp \
    numeric/gmp_utils.cpp \
    io/edge_list.cpp \
    properties/Q.cpp \
    linarr/D.cpp \
    properties/degrees.cpp \
    properties/mhd.cpp \
    properties/variance_C_gen_graphs_fast.cpp \
    linarr/C_brute_force.cpp \
    linarr/C_dyn_prog.cpp \
    linarr/C_ladder.cpp \
    generation/free_ulab_trees.cpp \
    properties/variance_C_freqs.cpp \
    properties/variance_C_gen_graphs.cpp \
    properties/variance_C_trees.cpp \
    generation/free_lab_trees.cpp \
    generation/rand_free_lab_trees.cpp \
    io/treebank_processor.cpp \
    io/treebank_reader.cpp \
    generation/rand_free_ulab_trees.cpp \
    properties/variance_C_forests.cpp \
    properties/D_rla.cpp \
    properties/C_rla.cpp \
    utils/conversions.cpp
