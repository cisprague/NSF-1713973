#include <iostream>
#include <string>
#include "../cor/body.hpp"
#include "../cor/spice.hpp"

int main (void) {

  spice::load_kernels();

  const std::string n("Earth");

  std::cout << "Consider the body: " << n << std::endl;

  const Body b(n);

  std::cout << "After initialising the body by name:\n";
  std::cout << "name = " << b.name << "\n";
  std::cout << "NAIF ID = " << b.id << "\n";
  std::cout << "radius = " << b.radius << "\n" << std::endl;

  const std::string date("11/12/2004 00:00:00.000");
  const double t(spice::mjd2000(date));
  const std::vector<double> s(b.state(t));

  std::cout << "The state of the body at the date:\n";
  std::cout << date << "\n";
  std::cout << "and median Julian date past 2000:\n";
  std::cout << t << "\n";
  std::cout << "state = " << "[ ";
  for (int i=0; i<6; ++i) {
    std::cout << s.at(i) << " ";
  };
  std::cout << "] [m,m,m,m/s,m/s,m/s,kg]\n" << std::endl;

  return 0;
};
