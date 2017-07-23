#ifndef phase_hpp
#define phase_hpp
#include <vector>
#include "spacecraft.hpp"
#include "body.hpp"
#include "dynamics.hpp"
#include "propagator.hpp"

struct Phase {

  // a priori
  const int               nseg;
  const Spacecraft        spacecraft;
  const std::vector<Body> bodies;

  // a posteriori
  std::vector<double> x0;
  std::vector<double> xN;
  double t0;
  double tN;

  // constructor
  Phase (
    const int               & nseg_,
    const Spacecraft        & spacecraft_,
    const std::vector<Body> & bodies_
  ) :
    nseg(nseg_),
    spacecraft(spacecraft_),
    bodies(bodies_),
    x0(7),
    xN(7) {
  };

  // destructor
  ~Phase (void) {};

  // time setters
  void set_initial_time (const double & t0_) {t0 = t0_;};

  void set_final_time (const double & tN_) {tN = tN_;};

  void set_times (const double & t0_, const double & tN_) {
    set_initial_time(t0_);
    set_final_time(tN_);
  };

  // state setters
  void set_initial_state (const std::vector<double> & x0_) {x0 = x0_;};

  void set_final_state (const std::vector<double> & xN_) {xN = xN_;};

  void set_states (
    const std::vector<double> & x0_,
    const std::vector<double> & xN_
  ) {
    set_initial_state(x0_);
    set_final_state(xN_);
  };

  // set states and times
  void set_phase (
    const std::vector<double> & x0_,
    const std::vector<double> & xN_,
    const double              & t0_,
    const double              & tN_
  ) {set_states(x0_, xN_);set_times(t0_, tN_);};

  // propogate phase dynamics
  propagator::Results propagate (
    std::vector<double>       & x0,
    const std::vector<double> & u,
    const double              & t0,
    const double              & tN,
    const double              & dt,
    const bool                & display = false
  ) const {
    // set up dynamics with constant control
    Dynamics dynamics(u, spacecraft, bodies);
    return propagator::propagate(x0, t0, tN, dt, dynamics, display);
  };

};
#endif
