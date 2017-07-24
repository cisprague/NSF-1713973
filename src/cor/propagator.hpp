// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp
// NSF Award 1713973

#ifndef propagator_hpp
#define propagator_hpp
#include <vector>
#include <boost/numeric/odeint.hpp>
#include "dynamics.hpp"
#include "vectools.hpp"

namespace propagator {

  struct Recorder {

    // records
    std::vector<std::vector<double>> & states;
    std::vector<double>              & times;
    const bool                       & disp;

    // constructor
    Recorder (
      std::vector<std::vector<double>> & states_,
      std::vector<double>              & times_,
      const bool                       & disp_ = true
    ) : states(states_), times(times_), disp(disp_) {};

    // destructor
    ~Recorder (void) {};

    // recorder
    void operator () (const std::vector<double> & x, const double & t) {
      states.push_back(x);
      times.push_back(t);
      if (disp) {std::cout << t << std::endl; vectools::display(x);};
    };

  };

  struct Results {

    // the records
    const std::vector<std::vector<double>> states;
    const std::vector<double>              times;

    // constructor
    Results (
      const std::vector<std::vector<double>> & states_,
      const std::vector<double> & times_
    ) : states(states_), times(times_) {};

    // destructor
    ~Results (void) {};

  };

  Results propagate (
    std::vector<double> & x0,
    const double        & t0,
    const double        & tN,
    const double        & dt,
    const Dynamics      & dynamics,
    const bool          & display = false,
    const double        & a_tol = 1e-10,
    const double        & r_tol = 1e-10
  ) {

    // set up records for state and time
    std::vector<std::vector<double>> states;
    std::vector<double>              times;

    // instantiate a recorder with these references
    Recorder recorder(states, times, display);

    // set up integrator
    using namespace boost::numeric::odeint;
    typedef std::vector<double> state_type;
    typedef runge_kutta_fehlberg78<state_type> error_stepper_type;
    integrate_adaptive(
      make_controlled(a_tol, r_tol, error_stepper_type()),
      dynamics, x0, t0, tN, dt, recorder
    );

    // use results structure
    return Results(states, times);

  };


};

#endif
