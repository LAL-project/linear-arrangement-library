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
	sed -i 's/: \"bool\"//g' $f
	sed -i 's/: \"uint32_t\"//g' $f
	sed -i 's/: \"lal::node\"//g' $f
	sed -i 's/: \"free_tree\"//g' $f
	sed -i 's/: \"rooted_tree\"//g' $f
	sed -i 's/: \"undirected_graph\"//g' $f
	sed -i 's/: \"directed_graph\"//g' $f
	sed -i 's/: \"std::vector< .* > const &\"//g' $f
done

