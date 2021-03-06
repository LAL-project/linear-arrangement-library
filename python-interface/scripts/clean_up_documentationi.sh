#!/bin/bash

D=submodules/documentation.i

####################################################################

echo "Looking for malformed mathematical expressions..."
python3 scripts/detect_superfluous_spaces_math.py $D | sed "s/^/    /g"
if [ $? -eq 1 ]; then
	echo -e "\e[1;4;31mError:\e[0m Found malformed mathematical expressions"
	echo -e "Rerun the script \e[1;4;31monce you have corrected them\e[0m"
	exit
fi

echo "Format 'Parameters' title appropriately"
sed -E 's/Parameters:/Parameters:|/g' $D | tr '|' '\n' > /tmp/asdf
mv /tmp/asdf $D

echo "Correcting math delimiters"
python3 scripts/correct_math_delimiters.py $D | sed "s/^/    /g"

# Clean up strings produced by 'documentation.i'
# These are found in the documentation of overloaded functions.

echo "Remove return types from functions"
sed -i 's/) -> .*`/)`/g' $D
