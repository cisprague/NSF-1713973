#!/bin/bash

# enter data directory
cd ../dta

# check spice
if [ ! -d "spice" ]
then
  echo "spice directory not found, creating directory..."
  mkdir spice
else
  echo "spice directory found..."
fi

# move to directory
cd cspice

# download some files
echo "Downloading spice kernels..."

if [ ! -f "L2.bsp" ]
then
  wget -O L2.bsp https://naif.jpl.nasa.gov/pub/naif/generic_kernels/spk/lagrange_point/L2_de431.bsp
fi

if [ ! -f "DE.bsp" ]
then
  wget -O DE.bsp https://naif.jpl.nasa.gov/pub/naif/generic_kernels/spk/planets/de430.bsp
fi

if [ ! -f "GM.tpc" ]
then
  wget -O GM.tpc https://naif.jpl.nasa.gov/pub/naif/generic_kernels/pck/gm_de431.tpc
fi

if [ ! -f "LS.tls" ]
then
  wget -O LS.tls https://naif.jpl.nasa.gov/pub/naif/generic_kernels/lsk/naif0011.tls
fi

if [ ! -f "PCK.tpc" ]
then
  wget -O PCK.tpc https://naif.jpl.nasa.gov/pub/naif/generic_kernels/pck/pck00010.tpc
fi

if [ ! -f "ORMS.bsp" ]
then
  wget -O ORMS.bsp https://naif.jpl.nasa.gov/pub/naif/SMART1/kernels/spk/ORMS__041111020517_00206.BSP
fi
