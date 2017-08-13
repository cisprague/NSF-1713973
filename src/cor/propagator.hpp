#ifndef propagator_hpp
#define propagator_hpp
#include <vector>
#include <iostream>
#include <boost/numeric/odeint.hpp>
#include "dynamics.hpp"

struct Propagator {

  // members
  const Dynamics dynamics;
  std::vector<std::vector<double>> states;
  std::vector<double> times;

  // constructor
  Propagator (const Dynamics & dynamics_) : dynamics(dynamics_), states(7), times(0) {};

  // destructor
  ~Propagator (void) {};

  // numerical integration
  std::vector<double> operator () (
    std::vector<double> x0,
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
    // clear previous states and times
    for (int i=0; i<7; ++i) {states[i].clear();}
    times.clear();
    // integrator
    using namespace boost::numeric::odeint;
    typedef std::vector<double> state_type;
    typedef runge_kutta_fehlberg78<state_type> error_stepper_type;
    integrate_adaptive(
      make_controlled(a_tol, r_tol, error_stepper_type()),
      dynamics, x0, t0, tf, dt, Record(states, times, disp)
    );
    // final state
    std::vector<double> xf;
    for (int i=0; i<7; ++i) {xf.push_back(states[i].back());};
    return xf;
  };

  // recorder
  struct Record {

    // members
    const bool disp;
    std::vector<std::vector<double>> & states;
    std::vector<double> & times;

    // construct
    Record (
      std::vector<std::vector<double>> & states_,
      std::vector<double> & times_,
      const bool & disp_
    ) : states(states_), times(times_), disp(disp_) {};

    // destruct
    ~Record (void) {};

    void operator () (const std::vector<double> & x, const double & t) {
      // make sure state is good
      if (x.size() != 7) {throw "State must be 7D.";};
      // make sure record is good
      if (states.size() != 7) {throw "State record must be 7D";};
      // append each state
      for (int i=0; i<7; ++i) {states[i].push_back(x[i]);};
      // append each time
      times.push_back(t);
      // display
      if (disp) {
        std::cout << "time = " << t << "  state = [ ";
        for (int i=0; i<7; ++i) {
          std::cout << x[i] << " ";
        };
        std::cout << "]" <<  std::endl;
      };
    };

  };
};

#endif
