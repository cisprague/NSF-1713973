// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp

#include <iostream>
#include <vector>
#include <string>

#include "../cor/spice.hpp"
#include "../cor/spacecraft.hpp"
#include "../cor/body.hpp"
#include "../cor/linalg.hpp"
#include "../cor/mlp.hpp"
#include "../cor/controller.hpp"
//#include "../cor/phase.hpp"
//#include "../cor/dynamics.hpp"
//#include "../cor/propagator.hpp"


int main(void) {

  // we load the ephemerides
  spice::load_kernels();

  // we create the spacecraft
  const Spacecraft sc(660, 92.3e-3, 3337);

  // we create the planets
  const std::vector<Body> bodies = {Body("Earth"), Body("Sun"), Body("Moon")};

  // we create a phase with segements and a spacecraft
  //Phase phase(20, sc, bodies);

  // we create an initial and final time
  double t0 = spice::mjd2000("1/1/2019 00:00:00.000");
  double tN = spice::mjd2000("1/1/2024 00:00:00.000");

  // we create an initial state at the L2 point 392
  std::vector<double> x0(spice::state(t0, "392"));
  x0.push_back(sc.mass);
  std::vector<double> xN(spice::state(tN, "392"));
  xN.push_back(sc.mass);

  // we define the structure the neural network hidden layers
  const std::vector<int> hshape = {10,10,10};

  // we compute the number of inputs
  const int nbod(bodies.size());
  const int nin(nbod*6 + 1);

  // we generate a normalisation vector
  std::vector<double> ref(nin);
  for (int i=0; i<nbod; ++i) {
    // we compute the initial state of the body
    const std::vector<double> sb(bodies[i].state(t0));
    // we add the relitive spacecraft state
    const std::vector<double> srel(6);
    for (int j=0; j<6; ++j) {ref[i*6 + j] = x0[j] - sb[j];};
  };
  ref[nin-1] = sc.mass;

  // we create a relative neural controller
  Controller::Relative control(bodies, hshape, ref);

  // compute a single control
  linalg::display_vec(control(xN, tN));

  return 0;
};
