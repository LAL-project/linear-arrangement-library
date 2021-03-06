#!/bin/bash

for f in $@; do
	echo $f
	
	echo "    Remove return types from function definitions"
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
	
	# From '::' to '.' for a more Python-looking code.
	# This can't be applied to the documentation.i because that would
	# change the definitions of %feature definitions, like so
	#
	#	from
	#
	#		%feature("docstring") lal::generate::all_lab_rooted_trees 
	#
	#	to
	#
	#		%feature("docstring") lal.generate.all_lab_rooted_trees 
	#
	
	echo "Replace '::' with '.'"
	sed -i 's/::/./g' $D
done

