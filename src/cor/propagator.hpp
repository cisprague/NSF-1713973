// Christopher Iliffe Sprague
// cisprague@ac.jaxa.jp
// NSF Award 1713973

#ifndef propagator_hpp
#define propagator_hpp
#include <vector>
#include <boost/numeric/odeint.hpp>
#include "dynamics.hpp"
#include "linalg.hpp"
#include "spice.hpp"
#include "matplotlibcpp.h"

namespace Propagator {

  struct Recorder {

    // records
    std::vector<std::vector<double>> & states;
    const int                        dim;
    std::vector<double>              & times;
    const bool                       & disp;

    // constructor
    Recorder (
      std::vector<std::vector<double>> & states_,
      std::vector<double>              & times_,
      const bool                       & disp_ = true
    ) : states(states_), dim(states.size()), times(times_), disp(disp_) {};

    // destructor
    ~Recorder (void) {};

    // recorder
    void operator () (const std::vector<double> & x, const double & t) {
      for (int i=0; i<dim; ++i) {states[i].push_back(x[i]);};
      times.push_back(t);
      if (disp) {std::cout << t << std::endl; linalg::display_vec(x);};
    };

  };

  struct Results {

    // the records
    const std::vector<std::vector<double>> states;
    const std::vector<double>              times;
    const int                              npts;

    // constructor
    Results (
      const std::vector<std::vector<double>> & states_,
      const std::vector<double> & times_
    ) : states(states_), times(times_), npts(times_.size()) {};

    // destructor
    ~Results (void) {};

    // results with respect to a NAIF point
    std::vector<std::vector<double>> wrt(const std::string & persp) const {
      // states vectors
      std::vector<std::vector<double>> srel(6);
      // for each time
      for (int i=0; i<npts; ++i) {
        // compute point state
        const std::vector<double> sb(spice::state(times.at(i), persp));
        // for each dimension except mass
        for (int j=0; j<6; ++j) {
          // relative state
          srel.at(j).push_back(states.at(j).at(i) - sb.at(j));
        };
      };
      return srel;
    };

    // plotting
    void plot (const int & xi, const int & yi, const std::string & persp, const std::string & mark = "k") const {
      std::vector<std::vector<double>> states(wrt(persp));
      matplotlibcpp::plot(states.at(xi), states.at(yi), mark);
    };

  };

  template <typename T>
  Results propagate (
    std::vector<double> & x0,
    const double        & t0,
    const double        & tN,
    const double        & dt,
    const T             & dynamics,
    const double        & a_tol = 1e-10,
    const double        & r_tol = 1e-10,
    const bool          & disp = false
  ) {

    // set up records for state and time
    std::vector<std::vector<double>> states(7);
    std::vector<double>              times;

    // instantiate a recorder with these references
    Recorder recorder(states, times, disp);

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
