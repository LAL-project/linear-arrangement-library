# BUILD OF COMPILATION AND ENVIRONMENT

# BUILD: debug/release
# ENVIR: devel (development) / dist (distribution)
#	* development: only for developers
#	* distribution: used to install the python interface to your
#		system.

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
	# this assumes that development happens within a Linux environment
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
	
	# location of LAL's header files
	LAL_INC_DIR = /usr/local/include
	# location of LAL's library files
	LAL_LIB_DIR = /usr/local/lib
endif
endif

# OS-dependent variables
ifeq ($(OS),Windows_NT)
	# OS directory
	OS_DIR	= win
	# LAL library's extension
	LIBRARY_EXTENSION = dll
	# Python module's extension
	SO_EXT	= pyd
else
	# OS directory
	# (this is not technically correct, but does the job for now)
	OS_DIR	= linux
	# LAL library's extension
	LIBRARY_EXTENSION = so
	# Python module's extension
	SO_EXT	= so
endif

ifeq ($(OS),Windows_NT)
	# ----------------------
	# FOR WINDOWS USERS ONLY

	# Python's minor version
	MINOR_VERSION_PYTHON = 8
	# Python 3 include dir
	PYTHON_INC_DIR	= C:/programming/Python/Python3$(MINOR_VERSION_PYTHON)/include
	# Python3 library directory
	PYTHON_LIBRARY	= C:/programming/Python/Python3$(MINOR_VERSION_PYTHON)/libs
	# Python3 linkage
	MAJOR_PY_LINK	= -lpython3
	MINOR_PY_LINK	= -lpython3$(MINOR_VERSION_PYTHON)
	# Directory where LAL's interface will be installed to
	LAL_PY_DEST		= C:/programming/python_lib_3.$(MINOR_VERSION_PYTHON)
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
	LAL_PY_DEST		= /usr/local/lib/python3.8/dist-packages
endif

# Architecture of the system
ARCH 			= $(shell getconf LONG_BIT)
ARCH_DIR 		= $(OS_DIR)/$(ARCH)

# SWIG executable
SWIG_EXE		= swig
# SWIG flags
SWIG_FLAGS_32	= -DSWIGWORDSIZE32
SWIG_FLAGS_64 	= -DSWIGWORDSIZE64
ifeq ($(OS),Windows_NT)
	SWIG_FLAGS	= $(SWIG_FLAGS_32)
else
	SWIG_FLAGS	= $(SWIG_FLAGS_$(ARCH))
endif

# Compiler to be used
CXX			= g++
# Flags for compiler
FLAGS		= -std=c++17 -fPIC -fopenmp
INCLUDES	= -I$(PYTHON_INC_DIR) -I$(LAL_INC_DIR)

# start building linkage libraries
ifeq ($(OS),Windows_NT)
ifeq ($(BUILD), debug)
	LIBS += -lpthread
endif
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

include Makefile.headers

# ----------------
# MODULE variables

include Makefile.module.variables

########################################################################
# GOALS

all: directories $(ALL_INSTALL_SO_FILES) $(ALL_INSTALL_PY_FILES)
python_interfaces: directories $(ALL_INSTALL_PY_FILES)
	rm -f $(ARCH_DIR)/temp.cxx	# clean up after yourself
directories: $(ARCH_DIR)/lal $(ARCH_DIR)/laldebug
$(ARCH_DIR)/lal:
	mkdir -p $(ARCH_DIR)/lal
$(ARCH_DIR)/laldebug:
	mkdir -p $(ARCH_DIR)/laldebug

########################################################################
# BUILDING RULES

include Makefile.module.rules

########################################################################
# CLEANING RULES

cleanwrap:
	rm -f $(ARCH_DIR)/$(INTERFACE_DIRECTORY)/*.cxx
	rm -f $(ARCH_DIR)/$(INTERFACE_DIRECTORY)/*.o

cleanso:
	rm -f $(ARCH_DIR)/$(INTERFACE_DIRECTORY)/*.$(SO_EXT)

clean:
	rm -rf $(ARCH_DIR)/$(INTERFACE_DIRECTORY)/*

distclean:
	rm -rf $(ARCH_DIR)

install: $(LAL_SO)
	mkdir -p $(LAL_PY_DEST)/$(INTERFACE_DIRECTORY)
	cp $(ALL_INSTALL_SO_FILES) $(LAL_PY_DEST)/$(INTERFACE_DIRECTORY)/
	cp $(ALL_INSTALL_PY_FILES) $(LAL_PY_DEST)/$(INTERFACE_DIRECTORY)/

uninstall:
	rm -f $(LAL_PY_DEST)/$(INTERFACE_DIRECTORY)/
	rm -f $(LAL_PY_DEST)/$(INTERFACE_DIRECTORY)/
