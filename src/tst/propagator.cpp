#include <iostream>
#include "../core/propagator.hpp"
#include "../core/dynamics.hpp"
#include "../core/controller.hpp"
#include "../core/body.hpp"
#include "../core/spacecraft.hpp"
#include "../core/spice.hpp"

int main (void) {

  // spacecraft
  const Spacecraft spacecraft(660, 0.0923, 3337);

  // bodies
  const std::vector<Body> bodies{Body("Earth")};

  // hidden layer shape
  const std::vector<int> hshape{10, 10, 10};

  // controller
  Controller controller(hshape, spacecraft, bodies);

  // dynamics
  Dynamics dynamics(spacecraft, bodies, controller);

  std::cout << "Testing constructor...\n" << std::endl;
  Propagator propagator(dynamics);

  // times
  const double t0(spice::mjd2000("11/12/2004 00:00:00.000"));
  const double tf(spice::mjd2000("11/12/2006 00:00:00.000"));

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
  std::cout << "Time: " << t0 << std::endl;

  propagator(x0, t0, tf, 0.001);

  return 0;
};
