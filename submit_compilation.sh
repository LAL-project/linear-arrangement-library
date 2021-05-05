#!/bin/bash

./make_makefiles_cluster.sh
sbatch -p short -c 4 compile_cluster.sh lal-release
sbatch -p short -c 4 compile_cluster.sh lal-debug
