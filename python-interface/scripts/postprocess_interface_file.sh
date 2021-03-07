#!/bin/bash

echo -e "\e[1;1;34m* Clean up interface file\e[0m"
scripts/clean_up_interfaces.sh $1 | sed 's/^/    /g'
echo -e "\e[1;1;34m* Prepend documentation\e[0m"
scripts/prepend_module_docs.sh $1 | sed 's/^/    /g'
