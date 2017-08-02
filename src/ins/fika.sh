#!/bin/bash

# enter the build directory
cd ../bld

# remove previous executable if existing
rm main

# generate makefile
cmake ../ins

# execute the make script
make

# execute the programme
./main
