#!/bin/bash

# move to source
cd ..

# check for bin
echo "Looking for binary directory..."
if [ -d "bin" ]
then
  echo "Binary directory found..."
  echo "Removing binary directory..."
  rm -r bin
else
  echo "Binary directory not found..."
  echo "No directory to remove..."
fi

# back to ins
cd ins
