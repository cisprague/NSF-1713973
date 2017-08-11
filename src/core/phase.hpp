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
    const std::vector<Body> & bodies_
  ) :
    spacecraft(spacecraft_),
    bodies(bodies_),
    x0(7),
    xf(7) {};


  // destructor
  ~Phase (void) {};

  // set phase
  void set (
    const std::vector<double> & x0_,
    const std::vector<double> & xf_,
    const double & t0_,
    const double & tf_
  ) {
    x0 = x0_;
    xf = xf_;
    t0 = t0_;
    tf = tf_;
  };

};

#endif
