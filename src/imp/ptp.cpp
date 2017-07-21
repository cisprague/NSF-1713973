// Christopher Iliffe Sprague
// cisprague.ac.jaxa.jp

#include <vector>
#include <iostream>
#include "../cor/spacecraft.hpp"
#include "../cor/body.hpp"
#include "../cor/phase.hpp"
using namespace std;

int main(void) {

  // we load the ephemerides
  load_kernels();

  // we create the spacecraft
  Spacecraft sc(660, 92.3e-3, 3337);

  // we create the phase with segements and the spacecraft
  Phase phase(20, sc);

  // we set the initial and final times
  phase.set_times(4000, 4010);

  // we create the planets
  Body earth("Earth");
  Body moon("Moon");
  Body sun("Sun");

  // we make apparent the planets within the phase
  phase.add_body(earth);
  phase.add_body(moon);
  phase.add_body(sun);

  // we make an arbitrary maximum throttle control
  Spacecraft::Control control = Spacecraft::Control::Random().normalized();

  // we compute a random circular LEO around Earth at a time
  Spacecraft::State state;
  state << random_orbit(earth, 4000), sc.mass;

  // we propagate the controlled dynamics in the phase
  phase.propagate(state, control, phase.t0, phase.tN, 0.00001);


  return 0;
};
