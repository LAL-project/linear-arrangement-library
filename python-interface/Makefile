# BUILD OF COMPILATION AND DISTRIBUTION
# compilation is debug by default
ifeq ($(BUILD), )
BUILD = debug
endif
# distribution is local by default
ifeq ($(DIST), )
DIST = local
endif

ifneq ($(BUILD),debug)
ifneq ($(BUILD),release)
$(info Error: invalid value for BUILD variable: $(BUILD))
endif
endif
# distribution is local by default
ifneq ($(DIST),local)
ifneq ($(DIST),dist)
$(info Error: invalid value for DIST variable: $(DIST))
endif
endif

########################################################################
# VARIABLES
PYTHON_INCLUDE	= /usr/include/python3.6/

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

# Architecture of the system
ARCH 			= $(shell getconf LONG_BIT)
ARCH_DIR 		= $(OS_DIR)/$(ARCH)

# SWIG flags
SWIG_FLAGS_32	= -DSWIGWORDSIZE32
SWIG_FLAGS_64 	= -DSWIGWORDSIZE64
ifeq ($(OS),Windows_NT)
	SWIG_FLAGS	= $(SWIG_FLAGS_32)
else
	SWIG_FLAGS	= $(SWIG_FLAGS_$(ARCH))
endif

# Compiler to be used
CXX				= g++
# Flags for 'g++'
COMMON_FLAGS	= -std=c++17 -fPIC -fopenmp
INCLUDES		= -I$(PYTHON_INCLUDE)

ifeq ($(DIST),local)
INCLUDES		+= -I$(LAL_DIR)
else
endif

# set lib and flags according to the mode of compilation
# define LAL's directory and dependencies
ifeq ($(BUILD),debug)
	include Makefile.variables.debug
else
ifeq ($(BUILD),release)
	include Makefile.variables.release
endif
endif

# ------------
# DEPENDENCIES

ifeq ($(DIST),local)
LAL_DIR = ..
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
else
LAL_DIR 		= 
LAL_DEP_HEADERS	=
DSE_DEP_HEADERS	=
endif

# ----------------
# MODULE variables

# lal module
LAL_MIMPL		= _$(LAL_BNAME)_impl.i
_LAL_PF			= $(LAL_MNAME).py
LAL_PY_FILE		= $(ARCH_DIR)/$(_LAL_PF)
LAL_WRAP		= $(ARCH_DIR)/$(LAL_MNAME)_wrap
_LAL_SO			= _$(LAL_MNAME).$(SO_EXT)
LAL_SO			= $(ARCH_DIR)/$(_LAL_SO)

# dataset_error module
DSE_MIMPL		= $(DSE_BNAME).i
_DSE_PF			= $(DSE_MNAME).py
DSE_PY_FILE		= $(ARCH_DIR)/$(_DSE_PF)
DSE_WRAP		= $(ARCH_DIR)/$(DSE_MNAME)_wrap
_DSE_SO			= _$(DSE_MNAME).$(SO_EXT)
DSE_SO			= $(ARCH_DIR)/$(_DSE_SO)

########################################################################
# GOALS

SUBMODULES		= $(DSE_SO)
ALL_SO			= $(SUBMODULES) $(LAL_SO)

all: directories $(ALL_SO)
directories: $(ARCH_DIR)
$(ARCH_DIR):
	mkdir -p $(ARCH_DIR)

########################################################################
# BUILDING RULES

$(LAL_SO): $(LAL_WRAP).o $(LAL_LIB_DEP) $(SUBMODULES)
	$(CXX) $(FLAGS) -shared -fPIC -o $@ $< $(LIBS)
$(LAL_WRAP).o: $(LAL_WRAP).cxx
	$(CXX) $(FLAGS) -o $@ -c $< $(INCLUDES)
$(LAL_WRAP).cxx: $(LAL_MNAME).i $(LAL_DEP_HEADERS) $(LAL_MIMPL)
	swig $(SWIG_FLAGS) -Wall -c++ -python -py3 -o $@ $(LAL_MNAME).i

$(DSE_SO): $(DSE_WRAP).o $(LAL_LIB_DEP)
	$(CXX) $(FLAGS) -shared -fPIC -o $@ $< $(LIBS)
$(DSE_WRAP).o: $(DSE_WRAP).cxx
	$(CXX) $(FLAGS) -o $@ -c $< $(INCLUDES)
$(DSE_WRAP).cxx: $(DSE_MNAME).i $(DSE_DEP_HEADERS) $(DSE_MIMPL)
	swig $(SWIG_FLAGS) -Wall -c++ -python -py3 -o $@ $(DSE_MNAME).i

########################################################################
# CLEANING RULES

clean:
	rm -rf $(ARCH_DIR)/__pycache__/
	rm -f $(ALL_SO)
	rm -f $(ARCH_DIR)/*.o

distclean:
	rm -rf $(ARCH_DIR)

install: $(LAL_SO)
	# install LAL
	cp $(LAL_SO) $(LAL_PY_INSTALL)/$(_LAL_SO)
	cp $(LAL_PY_FILE) $(LAL_PY_INSTALL)/$(_LAL_PF)
	# install dataset_error
	cp $(DSE_SO) $(LAL_PY_INSTALL)/$(_DSE_SO)
	cp $(DSE_PY_FILE) $(LAL_PY_INSTALL)/$(_DSE_PF)
	
uninstall:
	rm -f $(LAL_PY_INSTALL)/$(_LAL_SO)
	rm -f $(LAL_PY_INSTALL)/$(_LAL_PF)
	rm -f $(LAL_PY_INSTALL)/$(_DSE_SO)
	rm -f $(LAL_PY_INSTALL)/$(_DSE_PF)
