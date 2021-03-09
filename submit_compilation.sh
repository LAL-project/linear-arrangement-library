#!/bin/bash

sbatch -p short -c 4 compile_cluster.sh lal-release
sbatch -p short -c 4 compile_cluster.sh lal-debug
