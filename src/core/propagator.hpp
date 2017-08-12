#ifndef propagator_hpp
#define propagator_hpp
#include <vector>
#include <boost/numeric/odeint.hpp>
#include "dynamics.hpp"

struct Propagator {

  // members
  const Dynamics dynamics;
  std::vector<std::vector<double>> states;
  std::vector<double> times;

  // constructor
  Propagator (const Dynamics & dynamics_) : dynamics(dynamics_) {};

  // destructor
  ~Propagator (void) {};

  // numerical integration
  void operator () (
    const std::vector<double> & x0,
    const double & t0,
    const double & tf,
    const double & dt,
    const double & a_tol = 1e-10,
    const double & r_tol = 1e-10,
    const bool & disp = false
  ) {
    // make sure state is good
    if (x0.size() != 7) {throw "State must be 7D.";};
    // make sure it makes sense
    if (t0 < tf && dt <= 0) {
      throw "Time step must be positive for forwards propagtation.";
    };
    if (t0 > tf && dt >= 0) {
      throw "Time step must be negative for backwards propagtation.";
    };
    // integrator
    using namespace boost::numeric::odeint;
    typedef std::vector<double> state_type;
  };

};

#endif
