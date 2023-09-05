#!/bin/bash

CMAKE_DIR=/home/soft/cmake-3.22.1/bin

function make_directories_compiler() {
	local compiler=$1

	local suffix=""
	local CXX_compiler=""
	local C_compiler=""
	if [ ! -z $compiler ]; then
		suffix="-$compiler"
		CXX_compiler="-DCMAKE_CXX_COMPILER=/home/soft/gcc-$compiler/bin/g++"
		C_compiler="-DCMAKE_C_COMPILER=/home/soft/gcc-$compiler/bin/gcc"
	else
		CXX_compiler="-DCMAKE_CXX_COMPILER=/home/soft/gcc-11.4.0/bin/g++"
		C_compiler="-DCMAKE_C_COMPILER=/home/soft/gcc-11.4.0/bin/gcc"
	fi
	
	mkdir -p lal-debug$suffix
	cd lal-debug$suffix
	$CMAKE_DIR/cmake $CXX_compiler $C_compiler -DCMAKE_BUILD_TYPE=Debug ../lal
	cd ..

	mkdir -p lal-release$suffix
	cd lal-release$suffix
	$CMAKE_DIR/cmake $CXX_compiler $C_compiler -DCMAKE_BUILD_TYPE=Release ../lal
	cd ..
}

make_directories_compiler
make_directories_compiler 11.3
make_directories_compiler 11.4.0
make_directories_compiler 12.3
make_directories_compiler 13.1
