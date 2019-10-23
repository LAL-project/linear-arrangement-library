
########################################################################
# VARIABLES
PYTHON_INCLUDE	= /usr/include/python3.6/
LAL_DIR			= ..
LAL_DEP_HEADERS	= $(LAL_DIR)/lal/definitions.hpp \
				  $(LAL_DIR)/lal/graph.hpp \
				  $(LAL_DIR)/lal/conversions/conversions.hpp \
				  $(LAL_DIR)/lal/generation/free_ulab_trees.hpp \
				  $(LAL_DIR)/lal/generation/free_lab_trees.hpp \
				  $(LAL_DIR)/lal/generation/rand_free_lab_trees.hpp \
				  $(LAL_DIR)/lal/generation/rand_free_ulab_trees.hpp \
				  $(LAL_DIR)/lal/io/edge_list.hpp \
				  $(LAL_DIR)/lal/io/treebank_processor.hpp \
				  $(LAL_DIR)/lal/io/treebank_dataset.hpp \
				  $(LAL_DIR)/lal/io/treebank_reader.hpp \
				  $(LAL_DIR)/lal/linarr/C.hpp \
				  $(LAL_DIR)/lal/linarr/D.hpp \
				  $(LAL_DIR)/lal/numeric/gmp_utils.hpp \
				  $(LAL_DIR)/lal/numeric/integer.hpp \
				  $(LAL_DIR)/lal/numeric/rational.hpp \
				  $(LAL_DIR)/lal/properties/Q.hpp \
				  $(LAL_DIR)/lal/properties/degrees.hpp \
				  $(LAL_DIR)/lal/properties/C_rla.hpp \
				  $(LAL_DIR)/lal/properties/D_rla.hpp

DSE_DEP_HEADERS	= $(LAL_DIR)/lal/io/dataset_error.hpp

ifeq ($(OS),Windows_NT)
	# Python 3 include dir
	PYTHON_INCLUDE	= C:/programming/Python37/include
	# Directory where LAL's interface will be installed to
	LAL_PY_INSTALL	= C:/programming/python_lib
	# Python3 linkage
	PYTHON_LIBRARY	= -lpython37 -lpython3
else
	# Python 3 include dir
	PYTHON_INCLUDE	= /usr/include/python3.6
	# Directory where LAL's interface will be installed to
	LAL_PY_INSTALL	= /usr/lib/python3.6
	# Python3 linkage (not needed)
	PYTHON_LIBRARY	= 
endif

########################################################################
# VARIABLES

# Compiler to be used
CXX				= g++
# Flags for 'g++'
COMMON_FLAGS	= -std=c++17 -fPIC -fopenmp
FLAGS_DEB		= $(COMMON_FLAGS) -g -O0 -DDEBUG -D_GLIBCXX_DEBUG
FLAGS_REL		= $(COMMON_FLAGS) -O3 -DNDEBUG
LAL_LIB_DEB		= -L $(LAL_DIR)/lal-debug -llaldebug
LAL_LIB_REL		= -L $(LAL_DIR)/lal-release -llal
LIBS_DEB		= $(LAL_LIB_DEB) -lgmp -fopenmp $(PYTHON_LIBRARY)
LIBS_REL		= $(LAL_LIB_REL) -lgmp -fopenmp $(PYTHON_LIBRARY)
INCLUDES		= -I$(PYTHON_INCLUDE) -I$(LAL_DIR)

# OS-dependent variables
ifeq ($(OS),Windows_NT)
	# OS directory
	OS_DIR	= win
	# Python module's extension
	SO_EXT	= pyd
else
	# OS directory
	# (this is not technically correct, but does the job for now)
	OS_DIR	= linux
	# Python module's extension
	SO_EXT	= so
endif

# Architecture of the system
ARCH 			= $(shell getconf LONG_BIT)
ARCH_DIR 		= $(OS_DIR)/$(ARCH)

# SWIG flags
SWIG_FLAGS_32	= -DSWIGWORDSIZE32
SWIG_FLAGS_64 	= -DSWIGWORDSIZE64
ifeq ($(OS),Windows_NT)
	SWIG_FLAGS_ARCH = $(SWIG_FLAGS_32)
else
	SWIG_FLAGS_ARCH = $(SWIG_FLAGS_$(ARCH))
endif

########################################################################
# Local module and interface file names

