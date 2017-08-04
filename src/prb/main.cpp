// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp

// problems
#include "ptp.hpp"
#include "../cor/linalg.hpp"

int main(void) {

  // instantiate the problem
  Problems::PTP prob("/home/cisprague/Documents/Dev/NSF-1713973/src/prb/ptp.yaml");
  prob.phase.plot(0, 1, "Earth");

  return 0;
};
