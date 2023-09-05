#!/bin/bash

./make_makefiles_cluster.sh
sbatch -p short -c 8 compile_cluster.sh lal-release
sbatch -p short -c 8 compile_cluster.sh lal-debug
sbatch -p short -c 8 compile_cluster.sh lal-release-11.3
sbatch -p short -c 8 compile_cluster.sh lal-debug-11.3
sbatch -p short -c 8 compile_cluster.sh lal-release-11.4.0
sbatch -p short -c 8 compile_cluster.sh lal-debug-11.4.0
sbatch -p short -c 8 compile_cluster.sh lal-release-12.3
sbatch -p short -c 8 compile_cluster.sh lal-debug-12.3
sbatch -p short -c 8 compile_cluster.sh lal-release-13.1
sbatch -p short -c 8 compile_cluster.sh lal-debug-13.1
