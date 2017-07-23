// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp

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
    const std::vector<double> times;

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
    const bool          & display = false
  ) {

    // set up records for state and time
    std::vector<std::vector<double>> states;
    std::vector<double>              times;

    // instantiate a recorder with these references
    Recorder recorder(states, times, display);

    // numerically integrate
    boost::numeric::odeint::integrate(dynamics, x0, t0, tN, dt, recorder);

    // use results structure
    return Results(states, times);

  };


};

#endif
