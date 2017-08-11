#include <iostream>
#include <string>
#include "../core/spice.hpp"

int main (void) {

  spice::load_kernels();

  const std::string n("Earth");

  std::cout << "Consider the body: " << n << "\n" << std::endl;

  std::cout << "Some info about the body:\n";
  std::cout << "NAIF ID: " << spice::id(n) << "\n";
  std::cout << "gravitational parametre " << spice::mu(n) << " [m^3/s^2]\n";
  std::cout << "radius " << spice::radius(n) << " [m]\n" << std::endl;

  const std::string date("11/12/2004 00:00:00.000");
  std::cout << "Consider the time and date:\n";
  std::cout << date << "\n" << std::endl;

  const double time(spice::mjd2000(date));
  std::cout << "The median julian date past 2000:\n";
  std::cout << time << " [s]\n" << std::endl;

  const std::vector<double> state(spice::state(time, n));
  std::cout << "The body state at this time wrt SSB:\n";
  std::cout << "[ ";
  for (int i=0; i<6; ++i) {
    std::cout << state.at(i) << " ";
  };
  std::cout << "] [m,m,m,m/s,m/s,m/s,kg]\n" << std::endl;


  return 0;
};
