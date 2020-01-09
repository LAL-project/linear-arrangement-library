# BUILD OF COMPILATION AND ENVIRRIBUTION
# compilation is debug by default
ifeq ($(BUILD), )
BUILD = debug
endif
# distribution is devel by default
ifeq ($(ENVIR), )
ENVIR = devel
endif

ifneq ($(BUILD),debug)
ifneq ($(BUILD),release)
$(info Error: invalid value for BUILD variable: $(BUILD))
endif
endif
# distribution is devel by default
ifneq ($(ENVIR),devel)
ifneq ($(ENVIR),dist)
$(info Error: invalid value for ENVIR variable: $(ENVIR))
endif
endif

########################################################################
# VARIABLES

ifeq ($(ENVIR), devel)
	LAL_INC_DIR = ..
	ifeq ($(BUILD), debug)
		LAL_LIB_DIR = ../lal-debug
	else
		LAL_LIB_DIR = ../lal-release
	endif
else
ifeq ($(OS),Windows_NT)
	# ----------------------
	# FOR WINDOWS USERS ONLY
	
	# where are LAL's include files
	LAL_INC_DIR = C:/programming/c++/include
	# where are LAL's library files
	LAL_LIB_DIR = C:/programming/c++/lib
else
	# --------------------
	# FOR LINUX USERS ONLY
	
	# where are LAL's include files
	LAL_INC_DIR = /usr/local/include
	# where are LAL's library files
	LAL_LIB_DIR = /usr/local/lib
endif
endif

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
	# ----------------------
	# FOR WINDOWS USERS ONLY

	# Python 3 include dir
	PYTHON_INC_DIR	= C:/programming/Python38/include
	# Python3 library directory
	PYTHON_LIBRARY	= C:/programming/Python38/libs
	# Python3 linkage
	MAJOR_PY_LINK	= -lpython3
	MINOR_PY_LINK	= -lpython38
	# Directory where LAL's interface will be installed to
	LAL_PY_DEST		= C:/programming/python_lib
else
	# --------------------
	# FOR LINUX USERS ONLY
	
	# Python 3 include dir
	PYTHON_INC_DIR	= /usr/include/python3.8
	# Python3 library directory
	PYTHON_LIBRARY	= /usr/lib/x86_64-linux-gnu/
	# Python3 linkage
	MAJOR_PY_LINK	=
	MINOR_PY_LINK	= -lpython3.8
	# Directory where LAL's interface will be installed to
	LAL_PY_DEST		= /usr/lib/python3.8
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
FLAGS			= -std=c++17 -fPIC -fopenmp
INCLUDES		= -I$(PYTHON_INC_DIR) -I$(LAL_INC_DIR)

# set lib and flags according to the mode of compilation
# define LAL's directory and dependencies
ifeq ($(BUILD),debug)
	include Makefile.variables.debug
else
ifeq ($(BUILD),release)
	include Makefile.variables.release
endif
endif

# complete linkage flags
LIBS += -lgmp -fopenmp
ifeq ($(PYTHON_LIBRARY), )
	ifneq ($(MINOR_PY_LINK), )
		LIBS += -L $(MINOR_PY_LINK)
	endif
	ifneq ($(MAJOR_PY_LINK), )
		LIBS += -L $(MAJOR_PY_LINK)
	endif
else
	ifneq ($(MINOR_PY_LINK), )
		LIBS += -L $(PYTHON_LIBRARY) $(MINOR_PY_LINK)
	endif
	ifneq ($(MAJOR_PY_LINK), )
		LIBS += -L $(PYTHON_LIBRARY) $(MAJOR_PY_LINK)
	endif
endif

# ------------
# DEPENDENCIES (headers)

include Makefile.module.headdep

# ----------------
# MODULE variables

include Makefile.module.variables

########################################################################
# GOALS

