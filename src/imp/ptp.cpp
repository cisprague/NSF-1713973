// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp

#include <iostream>
#include <vector>
#include <string>
#include "../cor/spice.hpp"
#include "../cor/spacecraft.hpp"
#include "../cor/body.hpp"
#include "../cor/vectools.hpp"
#include "../cor/phase.hpp"
#include "../cor/dynamics.hpp"
#include "../cor/propagator.hpp"
#include "../cor/mlp.hpp"

int main(void) {

  // we load the ephemerides
  spice::load_kernels();

  // we create the spacecraft
  const Spacecraft sc(660, 92.3e-3, 3337);

  // we create the planets
  std::vector<Body> bodies = {Body("Earth"), Body("Sun"), Body("Moon")};

  // we create a phase with segements and a spacecraft
  Phase phase(20, sc, bodies);

  // we create an uncontrolled control vector
  //std::vector<double> u = vectools::random_unit_vec();
  std::vector<double> u(3,0);

  // we create an initial and final time
  double t0 = spice::mjd2000("1/1/2019 00:00:00.000");
  double tN = spice::mjd2000("1/1/2023 00:00:00.000");

  // we create an initial state at the L2 point 392
  std::vector<double> x0 = spice::state(t0,"392");
  x0.push_back(sc.mass);

  // we propagate the phase dynamics
  //propagator::Results results = phase.propagate(x0, u, t0, tN, 0.0001, false, 1e-14, 1e-14);
  //phase.plot_traj(results, "Earth");

  // we define the structure the neural network hidden layers
  const std::vector<int> shape = {5,5};

  // we create a neural netwokr with 7 inputs and 3 outputs
  ML::MLP mlp(shape, 7, 3);
  std::cout << mlp.w.size() << std::endl;

  return 0;
};
