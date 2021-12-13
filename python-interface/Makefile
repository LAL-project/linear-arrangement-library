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
exit 1
endif
endif

# distribution is devel by default
ifneq ($(ENVIR),devel)
ifneq ($(ENVIR),dist)
$(info Error: invalid value for ENVIR variable: $(ENVIR))
exit 1
endif
endif

OS_ID = unknown
ifeq ($(OS),Windows_NT)
	OS_ID = windows
else
	# from https://gist.github.com/sighingnow/deee806603ec9274fd47
	UNAME_S = $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		OS_ID = linux
	endif
	
	ifeq ($(UNAME_S),Darwin)
		OS_ID = macos
	endif
endif

$(info Building for OS '$(OS_ID)')
ifeq ($(OS_ID),unknown)

$(info Error: Unknown Operative System '$(OS_ID)'. Contact the developers)
$(info .    https://github.com/LAL-project/linear-arrangement-library)
$(info .    https://cqllab.upc.edu/lal/)

all: leave
leave:
	exit 1

endif

########################################################################
# OUTPUT DEBUG INFORMATION

$(info Compilation mode: $(BUILD))
$(info Link against: $(ENVIR))
$(info .    'devel' = development distribution)
$(info .    'dist'  = installed version)

########################################################################
# VARIABLES

ifeq ($(ENVIR), devel)
	# this assumes that development happens within the python-interface directory
	LAL_INC_DIR = ..
	ifeq ($(BUILD), debug)
		LAL_LIB_DIR = ../lal-debug
	else
		LAL_LIB_DIR = ../lal-release
	endif
else

ifeq ($(OS_ID),windows)
	# ------------------
	# WINDOWS USERS ONLY
	
	# where are LAL's include files if installed with default values in CMake
	LAL_INC_DIR = C:/programming/c++/include
	# where are LAL's library files if installed with default values in CMake
	LAL_LIB_DIR = C:/programming/c++/lib

else ifeq ($(OS_ID),linux)
	# ----------------
	# LINUX USERS ONLY
	
	# location of LAL's header files if installed with default values in CMake
	LAL_INC_DIR = /usr/local/include
	# location of LAL's library files if installed with default values in CMake
	LAL_LIB_DIR = /usr/local/lib

else ifeq ($(OS_ID),macos)
	# ----------------
	# MACOS USERS ONLY
	
	# location of LAL's header files if installed with default values in CMake
	LAL_INC_DIR = /usr/local/include
	# location of LAL's library files if installed with default values in CMake
	LAL_LIB_DIR = /usr/local/lib
endif

endif

$(info LAL source)
$(info .    LAL include header files in: $(LAL_INC_DIR))
$(info .    LAL library files in: $(LAL_LIB_DIR))

# directory (in which the compiled files will be stored)
OS_DIR	= $(OS_ID)
ifeq ($(OS_ID),windows)
	
	# LAL library's extension (liblaldebug.dll)
	LIBRARY_EXTENSION = dll
	# Python module's extension
	SO_EXT = pyd
	
else ifeq ($(OS_ID),linux)
	
	# LAL library's extension (liblaldebug.so)
	LIBRARY_EXTENSION = so
	# Python module's extension
	SO_EXT = so

else ifeq ($(OS_ID),macos)
	
	# LAL library's extension (liblaldebug.dylib)
	LIBRARY_EXTENSION = dylib
	# Python module's extension
	SO_EXT = so

endif

$(info File format)
$(info .    Library extension: $(LIBRARY_EXTENSION))
$(info .    Python modules extension: $(SO_EXT))

ifeq ($(OS_ID),windows)
	# ------------------
	# WINDOWS USERS ONLY

	# Python's minor version
	MINOR_VERSION_PYTHON = 8
	
	# Python 3 include dir
	PYTHON_INC_DIR	= C:/programming/Python/Python3$(MINOR_VERSION_PYTHON)/include
	
	# Python3 library directory
	PYTHON_LIB_DIR	= C:/programming/Python/Python3$(MINOR_VERSION_PYTHON)/libs
	
	# Python3 linkage
	MAJOR_PY_LINK	= -lpython3
	MINOR_PY_LINK	= -lpython3$(MINOR_VERSION_PYTHON)
	
	# Directory where LAL's interface will be installed to
	LAL_PY_DEST		= C:/programming/python_lib_3.$(MINOR_VERSION_PYTHON)
	
	# Other flags needed to compile against Python
	EXTRA_FLAGS		= 
	
else ifeq ($(OS_ID),linux)
	# ----------------
	# LINUX USERS ONLY
	
	# Python's minor version
	MINOR_VERSION_PYTHON = 8
	
	# Python 3 include dir
	PYTHON_INC_DIR	= /usr/include/python3.$(MINOR_VERSION_PYTHON)
	
	# Python3 library directory
	PYTHON_LIB_DIR	= /usr/lib/x86_64-linux-gnu/
	
	# Python3 linkage
	MAJOR_PY_LINK	=
	MINOR_PY_LINK	= -lpython3.$(MINOR_VERSION_PYTHON)
	
	# Directory where LAL's interface will be installed to
	LAL_PY_DEST		= /usr/local/lib/python3.$(MINOR_VERSION_PYTHON)/dist-packages
	
	# Other flags needed to compile against Python
	EXTRA_FLAGS		= 
	
else ifeq ($(OS_ID),macos)
	# ----------------
	# MACOS USERS ONLY
	
	# Python's minor version
	MINOR_VERSION_PYTHON = 8
	
	# Python 3 include dir
	PYTHON_INC_DIR	= ~/opt/anaconda3/include/python3.$(MINOR_VERSION_PYTHON)
	
	# Python3 library directory
	PYTHON_LIB_DIR	= ~/opt/anaconda3/lib/python3.$(MINOR_VERSION_PYTHON)
	
	# Python3 linkage
	MAJOR_PY_LINK	=
	MINOR_PY_LINK	= -lpython3.$(MINOR_VERSION_PYTHON)
	
	# Directory where LAL's interface will be installed to
	LAL_PY_DEST		= ~/opt/anaconda3/lib/python3.$(MINOR_VERSION_PYTHON)/site-packages
	
	# Other flags needed to compile against Python
	EXTRA_FLAGS		= -flat_namespace
	
endif

$(info Python linkage)
$(info .    Minor version of python: $(MINOR_VERSION_PYTHON))
$(info .    Python include directory: $(PYTHON_INC_DIR))
$(info .    Python library directory: $(PYTHON_LIB_DIR))
$(info .    Linkage flag against major: $(MAJOR_PY_LINK))
$(info .    Linkage flag against minor: $(MINOR_PY_LINK))
$(info .    Installation directory: $(LAL_PY_DEST))
$(info .    Other linkage flags: $(EXTRA_FLAGS))

# --------------------------
# Architecture of the system
ARCH 			= $(shell getconf LONG_BIT)
ARCH_DIR 		= $(OS_DIR)/$(ARCH)

# ---------------
# SWIG executable
SWIG_EXE		= swig
# SWIG flags
SWIG_FLAGS_32	= -DSWIGWORDSIZE32
SWIG_FLAGS_64 	= -DSWIGWORDSIZE64
ifeq ($(OS_ID),windows)
	SWIG_FLAGS	= $(SWIG_FLAGS_32)
else ifeq ($(OS_ID),linux)
	SWIG_FLAGS	= $(SWIG_FLAGS_$(ARCH))
else ifeq ($(OS_ID),macos)
	SWIG_FLAGS	= 
endif

# -------------------
# Compiler to be used

ifeq ($(OS_ID),windows)
	# ------------------
	# WINDOWS USERS ONLY
	
	CXX			= g++
	FLAGS		= -std=c++17 -fPIC -fopenmp
	INCLUDES	= -I $(PYTHON_INC_DIR) -I$(LAL_INC_DIR)

else ifeq ($(OS_ID),linux)
	# ----------------
	# LINUX USERS ONLY
	
	CXX			= g++
	FLAGS		= -std=c++17 -fPIC -fopenmp
	INCLUDES	= -I $(PYTHON_INC_DIR) -I$(LAL_INC_DIR)

else ifeq ($(OS_ID),macos)
	# ----------------
	# MACOS USERS ONLY
	
	CXX			= /usr/local/Cellar/gcc/11.2.0/bin/g++-11
	FLAGS		= -std=c++17 -fPIC -fopenmp
	INCLUDES	= -I $(PYTHON_INC_DIR) -I$(LAL_INC_DIR)

endif

# set lib and flags according to the mode of compilation
# define LAL's directory and dependencies
ifeq ($(BUILD),debug)
	# -----------------
	# compilation flags

	FLAGS		+= -g -O3 -DDEBUG -D_GLIBCXX_DEBUG
	LIBS		+= -L $(LAL_LIB_DIR) -llaldebug
	SWIG_FLAGS	+= -DDEBUG -D_GLIBCXX_DEBUG

	# -----------------------
	# dependencies in modules

	ifeq ($(ENVIR),devel)
		LIBRARY_SHARED_DEP	= $(LAL_LIB_DIR)/liblaldebug.$(LIBRARY_EXTENSION)
	else
		LIBRARY_SHARED_DEP	=
	endif

	# -------------------------
	# directory for the modules

	INTERFACE_DIRECTORY	= laldebug
else ifeq ($(BUILD),release)
	# -----------------
	# compilation flags

	FLAGS		+= -O3 -UDEBUG -DNDEBUG
	LIBS		+= -L $(LAL_LIB_DIR) -llal
	SWIG_FLAGS	+= -DNDEBUG

	# -----------------------
	# dependencies in modules

	ifeq ($(ENVIR),devel)
		LIBRARY_SHARED_DEP	= $(LAL_LIB_DIR)/liblal.$(LIBRARY_EXTENSION)
	else
		LIBRARY_SHARED_DEP	=
	endif

	# -------------------------
	# directory for the modules

	INTERFACE_DIRECTORY	= lal
endif

# complete linkage flags
LIBS += -lgmp -fopenmp -lpthread $(EXTRA_FLAGS)
ifeq ($(PYTHON_LIB_DIR), )
	ifneq ($(MINOR_PY_LINK), )
		LIBS += -L $(MINOR_PY_LINK)
	endif
	ifneq ($(MAJOR_PY_LINK), )
		LIBS += -L $(MAJOR_PY_LINK)
	endif
else
	ifneq ($(MINOR_PY_LINK), )
		LIBS += -L $(PYTHON_LIB_DIR) $(MINOR_PY_LINK)
	endif
	ifneq ($(MAJOR_PY_LINK), )
		LIBS += -L $(PYTHON_LIB_DIR) $(MAJOR_PY_LINK)
	endif
endif

$(info Compiler)
$(info .    Path: $(CXX))
$(info .    Flags: $(FLAGS))
$(info .    Includes: $(INCLUDES))
$(info .    Linked libraries: $(LIBS))

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
