#include <iostream>
#include "../core/propagator.hpp"
#include "../core/dynamics.hpp"
#include "../core/controller.hpp"
#include "../core/body.hpp"
#include "../core/spacecraft.hpp"
#include "../core/spice.hpp"

int main (void) {

  // load kernels
  spice::load_kernels();

  // spacecraft
  const Spacecraft spacecraft(660, 0.0923, 3337);

  // bodies
  const std::vector<Body> bodies{Body("Earth"), Body("Sun"), Body("Moon")};

  // hidden layer shape
  const std::vector<int> hshape{10, 10, 10};

  // controller
  Controller controller(hshape, spacecraft, bodies);

  // dynamics
  Dynamics dynamics(spacecraft, bodies, controller);

  std::cout << "Testing constructor...\n" << std::endl;
  Propagator propagator(dynamics);

  // times
  const std::string t0s("11/12/2004 00:00:00.000");
  const std::string tfs("11/12/2005 00:00:00.000");
  const double t0(spice::mjd2000(t0s));
  const double tf(spice::mjd2000(tfs));

  // states
  std::vector<double> x0(spice::state(t0, "1"));
  x0.push_back(spacecraft.mass);

  std::cout << "Testing state propagator..." << std::endl;
  std::cout << "Using sample state vector:" << std::endl;
  std::cout << "[ ";
  for (int i=0; i<7; ++i) {
    std::cout << x0.at(i) << " ";
  };
  std::cout << " ]" << std::endl;
  std::cout << "Time: " << t0s << std::endl;

  propagator(x0, t0, tf, 0.001, 1e-13, 1e-13, false);

  std::cout << "New state vector:" <<std::endl;
  std::cout << "[ ";
  for (int i=0; i<7; ++i) {
    std::cout << propagator.states[i].back() << " ";
  };
  std::cout << "]" << std::endl;
  std::cout << "Time: " << tfs << std::endl;

  return 0;
};
