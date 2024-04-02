#!/bin/bash

./make_makefiles_cluster.sh
echo "release: g++ system-wide "
sbatch -p short -c 8 compile_cluster.sh lal-release
echo "debug: g++ system-wide"
sbatch -p short -c 8 compile_cluster.sh lal-debug

echo "release: g++ 11.3"
sbatch -p short -c 8 compile_cluster.sh lal-release-11.3
echo "debug: g++ 11.3"
sbatch -p short -c 8 compile_cluster.sh lal-debug-11.3

echo "release: g++ 11.4.0"
sbatch -p short -c 8 compile_cluster.sh lal-release-11.4.0
echo "debug: g++ 11.4.0"
sbatch -p short -c 8 compile_cluster.sh lal-debug-11.4.0

echo "release: g++ 12.3"
sbatch -p short -c 8 compile_cluster.sh lal-release-12.3
echo "debug: g++ 12.3"
sbatch -p short -c 8 compile_cluster.sh lal-debug-12.3

echo "release: g++ 13.1"
sbatch -p short -c 8 compile_cluster.sh lal-release-13.1
echo "debug: g++ 13.1"
sbatch -p short -c 8 compile_cluster.sh lal-debug-13.1
