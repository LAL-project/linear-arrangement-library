#!/bin/bash

D=modules/documentation.i

####################################################################

echo -e "\e[1;1;33mLooking for malformed mathematical expressions...\e[0m"
python scripts/detect_superfluous_spaces_math.py $D | sed "s/^/    /g"
if [ $? -eq 1 ]; then
	echo -e "\e[1;4;31mError:\e[0m Found malformed mathematical expressions"
	echo -e "Rerun the script \e[1;4;31monce you have corrected them\e[0m"
	exit
fi

echo -e "\e[1;1;33mFormat 'Parameters' title appropriately\e[0m"
sed -E 's/Parameters:/Parameters:ª/g' $D | tr 'ª' '\n' > /tmp/asdf
mv /tmp/asdf $D

echo -e "\e[1;1;33mCorrecting math delimiters\e[0m"
python scripts/correct_math_delimiters.py $D | sed "s/^/    /g"

# Clean up strings produced by 'documentation.i'
# These are found in the documentation of overloaded functions.

echo -e "\e[1;1;33mRemove return types from functions\e[0m"
sed -i 's/) -> .*`/)`/g' $D
