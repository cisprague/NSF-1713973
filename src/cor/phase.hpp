// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp
// NSF Award 1713973

#ifndef phase_hpp
#define phase_hpp
#include <vector>
#include "spacecraft.hpp"
#include "body.hpp"
#include "dynamics.hpp"
#include "propagator.hpp"
#include "matplotlibcpp.h"

struct Phase {

  // a priori
  const int               nseg;
  const Spacecraft        spacecraft;
  const std::vector<Body> bodies;
  const int               nbodies;

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
    nbodies(bodies_.size()),
    x0(7),
    xN(7) {};

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
  ) {set_initial_state(x0_); set_final_state(xN_);};

  // set states and times
  void set_phase (
    const std::vector<double> & x0_,
    const std::vector<double> & xN_,
    const double              & t0_,
    const double              & tN_
  ) {set_states(x0_, xN_); set_times(t0_, tN_);};

  // propogate phase dynamics
  propagator::Results propagate (
    std::vector<double>       & x0,
    const std::vector<double> & u,
    const double              & t0,
    const double              & tN,
    const double              & dt,
    const bool                & display = false,
    const double              & a_tol = 1e-10,
    const double              & r_tol = 1e-10
  ) const {
    // set up dynamics with constant control
    Dynamics::Constant_Control dynamics(u, spacecraft, bodies);
    return propagator::propagate(x0, t0, tN, dt, dynamics, display, a_tol, r_tol);
  };

  void plot_traj (
    const propagator::Results & results,
    const std::string         & persp = "Earth"
  ) const {

    // number of points
    const int pts = results.times.size();
    // state vector
    std::vector<std::vector<double>> states(6, std::vector<double>(pts));
    // state of origin
    std::vector<double> pstate(6);

    // plot spacecraft trajectory
    for (int i=0; i<pts; ++i) {
      pstate = spice::state(results.times[i], persp);
      for (int dim=0; dim<6; ++dim) {
        states[dim][i] = results.states[i][dim] - pstate[dim];
      };
    };
    matplotlibcpp::named_plot("Spacecraft", states[0], states[1], "k-");

    // plot body trajectories
    std::vector<double> bstate(6);
    for (int i=0; i<nbodies; ++i) {
      for (int j=0; j<pts; ++j) {
        bstate = bodies[i].state(results.times[j], persp);
        for (int dim=0; dim<6; ++dim) {states[dim][j] = bstate[dim];};
      };
      matplotlibcpp::named_plot(bodies[i].name, states[0], states[1], ".");
    };

    matplotlibcpp::legend();
    matplotlibcpp::show();
  };

};
#endif
