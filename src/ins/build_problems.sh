#!/bin/bash
echo

# check for infrastructure
echo "Checking problems infrastructure..."
./check_problems.sh

# go to binary directory
cd ../../usr

# check for binaries
echo "Deleting existing problem binaries.."
find . -type f  ! -name "*.*"  -delete

# go back to build
cd ../src/bld/prb

# build the source
echo "Building problems source..."
cmake ../../prb
echo "Problem source built."

# create the binaries
echo "Building binaries..."
make
