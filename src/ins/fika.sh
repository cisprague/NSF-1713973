#!/bin/bash

# make the makefile
cd ../bld
cmake ../ins
make

# execute
./fika
