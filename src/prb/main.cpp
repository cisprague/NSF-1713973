// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp

// problems

#include <iostream>
#include "ptp.hpp"
#include "../cor/phase.hpp"

int main(void) {

  PTP prob("/home/cisprague/Documents/Dev/NSF-1713973/src/prb/ptp.yaml");

  std::cout << prob.phase.controller.mlp.ni << std::endl;

  return 0;
};
