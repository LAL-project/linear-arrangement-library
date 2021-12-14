#!/bin/bash

echo "Generating Doxygen (html) documentation of the C++ code..."
doxygen doxyconfig/doxyfile | sed "s/^/    /g"

