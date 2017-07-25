// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp

#include <iostream>
#include <vector>
#include <string>

#include "../cor/spice.hpp"
#include "../cor/spacecraft.hpp"
#include "../cor/body.hpp"
#include "../cor/linalg.hpp"
#include "../cor/phase.hpp"
#include "../cor/dynamics.hpp"
#include "../cor/propagator.hpp"
#include "../cor/linalg.hpp"
#include "../cor/mlp.hpp"
#include "../cor/controller.hpp"

int main(void) {

  // we load the ephemerides
  spice::load_kernels();

  // we create the spacecraft
  const Spacecraft sc(660, 92.3e-3, 3337);
  //const Spacecraft sc(660, 20, 3337);

  // we create the planets
  const std::vector<Body> bodies = {Body("Earth"), Body("Sun"), Body("Moon")};

  // we create a phase with segements and a spacecraft
  Phase phase(20, sc, bodies);

  // we create an uncontrolled control vector
  std::vector<double> u = linalg::random_unit_vec();
  //std::vector<double> u(3,0);

  // we create an initial and final time
  double t0 = spice::mjd2000("1/1/2019 00:00:00.000");
  double tN = spice::mjd2000("1/1/2022 00:00:00.000");

  // we create an initial state at the L2 point 392
  std::vector<double> x0(spice::state(t0,"1"));
  x0.push_back(sc.mass);
  std::vector<double> xN(spice::state(tN, "392"));
  xN.push_back(sc.mass);

  // we propagate the phase dynamics
  propagator::Results results1 = phase.propagate(x0, u, t0, tN, 0.01, false, 1e-14, 1e-14);
  propagator::Results results2 = phase.propagate(xN, u, tN, t0, -0.01, false, 1e-14, 1e-14);
  //const std::vector<propagator::Results> res = {results1, results2};
  //phase.plot_traj(res, "SSB");

  // we define the structure the neural network hidden layers
  //const std::vector<int> shape = {7,200,200,200,10,3};
  //const std::vector<double> v = {0,1,25};

  //Controller control(sc.thrust, shape, x0);
  //linalg::display_vec(control(x0));
  //std::cout << linalg::norm(control(x0)) << std::endl;

  return 0;
};
