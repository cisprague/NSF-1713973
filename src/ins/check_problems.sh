#!/bin/bash

# enter problem source directory
cd ../prb

# check for cmake
echo "Checking for problems cmake.."
./../ins/check_cmake.sh

# back to ins
cd ../ins

# check for build directory
./check_build.sh

# enter build directory
cd ../bld

# check for problem directory
echo "Checking for problem build directory..."
if [ -d "prb" ]
then
  echo "Problem directory found..."
else
  echo "Problem directory not found..."
  echo "Creating problem directory..."
  mkdir prb
fi

# return to ins
cd ../ins

# check for binary directory
./check_bin.sh

# enter binary
cd ../bin

# check for problem binary directory
echo "Checking for problem binary directory..."
if [ -d "prb" ]
then
  echo "Binary problem directory found..."
else
  echo "Binary problem directory not found..."
  echo "Building binary problem directory..."
  mkdir prb
fi

# back to ins
cd ../ins
