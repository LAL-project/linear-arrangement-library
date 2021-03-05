#!/bin/bash

for f in $@; do
	echo $f
	
	# Clean up strings produced by 'documentation.i'
	# These are found in the documentation of overloaded functions.
	
	echo "    Remove return types from functions"
	sed -i 's/) -> .*`/)`/g' $f
	
	# Clean up strings produced by SWIG
	
	echo "    Remove return types from functions"
	sed -i 's/) -> \".*\":/):/g' $f
	
	echo "    Remove parameter types from function definitions"
	sed -i 's/: \".*\"):/):/g' $f
	
done

