#!/bin/bash

# enter test source
cd ../tst

# check for cmake
echo "Checking for test cmake..."
./../ins/check_cmake.sh

# back to ins
cd ../ins

# bld directory check
./check_build.sh

# enter bld directory
cd ../bld

# does test directory exist?
echo "Checking for test build directory..."
if [ -d "tst" ]
then
  echo "Test directory found..."
else
  echo "Test directory not found..."
  echo "Creating test directory..."
  mkdir tst
fi

# return to ins
cd ../ins

# check for binary directory
./check_bin.sh

# enter binary
cd  ../bin

# check for tst directory
echo "Checking for test binary directory..."
if [ -d "tst" ]
then
  echo "Binary test directory found..."
else
  echo "Binary test directory not found..."
  echo "Creating binary test directory..."
  mkdir tst
fi

# back to ins
cd ../ins
