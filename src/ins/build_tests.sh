#!/bin/bash

# check tst infrastructure
echo "Checking for test infrastructure..."
./check_tests.sh

# remove binaries
echo "Removing existing binaries..."
rm ../bin/tst/*

# enter build directory
cd ../bld/tst

# build the programme
echo "Building tests..."
cmake ../../tst
make

# back to ins
cd ../../ins
