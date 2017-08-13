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
echo "Testing body..."
echo
./body

echo
echo "Testing controller..."
echo
./controller

echo
echo "Testing dynamics..."
echo
./dynamics

echo
echo "Testing propagator..."
echo
./propagator

echo
echo "Testing phase..."
echo
./phase
