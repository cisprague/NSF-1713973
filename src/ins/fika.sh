#!/bin/bash

# make the makefile
cd ../bld
cmake -G Ninja ../ins
make

# execute
./ptp
