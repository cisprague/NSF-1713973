// Christopher Iliffe Sprague
// cisprague.ac.jaxa.jp

#include <vector>
#include <iostream>
#include "../cor/spacecraft.hpp"
#include "../cor/body.hpp"
#include "../cor/phase.hpp"
using namespace std;

int main(void) {
  load_kernels();
  Spacecraft sc(400, 400, 400);
  Body earth("Earth");
  Body moon("Moon");
  Body sun("Sun");
  Phase phase(20, sc);

  // make up state and control
  Spacecraft::Control cont = Spacecraft::Control::Random().normalized();
  Spacecraft::State st;
  st << earth.eph(4000), sc.mass;
  double t = 400;
  Spacecraft::State dxdt = Spacecraft::State::Zero();

  // use integrator
  //propogate(st, cont, t);
  phase.add_body(earth);
  phase.add_body(moon);
  phase.add_body(sun);
  phase.propagate(st, cont, 0, 50, 0.1);
  //phase.motion(st, cont, dxdt, t);

  /*
  phase.set_random_controls();
  for (int i=0; i<phase.nseg; i++) {
    std::cout << phase.controls[i].transpose() << std::endl;
    std::cout << phase.controls[i].norm() << std::endl;
  };
  */
  return 0;
};
