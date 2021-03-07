#!/bin/bash

# Remember that 
#
#      | sed "s/^/    /g"
#
# indents the output of a program!!

if [ -z $1 ]; then
	echo "Error: need c++/python"
	echo "    ./make_docs.sh c++/python"
	exit
fi

if [ "$1" != "c++" ] && [ "$1" != "python" ]; then
	echo "Error: need c++/python"
	echo "    Received '$1'"
	echo "    ./make_docs.sh c++/python"
	exit
fi

if [ "$1" == "c++" ]; then
	echo "Generating Doxygen (html) documentation of the C++ code..."
	doxygen doxyconfig/doxyfile.c++ | sed "s/^/    /g"
else
	echo -e "\e[1;1;32mGenerating Doxygen (html) documentation of the C++ code...\e[0m"
	doxygen doxyconfig/doxyfile.python | sed "s/^/    /g"
	
	####################################################################
	
	echo -e "\e[1;1;32mParsing XML files with 'doxy2swig.py' to generate some Python documentation\e[0m"
	python3 doxyconfig/doxy2swig.py -c -o -a pythondocshtml/xml/index.xml python-interface/modules/documentation.i | sed "s/^/    /g"
	
	####################################################################
	
	cd python-interface
	
	####################################################################
	
	echo -e "\e[1;1;32mClean up documentation.i\e[0m"
	scripts/clean_up_documentationi.sh | sed "s/^/    /g"
	
	####################################################################
	
	echo -e "\e[1;1;32mGenerating the Python interfaces only...\e[0m"
	echo -e "\e[1;1;33m    Release\e[0m"
	make BUILD=release python_interfaces | sed "s/^/        /g"
	echo -e "\e[1;1;33m    Debug\e[0m"
	make BUILD=debug python_interfaces | sed "s/^/        /g"
fi

