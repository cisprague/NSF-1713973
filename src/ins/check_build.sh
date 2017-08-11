#!/bin/bash

# go to src directory
cd ..

# does build directory exist?
if [ -d "bld" ]
then
  echo "Build directory found..."
else
  echo "Build directory not found..."
  echo "Creating build directory..."
  mkdir bld
fi

# return to ins
cd ins
