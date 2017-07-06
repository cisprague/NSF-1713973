// Christopher Iliffe Sprague
// https://cisprague.github.io
// cisprague@ac.jaxa.jp

#include <iostream>
#include "../cor/fika.hpp"
#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;

int main(void) {

  // load kernels
  spc::ld_krnl("../dta/de430.bsp");    // for states
  spc::ld_krnl("../dta/PCK00008.tpc"); // for radii
  spc::ld_krnl("../dta/gm_de431.tpc"); // for gravity

  // create planet
  plnt earth("EARTH");

  // compute states
  double et0 = 100;
  double et;
  for (int i=0; i<2; i++) {
    et = et0 + i*3600;
    earth.eph(et, "SSB");
    cout << earth.state.transpose() << endl;
  };

  return 0;
};
