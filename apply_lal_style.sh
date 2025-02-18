#!/bin/bash

cd lal
find . -iname '*.hpp' -o -iname '*.cpp' | xargs /usr/bin/clang-format-19 -i
cd ..