SUBMODULES	= $(DSE_SO) 	\
			  $(TST_SO) 	\
			  $(ALGCR_SO) 	\
			  $(DEFS_SO) 	\
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
	cp $(LAL_SO) $(LAL_PY_DEST)/$(_LAL_SO)
	cp $(LAL_PY_FILE) $(LAL_PY_DEST)/$(_LAL_PF)
	cp $(DSE_SO) $(LAL_PY_DEST)/$(_DSE_SO)
	cp $(DSE_PY_FILE) $(LAL_PY_DEST)/$(_DSE_PF)
	cp $(TST_SO) $(LAL_PY_DEST)/$(_TST_SO)
	cp $(TST_PY_FILE) $(LAL_PY_DEST)/$(_TST_PF)
	cp $(ALGCR_SO) $(LAL_PY_DEST)/$(_ALGCR_SO)
	cp $(ALGCR_PY_FILE) $(LAL_PY_DEST)/$(_ALGCR_PF)
	cp $(DEFS_SO) $(LAL_PY_DEST)/$(_DEFS_SO)
	cp $(DEFS_PY_FILE) $(LAL_PY_DEST)/$(_DEFS_PF)
	cp $(GEN_SO) $(LAL_PY_DEST)/$(_GEN_SO)
	cp $(GEN_PY_FILE) $(LAL_PY_DEST)/$(_GEN_PF)
	cp $(GRAPHS_SO) $(LAL_PY_DEST)/$(_GRAPHS_SO)
	cp $(GRAPHS_PY_FILE) $(LAL_PY_DEST)/$(_GRAPHS_PF)
	cp $(IO_SO) $(LAL_PY_DEST)/$(_IO_SO)
	cp $(IO_PY_FILE) $(LAL_PY_DEST)/$(_IO_PF)
	cp $(ITER_SO) $(LAL_PY_DEST)/$(_ITER_SO)
	cp $(ITER_PY_FILE) $(LAL_PY_DEST)/$(_ITER_PF)
	cp $(LINARR_SO) $(LAL_PY_DEST)/$(_LINARR_SO)
	cp $(LINARR_PY_FILE) $(LAL_PY_DEST)/$(_LINARR_PF)
	cp $(NMRC_SO) $(LAL_PY_DEST)/$(_NMRC_SO)
	cp $(NMRC_PY_FILE) $(LAL_PY_DEST)/$(_NMRC_PF)
	cp $(PROP_SO) $(LAL_PY_DEST)/$(_PROP_SO)
	cp $(PROP_PY_FILE) $(LAL_PY_DEST)/$(_PROP_PF)

uninstall:
	rm -f $(LAL_PY_DEST)/$(_LAL_SO)
	rm -f $(LAL_PY_DEST)/$(_LAL_PF)
	rm -f $(LAL_PY_DEST)/$(_DSE_SO)
	rm -f $(LAL_PY_DEST)/$(_DSE_PF)
	rm -f $(LAL_PY_DEST)/$(_TST_SO)
	rm -f $(LAL_PY_DEST)/$(_TST_PF)
	rm -f $(LAL_PY_DEST)/$(_ALGCR_SO)
	rm -f $(LAL_PY_DEST)/$(_ALGCR_PF)
	rm -f $(LAL_PY_DEST)/$(_DEFS_SO)
	rm -f $(LAL_PY_DEST)/$(_DEFS_PF)
	rm -f $(LAL_PY_DEST)/$(_GEN_SO)
	rm -f $(LAL_PY_DEST)/$(_GEN_PF)
	rm -f $(LAL_PY_DEST)/$(_GRAPHS_SO)
	rm -f $(LAL_PY_DEST)/$(_GRAPHS_PF)
	rm -f $(LAL_PY_DEST)/$(_IO_SO)
	rm -f $(LAL_PY_DEST)/$(_IO_PF)
	rm -f $(LAL_PY_DEST)/$(_ITER_SO)
	rm -f $(LAL_PY_DEST)/$(_ITER_PF)
	rm -f $(LAL_PY_DEST)/$(_LINARR_SO)
	rm -f $(LAL_PY_DEST)/$(_LINARR_PF)
	rm -f $(LAL_PY_DEST)/$(_NMRC_SO)
	rm -f $(LAL_PY_DEST)/$(_NMRC_PF)
	rm -f $(LAL_PY_DEST)/$(_PROP_SO)
	rm -f $(LAL_PY_DEST)/$(_PROP_PF)
