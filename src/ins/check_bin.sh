#!/bin/bash

# enter source
cd ..

# check for bin directory
echo "Checking for binary directory..."
if [ -d "bin" ]
then
  echo "Binary directory found..."
else
  echo "Binary directory not found..."
  mkdir bin
fi

# back to ins
cd ins
