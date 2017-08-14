#!/bin/bash

# checking for cmake list
echo "Checking for CMakeLists.txt..."
if [ -f "CMakeLists.txt" ]
then
  echo "CMakeLists.txt found."
else
  echo "CMakeLists.txt not found."
  exit
fi
