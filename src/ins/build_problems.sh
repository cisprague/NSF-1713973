#!/bin/bash

# check prb infrastructure
echo "Checking for problem infrastructure..."
./check_problems.sh

# remove binaries
echo "Removing existing binaries..."
rm ../bin/prb/*

# enter build directory
cd ../bld/prb

# build the programme
echo "Building problems..."
cmake ../../prb
make

# back to ins
cd ../../ins
