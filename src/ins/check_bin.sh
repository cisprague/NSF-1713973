#!/bin/bash

# enter source
cd ..

# check for bin directory
echo "Checking for binary directory..."
if [ -d "bin" ]
then
  echo "Binary directory found."
else
  echo "Binary directory not found."
  echo "Creating binary directory..."
  mkdir bin
fi

# back to ins
cd ins
