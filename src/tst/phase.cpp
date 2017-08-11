#include <iostream>
#include <string>
#include "../core/phase.hpp"
#include "../core/spacecraft.hpp"
#include "../core/body.hpp"
#include "../core/spice.hpp"

int main (void) {
  spice::load_kernels();

  const std::string n1("Earth"), n2("Sun");
  const double mass(660), thrust(0.0923), isp(3337);
  const std::string d1("11/12/2004 00:00:00.000");
  const std::string d2("11/12/2010 00:00:00.000");

  std::cout << "Consider a phase from one body to another:\n";
  std::cout << "mass = 660 [kg], thrust = 0.0923 [N], isp = 3337 [s]\n";
  std::cout << std::endl;

  std::cout << "Testing constructor 1..." << std::endl;
  Phase p1(Spacecraft(mass, thrust, isp), {Body(n1), Body(n2)});


  return 0;
};
