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

LAL_DIR = ..

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
# DEPENDENCIES (of the modules)

include Makefile.module.dependencies

# ----------------
# MODULE variables

include Makefile.module.variables

########################################################################
# GOALS

SUBMODULES	= $(DSE_SO) 	\
			  $(TST_SO) 	\
			  $(ALGCR_SO) 	\
			  $(GEN_SO) 	\
			  $(GRAPHS_SO) 	\
			  $(IO_SO)		\
			  $(ITER_SO)	\
			  $(LINARR_SO)	\
			  $(NMRC_SO)	\
			  $(PROP_SO)
ALL_SO		= $(SUBMODULES) $(LAL_SO)

all: directories $(ALL_SO)
directories: $(ARCH_DIR)
$(ARCH_DIR):
	mkdir -p $(ARCH_DIR)

########################################################################
# BUILDING RULES

include Makefile.module.rules

########################################################################
# CLEANING RULES

clean:
	rm -rf $(ARCH_DIR)/__pycache__/
	rm -f $(ALL_SO)
	rm -f $(ARCH_DIR)/*.o

distclean:
	rm -rf $(ARCH_DIR)

install: $(LAL_SO)
	cp $(LAL_SO) $(LAL_PY_INSTALL)/$(_LAL_SO)
	cp $(LAL_PY_FILE) $(LAL_PY_INSTALL)/$(_LAL_PF)
	cp $(DSE_SO) $(LAL_PY_INSTALL)/$(_DSE_SO)
	cp $(DSE_PY_FILE) $(LAL_PY_INSTALL)/$(_DSE_PF)
	cp $(TST_SO) $(LAL_PY_INSTALL)/$(_TST_SO)
	cp $(TST_PY_FILE) $(LAL_PY_INSTALL)/$(_TST_PF)
	cp $(ALGCR_SO) $(LAL_PY_INSTALL)/$(_ALGCR_SO)
	cp $(ALGCR_PY_FILE) $(LAL_PY_INSTALL)/$(_ALGCR_PF)
	cp $(GEN_SO) $(LAL_PY_INSTALL)/$(_GEN_SO)
	cp $(GEN_PY_FILE) $(LAL_PY_INSTALL)/$(_GEN_PF)
	cp $(GRAPHS_SO) $(LAL_PY_INSTALL)/$(_GRAPHS_SO)
	cp $(GRAPHS_PY_FILE) $(LAL_PY_INSTALL)/$(_GRAPHS_PF)
	cp $(IO_SO) $(LAL_PY_INSTALL)/$(_IO_SO)
	cp $(IO_PY_FILE) $(LAL_PY_INSTALL)/$(_IO_PF)
	cp $(ITER_SO) $(LAL_PY_INSTALL)/$(_ITER_SO)
	cp $(ITER_PY_FILE) $(LAL_PY_INSTALL)/$(_ITER_PF)
	cp $(LINARR_SO) $(LAL_PY_INSTALL)/$(_LINARR_SO)
	cp $(LINARR_PY_FILE) $(LAL_PY_INSTALL)/$(_LINARR_PF)
	cp $(NMRC_SO) $(LAL_PY_INSTALL)/$(_NMRC_SO)
	cp $(NMRC_PY_FILE) $(LAL_PY_INSTALL)/$(_NMRC_PF)
	cp $(PROP_SO) $(LAL_PY_INSTALL)/$(_PROP_SO)
	cp $(PROP_PY_FILE) $(LAL_PY_INSTALL)/$(_PROP_PF)

uninstall:
	rm -f $(LAL_PY_INSTALL)/$(_LAL_SO)
	rm -f $(LAL_PY_INSTALL)/$(_LAL_PF)
	rm -f $(LAL_PY_INSTALL)/$(_DSE_SO)
	rm -f $(LAL_PY_INSTALL)/$(_DSE_PF)
	rm -f $(LAL_PY_INSTALL)/$(_TST_SO)
	rm -f $(LAL_PY_INSTALL)/$(_TST_PF)
	rm -f $(LAL_PY_INSTALL)/$(_ALGCR_SO)
	rm -f $(LAL_PY_INSTALL)/$(_ALGCR_PF)
	rm -f $(LAL_PY_INSTALL)/$(_GEN_SO)
	rm -f $(LAL_PY_INSTALL)/$(_GEN_PF)
	rm -f $(LAL_PY_INSTALL)/$(_GRAPHS_SO)
	rm -f $(LAL_PY_INSTALL)/$(_GRAPHS_PF)
	rm -f $(LAL_PY_INSTALL)/$(_IO_SO)
	rm -f $(LAL_PY_INSTALL)/$(_IO_PF)
	rm -f $(LAL_PY_INSTALL)/$(_ITER_SO)
	rm -f $(LAL_PY_INSTALL)/$(_ITER_PF)
	rm -f $(LAL_PY_INSTALL)/$(_LINARR_SO)
	rm -f $(LAL_PY_INSTALL)/$(_LINARR_PF)
	rm -f $(LAL_PY_INSTALL)/$(_NMRC_SO)
	rm -f $(LAL_PY_INSTALL)/$(_NMRC_PF)
	rm -f $(LAL_PY_INSTALL)/$(_PROP_SO)
	rm -f $(LAL_PY_INSTALL)/$(_PROP_PF)
