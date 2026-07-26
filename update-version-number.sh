#!/bin/bash

newvernum=$1
if [ -z $newvernum ]; then
    echo "New version number was not provided"
    exit
fi

echo "Updating version number to $newvernum"

sed -i "s/99.99/$newvernum/g" lal/lal_version.hpp
sed -i "s/99/00/g" lal/lal_version.hpp

sed -i "s/set(LAL_MAJOR 99.99)/set(LAL_MAJOR $newvernum)/g" CMakeLists.txt
sed -i "s/set(LAL_PATCH 99)/set(LAL_PATCH 00)/g" CMakeLists.txt