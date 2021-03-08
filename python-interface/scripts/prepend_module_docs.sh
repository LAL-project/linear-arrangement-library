#!/bin/bash

function prepend() {
	doc=$1
	file=$2
	
	mv $file $file.tmp
	cat $doc $file.tmp > $file
	rm -f $file.tmp
}

f=$1

if [ ! -f "$f" ]; then
	echo -e "\e[1;1;31m! File\e[0m '$f' \e[1;1;31mdoes not exist\e[0m"
	exit
fi

line=$(head -n 1 $f)
if [ "$line" != "r\"\"\"" ]; then
	
	if [[ $f == *"generate"* ]]; then
		prepend modules_pydocs/generate.py $f
		
	elif [[ $f == *"graphs"* ]]; then
		prepend modules_pydocs/graphs.py $f

	elif [[ $f == *"version"* ]]; then
		prepend modules_pydocs/version.py $f

	elif [[ $f == *"io"* ]]; then
		prepend modules_pydocs/io.py $f

	elif [[ $f == *"iterators"* ]]; then
		prepend modules_pydocs/iterators.py $f

	elif [[ $f == *"linarr"* ]]; then
		prepend modules_pydocs/linarr.py $f

	elif [[ $f == *"numeric"* ]]; then
		prepend modules_pydocs/numeric.py $f

	elif [[ $f == *"properties"* ]]; then
		prepend modules_pydocs/properties.py $f

	elif [[ $f == *"utilities"* ]]; then
		prepend modules_pydocs/utilities.py $f

	elif [ "$f" == "lal.py" ] || [ "$f" == "laldebug.py" ]; then
		prepend modules_pydocs/lal.py $f
		
	fi
fi