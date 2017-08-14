#!/bin/bash
echo

# define problems
problems=("ptp")

# enter source
cd ..

# check for prb
echo "Checking for problems source..."
if [ -d "prb" ]
then
  echo "Found problems source."
  # enter prb source
  cd prb
else
  echo "Problems source not found."
  exit
fi

# check for cmake
./../ins/check_cmake.sh

# check for source
echo "Checking for problem source files..."
for prob in "${problems[@]}"
do
  echo "Checking for $prob.cpp..."
  if [ -f "$prob.cpp" ]
  then
    echo "Found $prob.cpp."
  else
    echo "Could not find $prob.cpp"
    exit
  fi
done


# back to ins
cd ../ins

# check for build directory
./check_build.sh

# enter build
cd ../bld

# check for problem build
echo "Checking for problems build directory..."
if [ -d "prb" ]
then
  echo "Problem build directory found."
else
  echo "Problem build directory not found."
  echo "Creating problem build directory..."
  mkdir prb
fi

# go to git
cd ../..

# check for configuration directory
echo "Checking for user problem configuration directory..."
if [ -d "usr" ]
then
  echo "User problem configuration directory found."
else
  echo "User problem configuration directory not found."
  exit
fi

# check for configurations
cd usr
echo "Checking for problem configuration files..."
for prob in "${problems[@]}"
do
  echo "Checking for $prob.yaml..."
  if [ -f "$prob.yaml" ]
  then
    echo "Found $prob.yaml."
  else
    echo "Could not find $prob.yaml"
    exit
  fi
done

# check configuration scripts

echo "Problems infrastructure is sufficient."

# go back to ins
cd ../src/ins

echo
