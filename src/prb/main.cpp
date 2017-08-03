// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp

// problems

#include <iostream>
#include "ptp.hpp"
#include "../cor/linalg.hpp"

int main(void) {

  // instantiate the problem
  PTP prob("/home/cisprague/Documents/Dev/NSF-1713973/src/prb/ptp.yaml");
  linalg::display_vec(prob.get_decision());

  return 0;
};
