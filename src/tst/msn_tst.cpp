// Christopher Iliffe Sprague
// https://cisprague.github.io
// cisprague@ac.jaxa.jp

#include <iostream>
#include <vector>

#include "../cor/spc.hpp"
#include "../cor/pnt.hpp"
#include "../cor/phs.hpp"
#include "../cor/msn.hpp"

int main(void) {

  // load kernels
  spc::ld_krnl("../dta/de430.bsp");    // for states
  spc::ld_krnl("../dta/pck00008.tpc"); // for radii
  spc::ld_krnl("../dta/pck00010.tpc"); // for radii
  spc::ld_krnl("../dta/gm_de431.tpc"); // for gravity

  // create environment
  Body earth("Earth");
  Body moon("Moon");
  // create spacecraft
  Spacecraft sc(400,423,423);
  // create legs
  Phase l1(sc, Spacecraft::State::Random(), Spacecraft::State::Random(), 0, 100);
  Phase l2(sc, Spacecraft::State::Random(), Spacecraft::State::Random(), 0, 100);

  return 0;
};
