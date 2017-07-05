// Christopher Iliffe Sprague
// https://cisprague.github.io
// cisprague@ac.jaxa.jp

#include <iostream>
#include "../cor/fika.hpp"

int main(void) {

  // load kernels
  spc::ld_krnl("../dta/de430.bsp");    // for states
  spc::ld_krnl("../dta/PCK00008.tpc"); // for radii
  spc::ld_krnl("../dta/gm_de431.tpc"); // for gravity

  // create planet
  plnt earth("EARTH");

  // print details
  earth.print();

  // compute states
  double et0 = 100;
  double et;
  for (int i=0; i<100; i++) {
    et = et0 + i*3600;
    earth.eph(et, "Moon");
    // message
    cout << "Time: " << et << endl;
    cout << earth.state.transpose() << endl;

  };
  return 0;
};
