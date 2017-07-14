// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp

#include "../cor/fika.hpp"

namespace Mission {

  void PTP_exec(void) {
    // load kernels
    Spice::load_kernels();
    // instantiate bodies
    Body earth("EARTH");
    Body moon("MOON");
    // instantiate spacecraft
    Spacecraft sc(400,400,400);
    // instantiate problem
    PTP prob(sc, earth, moon, 20);
    // compute fitness
    prob.fitness({4,3,4,3});
  };

};

int main(void) {
  Mission::PTP_exec();
  return 0;
};
