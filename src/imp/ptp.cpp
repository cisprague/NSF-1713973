// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp

#include <iostream>
#include <vector>
#include <string>

#include "../cor/spice.hpp"
#include "../cor/spacecraft.hpp"
#include "../cor/body.hpp"
#include "../cor/linalg.hpp"
//#include "../cor/phase.hpp"
#include "../cor/dynamics.hpp"
//#include "../cor/propagator.hpp"
//#include "../cor/linalg.hpp"
//#include "../cor/mlp.hpp"
//#include "../cor/controller.hpp"

int main(void) {

  // we load the ephemerides
  spice::load_kernels();

  // we create the spacecraft
  const Spacecraft sc(660, 92.3e-3, 3337);
  //const Spacecraft sc(660, 20, 3337);

  // we create the planets
  std::vector<Body> bodies = {Body("Earth"), Body("Sun"), Body("Moon")};

  // we create a phase with segements and a spacecraft
  //Phase phase(20, sc, bodies);

  // we create an uncontrolled control vector
  std::vector<double> u = linalg::random_unit_vec();
  //std::vector<double> u(3,0);

  // we create an initial and final time
  double tn = spice::mjd2000("1/1/2018 00:00:00.000");
  double t0 = spice::mjd2000("1/1/2019 00:00:00.000");
  double tN = spice::mjd2000("1/1/2020 00:00:00.000");

  // we create an initial state at the L2 point 392
  std::vector<double> x0 = spice::state(t0,"1");
  x0.push_back(sc.mass);

  Dynamics::Uncontrolled dyn(bodies);
  std::vector<double> dxdt(7);
  dyn(x0, dxdt, t0);
  linalg::display_vec(dxdt);


  // we propagate the phase dynamics
  //propagator::Results results1 = phase.propagate(x0, u, t0, tN, 0.01, false, 1e-12, 1e-12);
  //phase.plot_traj(results1, "SSB");

  // we define the structure the neural network hidden layers
  //const std::vector<int> shape = {7,200,200,200,10,3};
  //const std::vector<double> v = {0,1,25};

  //Controller control(sc.thrust, shape, x0);
  //linalg::display_vec(control(x0));
  //std::cout << linalg::norm(control(x0)) << std::endl;

  return 0;
};
