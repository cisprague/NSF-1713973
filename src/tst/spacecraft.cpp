#include <iostream>
#include "../core/spacecraft.hpp"

int main (void) {

  const double mass(660);
  const double thrust(0.0923);
  const double isp(3337);

  std::cout << "Making spacecraft with constants:\n";
  std::cout << "mass = " << mass << " [kg]\n";
  std::cout << "thrust = " << thrust << " [N]\n";
  std::cout << "isp = " << isp << " [s]\n" << std::endl;

  Spacecraft sc(mass, thrust, isp);
  std::cout << "Derrived effective velocity is:\n";
  std::cout << "veff = " << sc.veff << " [m/s]\n" << std::endl;

  std::cout << "Create a sample control vector:\n";
  const std::vector<double> u{1,0,0};
  std::cout << "[ " << u.at(0) << " " << u.at(1) << " " << u.at(2) << " ]\n";
  std::cout << std::endl;

  std::cout << "Computing some things:\n";
  std::cout << "force vector = ";
  const std::vector<double> f(sc.force(u));
  std::cout << "[ " << f.at(0) << " " << f.at(1) << " " << f.at(2) << " ]";
  std::cout << " [N]\n";
  std::cout << "force magnitude = " << sc.fmag(u) << " [N]\n";
  std::cout << "mass flow rate = " << sc.mdot(u) << " [kg/s]\n";
  std::cout << std::endl;

  return 0;
};
