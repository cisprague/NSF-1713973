#!/bin/bash

# build the problems
./build_problems.sh

# go to problem binaries
cd ../../usr

# execute
echo
echo "Executing point to point (PTP) problem..."
echo
./ptp
