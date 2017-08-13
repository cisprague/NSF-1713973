#include <iostream>
#include "../cor/dynamics.hpp"
#include "../cor/controller.hpp"
#include "../cor/body.hpp"
#include "../cor/spacecraft.hpp"
#include "../cor/spice.hpp"

int main (void) {

  // load kernels
  spice::load_kernels();

  // spacecraft
  const Spacecraft spacecraft(660, 0.0923, 3337);

  // bodies
  const std::vector<Body> bodies{Body("Earth")};

  // hidden layer shape
  const std::vector<int> hshape{10, 10, 10};

  // controller
  Controller controller(hshape, spacecraft, bodies);

  std::cout << "Testing constructor...\n" << std::endl;
  Dynamics dynamics(spacecraft, bodies, controller);

  // times
  const double t0(spice::mjd2000("11/12/2004 00:00:00.000"));

  // states
  std::vector<double> x0(spice::state(t0, "1"));
  x0.push_back(spacecraft.mass);

  std::cout << "Testing state dynamics..." << std::endl;
  std::cout << "Using sample state vector:" << std::endl;
  std::cout << "[ ";
  for (int i=0; i<7; ++i) {
    std::cout << x0.at(i) << " ";
  };
  std::cout << " ]" << std::endl;
  std::cout << "Time: " << t0 << std::endl;

  std::vector<double> dxdt(7, 0);
  std::cout << "dxdt = [ ";
  for (int i=0; i<7; ++i) {
    std::cout << dxdt.at(i) << " ";
  };
  std::cout << "]" << std::endl;

  dynamics(x0, dxdt, t0);

  std::cout << "dxdt = [ ";
  for (int i=0; i<7; ++i) {
    std::cout << dxdt.at(i) << " ";
  };
  std::cout << "]" << std::endl;





  return 0;
};
