#!/bin/bash

# build tests
./build_tests.sh

# go to test binaries
cd ../bin/tst

# execute
echo
echo "Testing constants..."
echo
./constants

echo
echo "Testing spacecraft..."
echo
./spacecraft

echo
echo "Testing CSPICE..."
echo
./spice

echo
echo "Testing Body..."
echo
./body

echo
echo "Testing Body..."
echo
./phase
