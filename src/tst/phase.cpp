#include <iostream>
#include <string>
#include "../core/phase.hpp"
#include "../core/spacecraft.hpp"
#include "../core/body.hpp"
#include "../core/spice.hpp"

int main (void) {

  // load kernels
  spice::load_kernels();

  // spacecraft
  const Spacecraft spacecraft(660, 0.0923, 3337);

  // bodies
  const std::vector<Body> bodies{Body("Earth"), Body("Sun")};

  // times
  const double t0(spice::mjd2000("11/12/2004 00:00:00.000"));
  const double tf(spice::mjd2000("11/12/2010 00:00:00.000"));

  // states
  std::vector<double> x0(spice::state(t0, "1"));
  x0.push_back(spacecraft.mass);
  std::vector<double> xf(spice::state(tf, "392"));
  xf.push_back(10);

  // phase
  std::cout << "Testing constructor...";
  Phase phase(spacecraft, bodies, t0, tf, x0, xf);

  std::cout << "The initial state and time:\n";
  std::cout << "x0 = [ ";
  for (int i=0; i<7; ++i) {
    std::cout << phase.x0.at(i) << " ";
  };
  std::cout << " ]\n";
  std::cout << "t0 = " << phase.t0 << std::endl;

  std::cout << "The final state and time:\n";
  std::cout << "xf = [ ";
  for (int i=0; i<7; ++i) {
    std::cout << phase.xf.at(i) << " ";
  };
  std::cout << " ]\n";
  std::cout << "tf = " << phase.tf << std::endl;

  std::cout << "Testing setter..." << std::endl;
  phase.set({0,0,0,0,0,0,0},{0,0,0,0,0,0,0},0,0);

  std::cout << "The initial state and time:\n";
  std::cout << "x0 = [ ";
  for (int i=0; i<7; ++i) {
    std::cout << phase.x0.at(i) << " ";
  };
  std::cout << " ]\n";
  std::cout << "t0 = " << phase.t0 << std::endl;

  std::cout << "The initial state and time:\n";
  std::cout << "xf = [ ";
  for (int i=0; i<7; ++i) {
    std::cout << phase.xf.at(i) << " ";
  };
  std::cout << " ]\n";
  std::cout << "tf = " << phase.tf << "\n" << std::endl;

  std::cout << "Testing mismatch constraint..." << std::endl;
  phase.set(x0, xf, t0, tf);
  std::cout << "The initial state and time:\n";
  std::cout << "x0 = [ ";
  for (int i=0; i<7; ++i) {
    std::cout << phase.x0.at(i) << " ";
  };
  std::cout << " ]\n";
  std::cout << "t0 = " << phase.t0 << std::endl;

  std::cout << "The final state and time:\n";
  std::cout << "xf = [ ";
  for (int i=0; i<7; ++i) {
    std::cout << phase.xf.at(i) << " ";
  };
  std::cout << " ]\n";
  std::cout << "tf = " << phase.tf << std::endl;
  std::cout << "constructing controller..." << std::endl;
  Controller controller({10, 10, 10}, phase.spacecraft, phase.bodies);
  std::cout << "computing mismatch with controller..." << std::endl;
  const std::vector<double> ceq(phase.mismatch(controller, 1e-3, 1e-12, 1e-12, true, true));

  std::cout << "The mismatch vector:\n";
  std::cout << "[ ";
  for (int i=0; i<7; ++i) {
    std::cout << ceq[i] << " ";
  };
  std::cout << "]" << std::endl;


  return 0;
};
