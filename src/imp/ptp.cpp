// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp

#include <iostream>
#include <vector>
#include <string>
#include "matplotlibcpp.h"
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
  std::vector<Body> bodies = {Body("Earth"), Body("Sun"), Body("Moon"), Body("Mars")};

  // we create a phase with segements and a spacecraft
  Phase phase(20, sc, bodies);

  // we create an uncontrolled control vector
  std::vector<double> u(3,0);

  // we create an initial and final time
  double t0 = spice::mjd2000("1/1/2011 00:00:00.000");
  double tN = spice::mjd2000("1/1/2015 00:00:00.000");

  // we create an initial state at the L2 point
  std::vector<double> x0 = spice::state(t0,"395");
  x0.push_back(sc.mass);

  // we propagate the phase dynamics
  propagator::Results results = phase.propagate(x0, u, t0, tN, 0.1, false);

  // set up for plotting
  namespace plt = matplotlibcpp;
  int nd = results.states.size();
  std::vector<double> x(nd), y(nd);
  for (int i=0; i<results.states.size(); i++) {
    x[i] = results.states[i][0];
    y[i] = results.states[i][1];
  };
  plt::plot(x, y);
  plt::show();

  return 0;
};
