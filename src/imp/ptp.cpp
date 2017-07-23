// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp

#include <iostream>
#include <vector>

#include "../cor/spice.hpp"
#include "../cor/spacecraft.hpp"
#include "../cor/body.hpp"
#include "../cor/vectools.hpp"
#include "../cor/phase.hpp"
#include "../cor/dynamics.hpp"
#include "../cor/propagator.hpp"

int main(void) {

  // we load the ephemerides
  spice::load_kernels();

  // we create the spacecraft
  const Spacecraft sc(660, 92.3e-3, 3337);

  // we create the planets
  std::vector<Body> bodies = {Body("Earth")};
  std::cout << bodies[0].mu << std::endl;

  // we create a phase with segements and a spacecraft
  Phase phase(20, sc, bodies);

  // we create an uncontrolled control vector
  //std::vector<double> u(3,0);

  // we create an initial and final time
  //double t0 = spice::mjd2000("1/1/2011 00:00:00.000");
  //double tN = spice::mjd2000("1/1/2012 00:00:00.000");

  // we create an initial state at the L2 point
  //std::vector<double> x0 = spice::state(t0,"Moon");
  //x0.push_back(sc.mass);

  // we propagate the phase dynamics
  //phase.propagate(x0, u, t0, tN, 0.001);

  return 0;
};
