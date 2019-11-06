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
    generation/rand_rooted_lab_dir_trees.hpp \
    graphs.hpp \
    graphs/dgraph.hpp \
    graphs/graph.hpp \
    graphs/rdtree.hpp \
    graphs/rutree.hpp \
    graphs/ugraph.hpp \
    iterators.hpp \
    iterators/Q_iterator.hpp \
    iterators/edge_iterator.hpp \
    linarr/headedness.hpp \
    io/dataset_error.hpp \
    io/treebank_dataset.hpp \
    numeric/integer.hpp \
    numeric/rational.hpp \
    numeric/gmp_utils.hpp \
    io/edge_list.hpp \
    properties/degrees.hpp \
    properties/Q.hpp \
    linarr/C.hpp \
    linarr/D.hpp \
    generation/free_ulab_trees.hpp \
    generation/free_lab_trees.hpp \
    generation/rand_free_lab_trees.hpp \
    conversions/conversions.hpp \
    io/treebank_processor.hpp \
    io/treebank_reader.hpp \
    generation/rand_free_ulab_trees.hpp \
    properties/D_rla.hpp \
    properties/C_rla.hpp \
    C.hpp \
    D.hpp \
    properties/mhd.hpp \
    utils/bfs.hpp \
    utils/macros.hpp \
    utils/avl.hpp \
    io.hpp \
    utils/sort_integers.hpp

SOURCES += \
    generation/rand_rooted_lab_dir_trees.cpp \
    graphs/dgraph.cpp \
    graphs/graph.cpp \
    graphs/rdtree.cpp \
    graphs/rutree.cpp \
    graphs/ugraph.cpp \
    iterators/Q_iterator.cpp \
    iterators/edge_iterator.cpp \
    linarr/headedness.cpp \
    properties/hubiness.cpp \
    io/treebank_dataset.cpp \
    linarr/C_stack_based.cpp \
    numeric/integer.cpp \
    numeric/rational.cpp \
    numeric/gmp_utils.cpp \
    io/edge_list.cpp \
    properties/degrees.cpp \
    properties/Q.cpp \
    linarr/D.cpp \
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
    conversions/conversions.cpp \
    linarr/C_q.cpp \
    io/treebank_processor.cpp \
    io/treebank_reader.cpp \
    generation/rand_free_ulab_trees.cpp \
    properties/variance_C_forests.cpp \
    properties/D_rla.cpp \
    properties/C_rla.cpp \
    linarr/approx_C.cpp