# module base name
LAL_BNAME		= lal
LAL_DEB_BNAME	= $(LAL_BNAME)debug
LAL_REL_BNAME	= $(LAL_BNAME)

# python Debug/Release file
_LAL_PDF		= $(LAL_DEB_BNAME).py
_LAL_PRF		= $(LAL_REL_BNAME).py
LAL_PY_DEB_FILE	= $(ARCH_DIR)/$(_LAL_PDF)
LAL_PY_REL_FILE	= $(ARCH_DIR)/$(_LAL_PRF)

# wrap Debug/Release file
LAL_WRAP_DEB	= $(ARCH_DIR)/$(LAL_BNAME)_wrap_debug
LAL_WRAP_REL	= $(ARCH_DIR)/$(LAL_BNAME)_wrap

# LAL Debug/Release .so
LAL_DEB_SO		= $(ARCH_DIR)/_$(LAL_DEB_BNAME).$(SO_EXT)
LAL_REL_SO		= $(ARCH_DIR)/_$(LAL_REL_BNAME).$(SO_EXT)

########################################################################
# Common modules

# Module base name
DSE_BNAME		= dataset_error
# Python file
_DSE_PF			= $(DSE_BNAME).py
DSE_PY_FILE		= $(ARCH_DIR)/$(_DSE_PF)
# Wrap file
DSE_WRAP		= $(ARCH_DIR)/$(DSE_BNAME)_wrap
# Module file
DSE_SO			= $(ARCH_DIR)/_$(DSE_BNAME).$(SO_EXT)

########################################################################
# GOALS

COMMON_SO		= $(DSE_SO)
ALL_DEB_SO		= $(COMMON_SO) $(LAL_DEB_SO)
ALL_REL_SO		= $(COMMON_SO) $(LAL_REL_SO)

debug: directories $(ALL_DEB_SO)
release: directories $(ALL_REL_SO)
directories: $(ARCH_DIR)
$(ARCH_DIR):
	mkdir -p $(ARCH_DIR)

########################################################################
# BUILDING RULES

# -- COMMON SHARED OBJECTS

# dataset_error
$(DSE_SO): $(DSE_WRAP).o $(LAL_DIR)/lal/io/dataset_error.hpp
	$(CXX) -shared -fPIC -o $@ $<
$(DSE_WRAP).o: $(DSE_WRAP).cxx
	$(CXX) $(FLAGS_REL) -o $@ -c $< $(INCLUDES)
$(DSE_WRAP).cxx: $(DSE_BNAME).i $(DSE_DEP_HEADERS)
	swig $(SWIG_FLAGS_ARCH) -Wall -c++ -python -py3 -o $@ $(DSE_BNAME).i

# -- LIBRARY SHARED OBJECTS

# DEBUG
$(LAL_DEB_SO): $(LAL_WRAP_DEB).o $(LAL_DIR)/lal-debug/liblaldebug.a
	$(CXX) -shared -fPIC -o $@ $< $(LIBS_DEB)
$(LAL_WRAP_DEB).o: $(LAL_WRAP_DEB).cxx
	$(CXX) $(FLAGS_DEB) -o $@ -c $< $(INCLUDES)
$(LAL_WRAP_DEB).cxx: $(LAL_DEB_BNAME).i $(LAL_DEP_HEADERS) lal_impl.i
	swig $(SWIG_FLAGS_ARCH) -DDEBUG -Wall -c++ -python -py3 -o $@ $(LAL_DEB_BNAME).i

# RELEASE
$(LAL_REL_SO): $(LAL_WRAP_REL).o $(LAL_DIR)/lal-release/liblal.a
	$(CXX) -shared -fPIC -o $@ $< $(LIBS_REL) 
$(LAL_WRAP_REL).o: $(LAL_WRAP_REL).cxx
	$(CXX) $(FLAGS_REL) -o $@ -c $< $(INCLUDES)
$(LAL_WRAP_REL).cxx: $(LAL_REL_BNAME).i $(LAL_DEP_HEADERS) lal_impl.i
	swig $(SWIG_FLAGS_ARCH) -Wall -c++ -python -py3 -o $@ $(LAL_REL_BNAME).i

########################################################################
# CLEANING RULES

clean:
	rm -rf __pycache__/
	rm -f $(LAL_DEB_SO)
	rm -f $(LAL_REL_SO)
	rm -f $(ARCH_DIR)/*.o

distclean:
	rm -rf $(ARCH_DIR)
