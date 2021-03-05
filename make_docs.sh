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
	echo "Generating Doxygen (html) documentation of the C++ code..."
	doxygen doxyconfig/doxyfile.python | sed "s/^/    /g"

	########################################################################

	echo "Parsing XML files with 'doxy2swig.py' to generate some Python documentation"
	python3 doxyconfig/doxy2swig.py -c -o -a pythondocshtml/xml/index.xml python-interface/submodules/documentation.i | sed "s/^/    /g"

	########################################################################

	cd python-interface

	########################################################################

	function apply_doc_sed() {
		F=$1	# file to which sed has to be applied
		echo "        Format 'Parameters' title appropriately"
		sed -E 's/Parameters:/Parameters:|/g' $F | tr '|' '\n' > /tmp/asdf
		mv /tmp/asdf $F
	}

	echo "Apply 'sed' to 'documentation.i' to remove unwanted strings"
	echo "    documentation.i..."
	apply_doc_sed submodules/documentation.i

	########################################################################

	echo "Generating the Python interfaces only..."
	echo "    Release"
	make BUILD=release python_interfaces -j4 | sed "s/^/    /g"
	echo "    Debug"
	make BUILD=debug python_interfaces -j4 | sed "s/^/    /g"
fi

