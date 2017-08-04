// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp
// NSF Award 1713973

#ifndef phase_hpp
#define phase_hpp
#include <vector>
#include <cmath>
#include <utility>
#include "spacecraft.hpp"
#include "body.hpp"
#include "dynamics.hpp"
#include "propagator.hpp"
#include "controller.hpp"
#include "matplotlibcpp.h"

template <typename control_type>
struct Phase {

  // a priori
  const Spacecraft        spacecraft;
  const std::vector<Body> bodies;
  const int               nbodies;

  // a posteriori
  control_type controller;
  std::vector<double> x0;
  std::vector<double> xN;
  double t0;
  double tN;

  // constructor
  Phase (
    const Spacecraft        & spacecraft_,
    const std::vector<Body> & bodies_,
    const control_type      & controller_
  ) :
    spacecraft(spacecraft_),
    bodies(bodies_),
    nbodies(bodies_.size()),
    x0(7),
    xN(7),
    controller(controller_){};

  // constructor
  Phase (
    const Spacecraft          & spacecraft_,
    const std::vector<Body>   & bodies_,
    const control_type        & controller_,
    const std::vector<double> & x0_,
    const std::vector<double> & xN_,
    const double              & t0_,
    const double              & tN_
  ) :
    spacecraft(spacecraft_),
    bodies(bodies_),
    nbodies(bodies_.size()),
    controller(controller_),
    x0(x0_), xN(xN_), t0(t0_), tN(tN_) {};

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

  // propagate forwards and backwards
  std::pair<Propagator::Results, Propagator::Results> propagate_autonomous (void) {

    // we instantiate the dynamics
    Dynamics::Autonomous_Control<control_type> dynamics(bodies, spacecraft, controller);

    // we compute the matchpoint time
    const double tc(t0 + (tN-t0)/2);

    using namespace Propagator;
    // we propogate forward
    Results phase1(propagate(x0, t0, tc, 0.001, dynamics));
    // we propogate backward
    Results phase2(propagate(xN, tN, tc, -0.001, dynamics));

    // we bundle the results
    return std::pair<Results, Results>(phase1, phase2);
  };

  // get mismatch with current controller
  std::vector<double> mismatch (void) {

    // get results
    std::pair<Propagator::Results, Propagator::Results> results(propagate_autonomous());

    // define mismatch vector
    std::vector<double> mismatch(7);

    // compute mismatches
    for (int i=0, j=0; i<7, j<6; ++i, ++j) {
      mismatch.at(i) = results.second.states[i].back() - results.first.states[i].back();
    };

    return mismatch;
  };

  void plot (const int & xi, const int & yi, const std::string & persp = "SSB") {

    // get results
    std::pair<Propagator::Results, Propagator::Results> results(propagate_autonomous());
    std::vector<Propagator::Results> traj{results.first, results.second};

    // for each segement
    for (int i=0; i<2; ++i) {

      // times
      const std::vector<double> t(traj.at(i).times);
      // number of points
      const int npts(t.size());
      // states
      std::vector<double> x(traj.at(i).states.at(xi));
      std::vector<double> y(traj.at(i).states.at(yi));

      // for each time
      for (int j=0; j<npts; ++j) {
        // compute origin state
        const std::vector<double> so(spice::state(t.at(j), persp));
        // compute relative state
        x.at(j) -= so.at(xi);
        y.at(j) -= so.at(yi);
      };
      // plot the trajectory
      matplotlibcpp::plot(x, y, "k");

      // for each body
      for (int j=0; j<nbodies; ++j) {
        // states
        std::vector<double> x, y;
        // for each time
        for (int k=0; k<npts; ++k) {
          // compute state
          x.push_back(bodies.at(j).state(t.at(k), persp).at(xi));
          y.push_back(bodies.at(j).state(t.at(k), persp).at(yi));
        };
        matplotlibcpp::plot(x, y, "k,");
      };
    };
    matplotlibcpp::show();
  };

};
#endif
