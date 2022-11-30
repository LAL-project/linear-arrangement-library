#!/bin/bash

# ----------------------------------------------------------------------
mkdir -p lal-release && cd lal-release
qmake -makefile -o Makefile "LTO=YES" "ENVIR=CLUSTER" ../lal/lal.pro
cd ..

# ----------------------------------------------------------------------
mkdir -p lal-debug && cd lal-debug
qmake -makefile -o Makefile "LTO=YES" "ENVIR=CLUSTER" CONFIG+=debug ../lal/lal.pro
cd ..
