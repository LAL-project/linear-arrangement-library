#!/bin/bash

for d in lal-release lal-debug lal-release-11.3 lal-debug-11.3 lal-release-12.3 lal-debug-12.3 lal-release-13.1 lal-debug-13.1; do
	cd $d
	make clean
	cd ..
done
