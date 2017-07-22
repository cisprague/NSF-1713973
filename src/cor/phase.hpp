#ifndef phase_hpp
#define phase_hpp
#include "spacecraft.hpp"
#include "body.hpp"

struct Phase {

  // a priori
  const int nseg;
  const Spacecraft spacecraft;
  std::vector<Body> bodies;

  // a posteriori
  std::vector<double> state0;
  std::vector<double> stateN;
  double t0;
  double tN;

  // constructor
  Phase (const int & nseg_, const Spacecraft & spacecraft_) :
    nseg(nseg_),
    spacecraft(spacecraft_),
    state0(7),
    stateN(7) {
  };

  // destructor
  ~Phase (void) {};

  // set initial time
  void set_initial_time (const double & t0_) {t0 = t0_;};

  // set final time
  void set_final_time (const double & tN_) {tN = tN_;};

  // set both times
  void set_times (const double & t0_, const double & tN_) {
    set_initial_time(t0_);
    set_final_time(tN_);
  };

  // set intial state
  void set_initial_state (const std::vector<double> & state0_) {
    state0 = state0_;
  };

  // set intial state
  void set_final_state (const std::vector<double> & stateN_) {
    stateN = stateN_;
  };

  // set both states
  void set_states (
    const std::vector<double> & state0_,
    const std::vector<double> & stateN_
  ) {
    set_initial_state(state0_);
    set_final_state(stateN_);
  };

  // set states and times
  void set_phase (
    const std::vector<double> & state0_,
    const std::vector<double> & stateN_,
    const double & t0_,
    const double & tN_
  ) {
    set_states(state0_, stateN_);
    set_times(t0_, tN_);
  };

  // add gravitational influences
  void add_body (const Body & body_) {
    bodies.push_back(body_);
  };



};
#endif
