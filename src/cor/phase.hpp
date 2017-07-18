#ifndef phase_hpp
#define phase_hpp
#include <eigen3/Eigen/Dense>
#include <iostream>
#include <random>
#include <vector>
#include <stdexcept>
#include "body.hpp"
#include "spacecraft.hpp"

struct Phase {

  /*
    zi = {t0,tN,ux0,uy0,uz0,...,uxM,uyM,uzM}
    i = phase index
    j = node index
    N = number of nodes
    M = N-1 = number of segements
    O--------O--------O----...----O--------O--------O
    j=0      j=1      j=2         j=N-2    j=N-1    j=N
  */

  // a priori
  const int nseg;
  const std::vector<Body> bodies;

  // a posteriori
  double t0;
  double tN;
  Spacecraft::State state0;
  Spacecraft::State stateN;
  std::vector<Spacecraft::Control> controls;

  // constructor
  Phase (const int & nseg_) : nseg(nseg_) {
    controls.resize(nseg);
  };

  // destructor
  ~Phase (void) {};

  // set initial time
  void set_initial_time(const double & t0_) {t0 = t0_;};

  // set final time
  void set_final_time(const double & tN_) {tN = tN_;};

  // set both times
  void set_times(const double & t0_, const double & tN_) {
    set_initial_time(t0_);
    set_final_time(tN_);
  };

  // set intial state
  void set_initial_state(const Spacecraft::State state0_) {
    state0 = state0_;
  };

  // set intial state
  void set_final_state(const Spacecraft::State stateN_) {
    stateN = stateN_;
  };

  // set both states
  void set_states(const Spacecraft::State state0_, const Spacecraft::State stateN_) {
    set_initial_state(state0_);
    set_final_state(stateN_);
  };

  // set random controls
  void set_random_controls(void) {
    for (int seg=0; seg<nseg; seg++) {
      controls[seg] = Spacecraft::Control::Random().normalized();
    };
  };

  // set controls with throttle vector
  void set_throttles(const std::vector<double> throttles_) {
    if (throttles_.size() != nseg*3) {
      throw std::invalid_argument("Throttle vector must match.");
    };
    for (int i=0, seg=0; i<throttles_.size() && seg<nseg; i=i+3, seg++) {
      for (int dim=0; dim<3; dim++) {
        controls[seg][dim] = throttles_[i+dim];
      };
    };
  };




};


#endif
