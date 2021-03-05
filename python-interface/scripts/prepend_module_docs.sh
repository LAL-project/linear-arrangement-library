#!/bin/bash

function process_file() {
	f=$1
	filename=${f##*/}
	echo "    filename:" $filename
	
	mv $f $f.tmp
	
	if [[ $filename == *"generate"* ]]; then
		cat namespaces/generate.py $f.tmp > $f
		
	elif [[ $filename == *"graphs"* ]]; then
		cat namespaces/graphs.py $f.tmp > $f
	
	elif [[ $filename == *"version"* ]]; then
		cat namespaces/version.py $f.tmp > $f
	
	elif [[ $filename == *"io"* ]]; then
		cat namespaces/io.py $f.tmp > $f
	
	elif [[ $filename == *"iterators"* ]]; then
		cat namespaces/iterators.py $f.tmp > $f
	
	elif [[ $filename == *"linarr"* ]]; then
		cat namespaces/linarr.py $f.tmp > $f
	
	elif [[ $filename == *"numeric"* ]]; then
		cat namespaces/numeric.py $f.tmp > $f
	
	elif [[ $filename == *"properties"* ]]; then
		cat namespaces/properties.py $f.tmp > $f
	
	elif [[ $filename == *"utilities"* ]]; then
		cat namespaces/utilities.py $f.tmp > $f
	
	elif [ "$filename" == "lal.py" ] || [ "$filename" == "laldebug.py" ]; then
		cat namespaces/lal.py $f.tmp > $f
	fi
	
	rm $f.tmp
}

for f in $@; do
	echo $f
	
	line=$(head -n 1 $f)
	
	if [ "$line" != "r\"\"\"" ]; then
		process_file $f
	fi
	
done
