#!/bin/bash

# enter source
cd ..

# checking for bin
echo "Checking for build directory..."
if [ -d "bld" ]
then
  echo "Build directory found..."
  echo "Removing build directory..."
  rm -r bld
else
  echo "Build directory not found..."
  echo "No directory to remove..."
fi

# back to ins
cd ins
