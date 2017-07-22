// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp

#include <iostream>

#include "../cor/spice.hpp"
#include "../cor/spacecraft.hpp"
#include "../cor/body.hpp"
#include "../cor/vectools.hpp"
#include "../cor/phase.hpp"
#include "../cor/dynamics.hpp"
//#include "../cor/propagator.hpp"

int main(void) {

  // we load the ephemerides
  spice::load_kernels();

  // we create the spacecraft
  Spacecraft sc(660, 92.3e-3, 3337);

  // we create the planets
  Body earth("Earth");
  Body moon("Moon");
  Body sun("Sun");

  // we create a phase with segements and a spacecraft
  Phase phase(20, sc);

  // we add gravitational influence to the phase dynamics
  phase.add_body(earth);
  phase.add_body(moon);
  phase.add_body(sun);

  std::vector<double> control = vectools::random_unit_vec();
  std::vector<Body> bodies;
  for (int i=0; i<phase.bodies.size(); i++) {bodies.push_back(phase.bodies[i]);};

  Dynamics dyn(control, sc, bodies);

  //vectools::display(spice::state(spice::mjd2000("1/1/2011 00:00:00.000"),"392"));


  // we create the phase with segements and the spacecraft
  //Phase phase(20, sc);

  // we set the initial and final times
  //phase.set_times(4000, 4010);

  // we make apparent the planets within the phase
  //phase.add_body(earth);
  //phase.add_body(moon);
  //phase.add_body(sun);

  // we make an arbitrary maximum throttle control
  //Spacecraft::Control control = Spacecraft::Control::Random().normalized();

  // we compute a random circular LEO around Earth at a time
  //Spacecraft::State state;
  //state << random_orbit(earth, 4000), sc.mass;

  // we propagate the controlled dynamics in the phase
  //propagate(phase, state, control, phase.t0, phase.tN, 0.0001);


  return 0;
};
