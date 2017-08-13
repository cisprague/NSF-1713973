#ifndef phase_hpp
#define phase_hpp
#include <vector>
#include "spacecraft.hpp"
#include "body.hpp"
#include "dynamics.hpp"
#include "controller.hpp"
#include "propagator.hpp"
#include "constants.hpp"

struct Phase {

  // a priori
  const Spacecraft spacecraft;
  const std::vector<Body> bodies;

  // a posteriori
  std::vector<double> x0, xf;
  double t0, tf;

  // constructor
  Phase (
    const Spacecraft & spacecraft_,
    const std::vector<Body> & bodies_,
    const double & t0_,
    const double & tf_,
    const std::vector<double> & x0_,
    const std::vector<double> & xf_
  ) :
    spacecraft(spacecraft_), bodies(bodies_),
    x0(x0_), xf(xf_), t0(t0_), tf(tf_) {};


  // destructor
  ~Phase (void) {};

  // set phase
  void set (
    const std::vector<double> & x0_,
    const std::vector<double> & xf_,
    const double & t0_,
    const double & tf_
  ) {
    if (x0_.size() != 7 || xf_.size() != 7) {
      throw "State sizes must be 7D.";
    };
    x0 = x0_;
    xf = xf_;
    t0 = t0_;
    tf = tf_;
  };

  // compute mismatch
  std::vector<double> mismatch (
    const Controller & controller,
    const double & dt = 1e-3,
    const double & a_tol = 1e-12,
    const double & r_tol = 1e-12,
    const bool & disp = false,
    const bool & nondim = true
  ) const {
    // set up dynamics
    const Dynamics dynamics(spacecraft, bodies, controller);
    // set up propagator
    Propagator propagator(dynamics);
    // midpoint time
    const double tc(t0 + (tf-t0)/2.0);
    // propagate forward
    if (disp) {std::cout << "\nForward leg\n" << std::endl;};
    const std::vector<double> xcf(propagator(x0, t0, tc, dt, a_tol, r_tol, disp));
    // propagate backwards
    if (disp) {std::cout << "\nBackward leg\n" << std::endl;};
    const std::vector<double> xcb(propagator(xf, tf, tc, -dt, a_tol, r_tol, disp));
    // compute mismatch
    std::vector<double> ceq;
    for (int i=0; i<7; ++i) {ceq.push_back(xcf[i] - xcb[i]);};
    // scale and nondimensionalise
    if (nondim) {
      for (int i=0; i<3; ++i) {ceq[i] /= R_EARTH;};
      for (int i=3; i<6; ++i) {ceq[i] /= V_EARTH;};
      ceq[6] /= spacecraft.mass;
    };
    return ceq;
  };

};

#endif
