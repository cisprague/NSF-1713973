// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp

#ifndef propagator_hpp
#define propagator_hpp
#include <vector>
#include <boost/numeric/odeint.hpp>
#include "dynamics.hpp"
#include "vectools.hpp"

namespace propagator {

  struct Record {
    // records
    const bool display;
    // constructor
    Record (const bool & disp = true) : display(disp) {};
    // destructor
    ~Record (void) {};
    // recorder
    void operator () (const std::vector<double> & x, const double & t) {
      if (display) {vectools::display(x);};
    };
  };

  void propagate (
    std::vector<double> & x0,
    const double        & t0,
    const double        & tN,
    const double        & dt,
    const Dynamics      & dynamics
  ) {

    // initialise the recorder
    Record record(false);

    // numerically integrate
    boost::numeric::odeint::integrate(dynamics, x0, t0, tN, dt, record);

  };


};

#endif
