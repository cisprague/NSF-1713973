#ifndef phase_hpp
#define phase_hpp
#include <vector>
#include "spacecraft.hpp"
#include "body.hpp"

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
  std::vector<double> mistmatch (void) const {
    // midpoint time
    const double tc(t0 + (tf-t0)/2);
    
  };

};

#endif
