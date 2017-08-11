#include <iostream>
#include "../core/constants.hpp"

int main (void) {

  std::cout << "Average distance of Earth from the Sun (1 AU):\n";
  std::cout << R_EARTH << " [m]"  << std::endl;

  std::cout << "Average orbital velocity of Earth:\n";
  std::cout << V_EARTH << " [m/s]"  << std::endl;

  std::cout << "Average centripetal acceleration of Earth:\n";
  std::cout << A_EARTH << " [m/s^2]"  << std::endl;

  std::cout << "Sea-level gravitational acceleration of Earth:\n";
  std::cout << A_EARTH << " [m/s^2]" << std::endl;

  return 0;
};
