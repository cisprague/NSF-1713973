#!/bin/bash

# enter data directory
cd ../req/cspice

# check spice
if [ ! -d "kernels" ]
then
  echo "spice kernels directory not found, creating directory..."
  mkdir spice
else
  echo "spice kernels directory found..."
fi

# move to directory
cd kernels

# download some files

if [ ! -f "L2.bsp" ]
then
  echo "downloading L2 kernels..."
  wget -O L2.bsp https://naif.jpl.nasa.gov/pub/naif/generic_kernels/spk/lagrange_point/L2_de431.bsp
else
  echo "found L2 kernels..."
fi

if [ ! -f "DE.bsp" ]
then
  echo "downloading DE kernels..."
  wget -O DE.bsp https://naif.jpl.nasa.gov/pub/naif/generic_kernels/spk/planets/de430.bsp
else
  echo "found DE kernels..."
fi

if [ ! -f "GM.tpc" ]
then
  echo "downloading GM kernels..."
  wget -O GM.tpc https://naif.jpl.nasa.gov/pub/naif/generic_kernels/pck/gm_de431.tpc
else
  echo "found GM kernels..."
fi

if [ ! -f "LS.tls" ]
then
  echo "downloading LS kernels..."
  wget -O LS.tls https://naif.jpl.nasa.gov/pub/naif/generic_kernels/lsk/naif0011.tls
else
  echo "found LS kernels..."
fi

if [ ! -f "PCK.tpc" ]
then
  echo "downloading PCK kernels..."
  wget -O PCK.tpc https://naif.jpl.nasa.gov/pub/naif/generic_kernels/pck/pck00010.tpc
else
  echo "found PCK kernels..."
fi

if [ ! -f "ORMS.bsp" ]
then
  echo "downloading ORMS kernels..."
  wget -O ORMS.bsp https://naif.jpl.nasa.gov/pub/naif/SMART1/kernels/spk/ORMS__041111020517_00206.BSP
else
  echo "found ORMS kernels..."
fi
